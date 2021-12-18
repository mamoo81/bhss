#include "loginform.h"
#include "veritabani.h"
//***********************
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QLocale>
#include <QTranslator>
#include <QSharedMemory>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mhss_data");
    db.setHostName("localhost");
    db.setUserName("postgres");
    db.setPassword("postgres");
    if(db.open()){

        a.setOrganizationName("milis");
        a.setApplicationName("mhss");
        // ./home/user/.config/ altında mhss klasörü varmı kontrol
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
        LoginForm w;
        w.show();
        return a.exec();
    }
    else{
        QMessageBox msg(0);
        msg.setWindowTitle("Uyarı");
        msg.setText("PostgreSQL servisini kontrol edin. Konsola sırasıyla uygulayın;\n\nsudo servis ekle postgresql\nsudo servis aktif postgresql\nsudo servis kos postgresql");
        msg.setIcon(QMessageBox::Information);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        exit(0);
    }
}
