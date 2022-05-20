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
