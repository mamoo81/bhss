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

void veritabaniIlkleme(QSqlDatabase pDB, Veritabani pVT){
    //mhss_data veritabanı varmı. yoksa oluştur.
    QSqlQuery sorgu = QSqlQuery(pDB);
    sorgu.exec("SELECT datname FROM pg_database WHERE datname = 'mhss_data'");
    if(!sorgu.next()){
        QMessageBox msg(0);
        msg.setText("Veritabanı bulunamadı.\n\nSıfırdan oluşturmak ister misiniz?");
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Question);
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);
        // msg.setButtonText(QMessageBox::Yes, "Evet");
        // msg.setButtonText(QMessageBox::No, "Hayır");
        int cevap = msg.exec();
        if(cevap == QMessageBox::Yes){
            QMessageBox msg(0);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Veritabanı oluşturuldu");
            msg.setStandardButtons(QMessageBox::Ok);
            // msg.setButtonText(QMessageBox::Ok, "Tamam");
            if(pVT.veritabaniSifirla()){// sıfırla metodu vt yi yeniden yüklediği için bunu çağırıyorum.
                msg.exec();
            }
            else {
                msg.setText("Veritabanı oluşturulamadı");
                msg.exec();
            }
        }
    }
    pDB.close();
    pDB.setDatabaseName("mhss_data");
    pDB.open();
    if(pDB.lastError().isValid()){
        qDebug() << qPrintable(pDB.lastError().text());
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("mhss");

    // uygulama içindeki sağ klik menüleri türkçe olması için.
    QTranslator *tr_translator = new QTranslator();
    tr_translator->load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(tr_translator);
    a.setApplicationVersion(QString("0.2.5"));

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

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mhss_data");
    db.setHostName("localhost");
    db.setUserName("postgres");
    db.setPassword("postgres");
    Veritabani vt = Veritabani();
    if(db.open()){
        veritabaniIlkleme(db, vt);
    }
    else{
        QMessageBox msg(0);
        msg.setWindowTitle("Uyarı");
        msg.setText("PostgreSQL servisine bağlanılamadı! yüklenilsin mi?\n\nDikkat bu işlem:\n\"/etc/postgresql/<versiyon>/main/pg_hba.conf\"\ndosyasını değiştirecek.\nEğer başka bir uygulama için özel bir ayar barındırıyorsa yedek alın ve gözden geçirin.\n\nİşlem için bir kaç kez root şifresi isteyebilir.");
        msg.setIcon(QMessageBox::Information);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(msg.exec() == QMessageBox::Yes){

            // postgresql in yüklenmesi
            QProcess *prcs = new QProcess();
            if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){

            }
            else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
                prcs->start("pkexec", {"sudo", "apt", "--reinstall", "install", "postgresql", "-y"});
            }
            msg.setWindowTitle("Paketler kuruluyor... Lütfen bekleyin.");
            msg.setText("postgresql kuruluyor... Lütfen bekleyin.                    .");
            msg.setStandardButtons(QMessageBox::NoButton);
            msg.open();
            if(prcs->waitForFinished()){
                qDebug() << prcs->readAllStandardOutput().toStdString().c_str();
                prcs->kill();
                msg.close();
            }
            msg.setText("postgresql kuruldu.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();

            // pg_hba.conf dosyasını değiştir. trust
            QFile pg_hba(":/dosyalar/pg_hba.conf");
            pg_hba.copy(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/pg_hba.conf");
            QDir etcpostgresql("/etc/postgresql/");
            QStringList psqlDirList = etcpostgresql.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            foreach (QString dirName, psqlDirList) {
                QProcess *prcs2 = new QProcess();
                prcs2->start("pkexec", {"sudo", "cp", "/tmp/pg_hba.conf", QString(etcpostgresql.absolutePath() + "/" + dirName + "/main/pg_hba.conf")});
                if(prcs2->waitForFinished()){
                    qDebug() << prcs2->readAllStandardOutput().toStdString().c_str();
                    qDebug() << "pg_hba.conf aktarımı tamamlandı.";
                    prcs2->kill();
                }
            }

            // postgresql servisini yeniden başlatma
            QProcess *prcs3 = new QProcess();
            prcs3->start("pkexec", {"sudo", "systemctl", "restart", "postgresql"});
            if(prcs3->waitForFinished()){
                qDebug() << prcs3->readAllStandardOutput().toStdString().c_str();
                qDebug()<< "Postgresql Servisi yeniden başlatıldı.";
                prcs3->kill();
            }

            // postgres kullanıcısının şifresini postgres yapma
            QProcess *prcs4 = new QProcess();
            prcs4->setProcessChannelMode(QProcess::MergedChannels);
            prcs4->start("pkexec", {"sudo", "passwd", "postgres"});
            if(prcs4->waitForReadyRead()){
                qDebug() << prcs4->readAllStandardOutput().toStdString().c_str();
            }
            else {
                qDebug() << prcs4->errorString();
            }
            prcs4->write("postgres\n"); // \n eklemek gerekiyor yazılacak stringin sonuna
            if(prcs4->waitForReadyRead()){
                qDebug() << prcs4->readAllStandardOutput().toStdString().c_str();
            }
            else {
                qDebug() << prcs4->errorString();
            }
            prcs4->write("postgres\n");
            if(prcs4->waitForReadyRead()){
                qDebug() << prcs4->readAllStandardOutput().toStdString().c_str();
            }
            else {
                qDebug() << prcs4->errorString();
            }
            prcs4->kill();

            // postgresql sunucuda postgres şifresini güncelleme
            QProcess *prcs5 = new QProcess();
            prcs5->setProcessChannelMode(QProcess::MergedChannels);
            prcs5->start("pkexec", {"psql", "-U", "postgres", "-c", "alter user postgres with password 'postgres';"});
            if(prcs5->waitForReadyRead()){
                qDebug() << prcs5->readAllStandardOutput().toStdString().c_str();
            }
            else{
                qDebug() << prcs5->errorString().toStdString().c_str();
            }
            prcs5->kill();
            veritabaniIlkleme(db, vt);
        }
    }
    LoginForm w;
    QTimer::singleShot(2000, &splash, SLOT(close()));
    QTimer::singleShot(2000, &w, SLOT(show()));

//        w.show();
//        splash.finish(&w); QTimer ile kapatacağım için buna gerek yok.
    return a.exec();
}
