#include "loginform.h"
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
    a.setOrganizationName("milis");
    a.setApplicationName("mhss");
    // program birden fazla çalışmaması için kontrol
    QSharedMemory shared(QApplication::applicationName());
    if(!shared.create(512, QSharedMemory::ReadWrite)){
        QMessageBox msg(0);
        msg.setWindowTitle("Uyarı");
        msg.setText("Program zaten çalışıyor.");
        msg.setIcon(QMessageBox::Information);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        exit(0);
    }
    else{
        // .config altında mhss klasörü varmı kontrol
        auto dizin = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + QCoreApplication::applicationName();
        if(!QFileInfo::exists(dizin)){
            QDir().mkdir(dizin);// mhss klasörünü oluşturma.
            // mhss klasörü içinde hizlibutonlar.ini dosyasını oluşturma başlangıcı.
            QFile hizlibutonlarini(dizin + "/hizlibutonlar.ini");
            if(!QFileInfo::exists(hizlibutonlarini.fileName())){
                hizlibutonlarini.open(QIODevice::ReadWrite);
            }
        }
        else{
            // mhss klasörü içinde hizlibutonlar.ini dosyasını oluşturma başlangıcı.
            QFile hizlibutonlarini(dizin + "/hizlibutonlar.ini");
            if(!QFileInfo::exists(hizlibutonlarini.fileName())){
                hizlibutonlarini.open(QIODevice::ReadWrite);
            }
        }
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mhss_data");
        db.setHostName("localhost");
        db.setUserName("postgres");
        db.setPassword("postgres");
        if(!db.open())
            qFatal("veritabanı bağlantı hatası: %s",
                qPrintable(db.lastError().text()));
        QTranslator translator;
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages) {
            const QString baseName = "mhss_" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                a.installTranslator(&translator);
                break;
            }
        }
        LoginForm w;
        w.show();
        return a.exec();
    }
}
