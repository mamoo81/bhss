/*####################################
MIT LICENCE                          #
######################################
Copyright 2021 Mehmet AKDEMİR        #
bilgi@basat.dev                      #
######################################
Permission is hereby granted, free of charge,
to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "loginform.h"
#include "qprocess.h"
#include "veritabani.h"
//***********************
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QInputDialog>
#include <QLineEdit>
//***********************
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

QString getCurrentOsUser()
{
    QString user = qgetenv("USER");
    if(!user.isEmpty()) return user;
    struct passwd *pw = getpwuid(getuid());
    if(pw) return QString::fromLocal8Bit(pw->pw_name);
    return QString();
}

class SudoRunner
{
    QString m_password;
    QString m_errors;
public:
    bool askPassword(QWidget *parent) {
        bool ok;
        QString pwd = QInputDialog::getText(parent, "Yetkilendirme Gerekli",
            "Yönetici (root) şifresini girin:", QLineEdit::Password, "", &ok);
        if(ok && !pwd.isEmpty()) {
            m_password = pwd;
            return true;
        }
        return false;
    }

    bool run(const QStringList &args) {
        QProcess prcs;
        prcs.start("sudo", QStringList() << "-S" << "-k" << args);
        if(!m_password.isEmpty()) {
            prcs.write(m_password.toUtf8() + "\n");
        }
        prcs.waitForFinished(300000);
        QString stdoutStr = QString::fromLocal8Bit(prcs.readAllStandardOutput());
        QString stderrStr = QString::fromLocal8Bit(prcs.readAllStandardError());
        if(prcs.exitCode() != 0){
            m_errors += QString("Komut başarısız: sudo %1\nÇıkış kodu: %2\n%3\n%4\n\n")
                .arg(args.join(" "), QString::number(prcs.exitCode()), stdoutStr, stderrStr);
            return false;
        }
        return true;
    }

    bool hasErrors() const { return !m_errors.isEmpty(); }
    QString errors() const { return m_errors; }
    void clearErrors() { m_errors.clear(); }
};

bool runPostgresAdmin(SudoRunner &sudo, const QString &sql)
{
    QStringList args;
    args << "-u" << "postgres" << "psql" << "-tAc" << sql;
    return sudo.run(args);
}

void veritabaniIlkleme(QSqlDatabase &pDB, Veritabani &pVT, SudoRunner &sudo, QString &outErrors){
    QString currentUser = getCurrentOsUser();
    if(currentUser.isEmpty()){
        outErrors += "Sistem kullanıcısı tespit edilemedi.\n";
        return;
    }

    // Ensure current user has a PostgreSQL role
    QSqlQuery roleCheck(pDB);
    roleCheck.exec(QString("SELECT 1 FROM pg_roles WHERE rolname = '%1'").arg(currentUser));
    if(!roleCheck.next()){
        pDB.close();
        if(!runPostgresAdmin(sudo, QString("DO $$ BEGIN IF NOT EXISTS (SELECT FROM pg_roles WHERE rolname = '%1') THEN CREATE ROLE %1 WITH LOGIN SUPERUSER CREATEDB; END IF; END $$;").arg(currentUser))){
            outErrors += "PostgreSQL kullanıcı rolü oluşturulamadı.\n";
        }
        if(!pDB.open()){
            outErrors += "PostgreSQL rolü oluşturuldu ancak bağlantı kurulamadı.\n";
            return;
        }
    }

    // Check if mhss_data exists
    QSqlQuery sorgu(pDB);
    sorgu.exec("SELECT datname FROM pg_database WHERE datname = 'mhss_data'");
    if(!sorgu.next()){
        QMessageBox msg(0);
        msg.setText("Veritabanı bulunamadı.\n\nSıfırdan oluşturmak ister misiniz?");
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Question);
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);
        int cevap = msg.exec();
        if(cevap == QMessageBox::Yes){
            pDB.close();
            if(!runPostgresAdmin(sudo, QString("CREATE DATABASE mhss_data OWNER %1;").arg(currentUser))){
                outErrors += "mhss_data veritabanı oluşturulamadı.\n";
                return;
            }
            pDB.setDatabaseName("mhss_data");
            if(!pDB.open()){
                outErrors += "mhss_data veritabanına bağlanılamadı.\n";
                return;
            }

            if(!pVT.veritabaniSifirla()){
                outErrors += "Veritabanı şeması yüklenemedi (pg_restore başarısız).\n";
            }
        }
    }
    else {
        pDB.close();
        pDB.setDatabaseName("mhss_data");
        if(!pDB.open()){
            outErrors += "mhss_data veritabanına bağlanılamadı.\n";
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("mhss");

    // uygulama içindeki sağ klik menüleri türkçe olması için.
    QTranslator *tr_translator = new QTranslator();
    if (!tr_translator->load("qt_" + QLocale::system().name(), QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        qWarning() << "Qt translator load failed";
    }
    a.installTranslator(tr_translator);
    a.setApplicationVersion(QString("0.3.3"));

    QPixmap splashscreenimage(":/images/ui/basat-splash-screen.png");

    QSplashScreen splash(splashscreenimage);// png yerine bir form class tasarlayıp onu da gösterebilirsin.
    splash.show();
//    splash.showMessage("Uygulama başlatılıyor...", Qt::AlignVCenter);
    a.processEvents();

    // (/home/user/.config/) altında mhss klasörü varmı kontrol
    auto dizin = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + QCoreApplication::applicationName();
    if(!QFileInfo::exists(dizin)){
        QDir().mkdir(dizin);// mhss klasörünü oluşturma.
        // mhss klasörü içinde hizlibutonlar.ini dosyasını oluşturma başlangıcı.
        QFile hizlibutonlarini(dizin + "/hizlibutonlar.ini");
        if(!QFileInfo::exists(hizlibutonlarini.fileName())){
            hizlibutonlarini.open(QIODevice::ReadWrite);
        }
        QFile ayarlarini(dizin + "/genel.ini");
        if(!QFileInfo::exists(ayarlarini.fileName())){
            ayarlarini.open(QIODevice::ReadWrite);
        }
    }
    else{
        // mhss klasörü içinde hizlibutonlar.ini dosyasını oluşturma başlangıcı.
        QFile hizlibutonlarini(dizin + "/hizlibutonlar.ini");
        if(!QFileInfo::exists(hizlibutonlarini.fileName())){
            hizlibutonlarini.open(QIODevice::ReadWrite);
        }
        QFile ayarlarini(dizin + "/genel.ini");
        if(!QFileInfo::exists(ayarlarini.fileName())){
            ayarlarini.open(QIODevice::ReadWrite);
        }
    }
    // (/home/user/.local/) altında mhss klasörü varmı kontrol ve ekleme
    if(!QFileInfo::exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))){
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }
    // (/home/user/.local/mhss/barkodlar/) klasörü varmı kontrol ve ekleme
    auto barkodlarDizin = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/barkodlar/";
    if(!QFileInfo::exists(barkodlarDizin)){
        QDir().mkdir(barkodlarDizin);
    }

    QString currentUser = getCurrentOsUser();
    QString initErrors;
    SudoRunner sudo;

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mhss_data");
    db.setHostName("localhost");
    db.setUserName(currentUser);
    Veritabani vt;

    if(db.open()){
        veritabaniIlkleme(db, vt, sudo, initErrors);
    }
    else{
        QMessageBox msg(0);
        msg.setWindowTitle("Uyarı");
        msg.setText("PostgreSQL servisine bağlanılamadı!\n\nOtomatik kurulum yapılsın mı?\n\nDikkat:\n- Bu işlem pg_hba.conf dosyasını değiştirecek.\n- Bir kez root şifresi istenecek.\n\nVeya terminalde şunu çalıştırabilirsiniz:\nsudo apt install postgresql\nsudo systemctl start postgresql");
        msg.setIcon(QMessageBox::Information);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(msg.exec() == QMessageBox::Yes){
            if(!sudo.askPassword(nullptr)){
                initErrors += "Yönetici şifresi girilmedi.\n";
            }
            else {
                // postgresql kurulu mu kontrol et
                QProcess checkPsql;
                checkPsql.start("dpkg", QStringList() << "-l" << "postgresql");
                checkPsql.waitForFinished(10000);
                QString checkOutput = QString::fromLocal8Bit(checkPsql.readAllStandardOutput());
                bool psqlInstalled = checkOutput.contains("postgresql");

                if(!psqlInstalled){
                    msg.setWindowTitle("Paketler kuruluyor...");
                    msg.setText("PostgreSQL kuruluyor... Lütfen bekleyin.");
                    msg.setStandardButtons(QMessageBox::NoButton);
                    msg.open();
                    if(!sudo.run(QStringList() << "apt" << "install" << "postgresql" << "-y")){
                        initErrors += "PostgreSQL paketi kurulamadı.\n";
                    }
                    msg.close();
                    msg.setText("PostgreSQL kurulumu tamamlandı.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }

                // pg_hba.conf dosyasını sed ile güncelle (peer/md5 -> trust)
                if(!sudo.run(QStringList() << "bash" << "-c" <<
                               "for f in /etc/postgresql/*/main/pg_hba.conf; do "
                               "sed -i 's/peer/trust/g' \"$f\"; "
                               "sed -i 's/md5/trust/g' \"$f\"; "
                               "sed -i 's/scram-sha-256/trust/g' \"$f\"; "
                               "done")){
                    initErrors += "pg_hba.conf güncellenemedi.\n";
                }

                // Cluster kontrolü ve oluşturma
                QProcess prcs_ls;
                prcs_ls.start("pg_lsclusters");
                prcs_ls.waitForFinished(10000);
                QString clusterOutput = QString::fromLocal8Bit(prcs_ls.readAllStandardOutput()).trimmed();
                if(clusterOutput.isEmpty()){
                    if(!sudo.run(QStringList() << "bash" << "-c" <<
                                      "ver=$(pg_lsclusters 2>/dev/null | awk 'NR==2{print $1}'); "
                                      "if [ -z \"$ver\" ]; then ver=$(ls /usr/lib/postgresql/ 2>/dev/null | sort -V | tail -n 1); fi; "
                                      "if [ -n \"$ver\" ]; then pg_createcluster $ver main --start; fi")){
                        initErrors += "PostgreSQL cluster oluşturulamadı.\n";
                    }
                }

                // postgresql servisini yeniden başlatma
                if(!sudo.run(QStringList() << "systemctl" << "restart" << "postgresql")){
                    initErrors += "PostgreSQL servisi yeniden başlatılamadı.\n";
                }

                // Yeni bağlantı nesnesi oluştur (eski kapalı olabilir)
                QSqlDatabase::removeDatabase("mhss_data");
                db = QSqlDatabase::addDatabase("QPSQL", "mhss_data");
                db.setHostName("localhost");
                db.setUserName(currentUser);

                if(db.open()){
                    veritabaniIlkleme(db, vt, sudo, initErrors);
                }
                else{
                    initErrors += "PostgreSQL servisine hala bağlanılamadı.\n";
                }
            }
        }
    }

    if(!initErrors.isEmpty()){
        QMessageBox err(0);
        err.setWindowTitle("Veritabanı İlklendirme Hataları");
        err.setIcon(QMessageBox::Warning);
        err.setText("Veritabanı hazırlanırken bazı hatalar oluştu:\n\n" + initErrors + "\nLütfen terminalde şu komutları çalıştırın:\n\nsudo apt install postgresql\nsudo systemctl start postgresql\nsudo systemctl enable postgresql\n\nArdından programı yeniden başlatın.");
        err.setStandardButtons(QMessageBox::Ok);
        err.exec();
    }

    LoginForm w;
    QTimer::singleShot(2000, &splash, SLOT(close()));
    QTimer::singleShot(2000, &w, SLOT(show()));

    return a.exec();
}
