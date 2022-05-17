#include "loginform.h"
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("mhss");

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mhss_data");
    db.setHostName("localhost");
    db.setUserName("postgres");
    db.setPassword("postgres");
    Veritabani vt = Veritabani();
    if(db.open()){
        //mhss_data veritabanı varmı. yoksa oluştur.
        QSqlQuery sorgu = QSqlQuery(db);
        sorgu.exec("SELECT datname FROM pg_database WHERE datname = 'mhss_data'");
        if(!sorgu.next()){
            QMessageBox msg(0);
            msg.setText("Veritabanı bulunamadı.\n\nSıfırdan oluşturmak ister misiniz?");
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Question);
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setDefaultButton(QMessageBox::No);
            msg.setButtonText(QMessageBox::Yes, "Evet");
            msg.setButtonText(QMessageBox::No, "Hayır");
            int cevap = msg.exec();
            if(cevap == QMessageBox::Yes){
                if(vt.veritabaniSifirla()){// sıfırla metodu vt yi yeniden yüklediği için bunu çağırıyorum.
                    QMessageBox msg(0);
                    msg.setWindowTitle("Bilgi");
                    msg.setIcon(QMessageBox::Information);
                    msg.setText("Veritabanı oluşturuldu");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.setButtonText(QMessageBox::Ok, "Tamam");
                    msg.exec();
                    qWarning("Veritabanı oluşturuldu.");
                }
            }
        }
        db.close();
        db.setDatabaseName("mhss_data");
        db.open();
        if(db.lastError().isValid()){
            qWarning(qPrintable(db.lastError().text()));
        }

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
        if(!QFileInfo::exists(QStandardPaths::writableLocation(QStandardPaths::DataLocation))){
            QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
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
    }
}
