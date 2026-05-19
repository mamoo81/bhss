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

bool runPostgresAdmin(const QString &sql)
{
    QProcess prcs;
    prcs.start("pkexec", QStringList() << "sudo" << "-u" << "postgres" << "psql" << "-tAc" << sql);
    prcs.waitForFinished(30000);
    QString err = QString::fromLocal8Bit(prcs.readAllStandardError());
    if(prcs.exitCode() != 0){
        qDebug() << "Postgres admin failed:" << err;
        return false;
    }
    return true;
}

void veritabaniIlkleme(QSqlDatabase &pDB, Veritabani &pVT){
    QString currentUser = getCurrentOsUser();
    if(currentUser.isEmpty()){
        QMessageBox msg(0);
        msg.setWindowTitle("Hata");
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Sistem kullanıcısı tespit edilemedi.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }

    // Ensure current user has a PostgreSQL role
    QSqlQuery roleCheck(pDB);
    roleCheck.exec(QString("SELECT 1 FROM pg_roles WHERE rolname = '%1'").arg(currentUser));
    if(!roleCheck.next()){
        pDB.close();
        runPostgresAdmin(QString("DO $$ BEGIN IF NOT EXISTS (SELECT FROM pg_roles WHERE rolname = '%1') THEN CREATE ROLE %1 WITH LOGIN SUPERUSER CREATEDB; END IF; END $$;").arg(currentUser));
        if(!pDB.open()){
            QMessageBox err(0);
            err.setWindowTitle("Hata");
            err.setIcon(QMessageBox::Critical);
            err.setText("PostgreSQL rolü oluşturuldu ancak bağlantı kurulamadı.\nLütfen servisin çalıştığından emin olun.");
            err.setStandardButtons(QMessageBox::Ok);
            err.exec();
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
            if(!runPostgresAdmin(QString("CREATE DATABASE mhss_data OWNER %1;").arg(currentUser))){
                QMessageBox err(0);
                err.setWindowTitle("Hata");
                err.setIcon(QMessageBox::Critical);
                err.setText("mhss_data veritabanı oluşturulamadı.\nLütfen PostgreSQL servisinin çalıştığından emin olun.");
                err.setStandardButtons(QMessageBox::Ok);
                err.exec();
                return;
            }
            pDB.setDatabaseName("mhss_data");
            if(!pDB.open()){
                QMessageBox err(0);
                err.setWindowTitle("Hata");
                err.setIcon(QMessageBox::Critical);
                err.setText("mhss_data veritabanına bağlanılamadı.");
                err.setStandardButtons(QMessageBox::Ok);
                err.exec();
                return;
            }

            QMessageBox msg2(0);
            msg2.setWindowTitle("Bilgi");
            msg2.setIcon(QMessageBox::Information);
            if(pVT.veritabaniSifirla()){
                msg2.setText("Veritabanı oluşturuldu");
            }
            else {
                msg2.setText("Veritabanı oluşturulamadı");
            }
            msg2.setStandardButtons(QMessageBox::Ok);
            msg2.exec();
        }
    }
    else {
        pDB.close();
        pDB.setDatabaseName("mhss_data");
        if(!pDB.open()){
            QMessageBox err(0);
            err.setWindowTitle("Hata");
            err.setIcon(QMessageBox::Critical);
            err.setText("mhss_data veritabanına bağlanılamadı.");
            err.setStandardButtons(QMessageBox::Ok);
            err.exec();
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
    a.setApplicationVersion(QString("0.3.2"));

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

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mhss_data");
    db.setHostName("localhost");
    db.setUserName(currentUser);
    // No password needed with trust auth for localhost
    Veritabani vt;
    if(db.open()){
        veritabaniIlkleme(db, vt);
    }
    else{
        QMessageBox msg(0);
        msg.setWindowTitle("Uyarı");
        msg.setText("PostgreSQL servisine bağlanılamadı!\n\nOtomatik kurulum yapılsın mı?\n\nDikkat:\n- Bu işlem pg_hba.conf dosyasını değiştirecek.\n- Root şifresi isteyebilir.\n\nVeya terminalde şunu çalıştırabilirsiniz:\nsudo apt install postgresql\nsudo systemctl start postgresql");
        msg.setIcon(QMessageBox::Information);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(msg.exec() == QMessageBox::Yes){

            // postgresql kurulu mu kontrol et
            QProcess checkPsql;
            checkPsql.start("dpkg", QStringList() << "-l" << "postgresql");
            checkPsql.waitForFinished(10000);
            QString checkOutput = QString::fromLocal8Bit(checkPsql.readAllStandardOutput());
            bool psqlInstalled = checkOutput.contains("postgresql");

            if(!psqlInstalled){
                // postgresql in yüklenmesi
                QProcess *prcs = new QProcess();
                prcs->start("pkexec", QStringList() << "apt" << "install" << "postgresql" << "-y");
                msg.setWindowTitle("Paketler kuruluyor...");
                msg.setText("PostgreSQL kuruluyor... Lütfen bekleyin.");
                msg.setStandardButtons(QMessageBox::NoButton);
                msg.open();
                prcs->waitForFinished(300000);
                qDebug() << prcs->readAllStandardOutput().toStdString().c_str();
                qDebug() << prcs->readAllStandardError().toStdString().c_str();
                prcs->kill();
                msg.close();
                msg.setText("PostgreSQL kurulumu tamamlandı.");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
            }

            // pg_hba.conf dosyasını sed ile güncelle (peer/md5 -> trust)
            QProcess prcs_hba;
            prcs_hba.start("pkexec", QStringList() << "bash" << "-c" <<
                           "for f in /etc/postgresql/*/main/pg_hba.conf; do "
                           "sed -i 's/peer/trust/g' \"$f\"; "
                           "sed -i 's/md5/trust/g' \"$f\"; "
                           "sed -i 's/scram-sha-256/trust/g' \"$f\"; "
                           "done");
            prcs_hba.waitForFinished(30000);
            qDebug() << prcs_hba.readAllStandardOutput().toStdString().c_str();
            qDebug() << prcs_hba.readAllStandardError().toStdString().c_str();
            qDebug() << "pg_hba.conf güncellendi.";

            // Cluster kontrolü ve oluşturma
            QProcess prcs_ls;
            prcs_ls.start("pg_lsclusters");
            prcs_ls.waitForFinished(10000);
            QString clusterOutput = QString::fromLocal8Bit(prcs_ls.readAllStandardOutput()).trimmed();
            if(clusterOutput.isEmpty()){
                QProcess prcs_create;
                prcs_create.start("pkexec", QStringList() << "bash" << "-c" <<
                                  "ver=$(pg_lsclusters 2>/dev/null | awk 'NR==2{print $1}'); "
                                  "if [ -z \"$ver\" ]; then ver=$(ls /usr/lib/postgresql/ 2>/dev/null | sort -V | tail -n 1); fi; "
                                  "if [ -n \"$ver\" ]; then pg_createcluster $ver main --start; fi");
                prcs_create.waitForFinished(30000);
                qDebug() << prcs_create.readAllStandardOutput().toStdString().c_str();
                qDebug() << prcs_create.readAllStandardError().toStdString().c_str();
            }

            // postgresql servisini yeniden başlatma
            QProcess *prcs3 = new QProcess();
            prcs3->start("pkexec", QStringList() << "systemctl" << "restart" << "postgresql");
            prcs3->waitForFinished(30000);
            qDebug() << prcs3->readAllStandardOutput().toStdString().c_str();
            qDebug() << "Postgresql Servisi yeniden başlatıldı.";
            prcs3->kill();

            // Yeni bağlantı nesnesi oluştur (eski kapalı olabilir)
            QSqlDatabase::removeDatabase("mhss_data");
            db = QSqlDatabase::addDatabase("QPSQL", "mhss_data");
            db.setHostName("localhost");
            db.setUserName(currentUser);

            if(db.open()){
                // Create DB role for current user
                runPostgresAdmin(QString("DO $$ BEGIN IF NOT EXISTS (SELECT FROM pg_roles WHERE rolname = '%1') THEN CREATE ROLE %1 WITH LOGIN SUPERUSER CREATEDB; END IF; END $$;").arg(currentUser));
                veritabaniIlkleme(db, vt);
            }
            else{
                QMessageBox err(0);
                err.setWindowTitle("Hata");
                err.setIcon(QMessageBox::Critical);
                err.setText("PostgreSQL servisine hala bağlanılamadı.\n\nLütfen terminalde şu komutları çalıştırın:\n\nsudo apt install postgresql\nsudo systemctl start postgresql\nsudo systemctl enable postgresql\n\nArdından programı yeniden başlatın.");
                err.setStandardButtons(QMessageBox::Ok);
                err.exec();
            }
        }
    }
    LoginForm w;
    QTimer::singleShot(2000, &splash, SLOT(close()));
    QTimer::singleShot(2000, &w, SLOT(show()));

//        w.show();
//        splash.finish(&w); QTimer ile kapatacağım için buna gerek yok.
    return a.exec();
}
