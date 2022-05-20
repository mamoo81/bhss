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
#include "toplustokyukledialog.h"
#include "ui_toplustokyukledialog.h"
//*****************************************
#include <QStandardPaths>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

TopluStokYukleDialog::TopluStokYukleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopluStokYukleDialog)
{
    ui->setupUi(this);
}

TopluStokYukleDialog::~TopluStokYukleDialog()
{
    delete ui;
}

void TopluStokYukleDialog::on_SecpushButton_clicked()
{
    dosyaYolu = QFileDialog::getOpenFileName(this,
                                         "Stok Kartları Dosyasını Seçin",
                                         QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                         tr("JSON Dosyası (stok-kartlari.json)"));

    dosya.setFileName(dosyaYolu);
    QFileInfo dosyaBilgisi(dosya);
    ui->DosyaAdilabel->setText(dosyaBilgisi.fileName());
    dosya.open(QIODevice::ReadOnly | QIODevice::Text);
    QString string = dosya.readAll();
    dosya.close();
    QJsonDocument document = QJsonDocument::fromJson(string.toUtf8());
    QJsonObject object =document.object();
    ui->Bilgilabel->setText("Toplam: " + QString::number(object.keys().count()) + " adet stok kartı var");
}

void TopluStokYukleDialog::on_YuklepushButton_clicked()
{
    ui->Bilgilabel->setText("Aktarılıyor...");
    if(dosya.fileName() != ""){
        dosya.open(QIODevice::ReadOnly | QIODevice::Text);
        QString string = dosya.readAll();
        dosya.close();
        QJsonDocument document = QJsonDocument::fromJson(string.toUtf8());
        QJsonObject object =document.object();
        ui->progressBar->setMaximum(object.keys().count());
        foreach (QString barkod, object.keys()) {
            if(!vt.barkodVarmi(barkod)){
                QJsonValue value = object.value(barkod);
                sorgu.prepare("INSERT INTO stokkartlari(id, barkod, kod, ad, birim, miktar, grup, afiyat, sfiyat, kdv, kdvdahil, otv, otvdahil, tarih, uretici, tedarikci, aciklama) "
                                "VALUES(nextval('stokkartlari_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                sorgu.bindValue(0, barkod);
                sorgu.bindValue(1, value["kod"].toString());
                sorgu.bindValue(2, value["ad"].toString());
                //sorgu.bindValue(3, value["birim"].toString());
                sorgu.bindValue(3, value["birim"].toInt());
                if(ui->miktarcheckBox->isChecked()){// seçili ise miktarları 10000 adet/kg/metre girer.
                    sorgu.bindValue(4, 10000);
                }
                else{
                    sorgu.bindValue(4, value["miktar"].toVariant().value<float>());
                }
                sorgu.bindValue(5, value["grup"].toInt());
                if(ui->fiyatcheckBox->isChecked()){// seçili ise fiyatları 0 TL girer.
                    sorgu.bindValue(6, 0);
                    sorgu.bindValue(7, 0);
                }
                else{
                    sorgu.bindValue(6, value["afiyat"].toDouble());
                    sorgu.bindValue(7, value["sfiyat"].toDouble());
                }
                sorgu.bindValue(8, value["kdv"].toInt());
                sorgu.bindValue(9, value["kdvdahil"].toBool());
                sorgu.bindValue(10, value["otv"].toInt());
                sorgu.bindValue(11, value["otvdahil"].toBool());
                QDateTime zaman = value["tarih"].toVariant().value<QDateTime>();
                sorgu.bindValue(12, value["tarih"].toVariant().value<QDateTime>());
                sorgu.bindValue(13, value["uretici"].toInt());
                sorgu.bindValue(14, QVariant(QString()));
                sorgu.bindValue(15, value["aciklama"].toString());
                sorgu.exec();
                if(sorgu.lastError().isValid()){
                    qCritical(qPrintable(sorgu.lastError().text()));
                    basarisiz++;
                }
                yuklenen++;
            }
            else{
                esGecilen++;
            }
            ui->progressBar->setValue(ui->progressBar->value() + 1);
        }
        ui->Bilgilabel->setText("Aktarım tamamlandı.");
        QMessageBox msg(this);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Warning);
        msg.setText(tr("%1 stok kartı veritabanına girilmiştir.").arg(yuklenen) + tr("\n%1 adet stok kartı zaten mevcut.").arg(esGecilen) + tr("\n%1 adet stok kartı yüklemesi başarısız.").arg(basarisiz));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Önce Stok kartları dosyası seçilmeli.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

