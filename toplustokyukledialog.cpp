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
                sorgu.prepare("INSERT INTO stokkartlari(id, barkod, kod, marka, ad, birim, miktar, grup, afiyat, sfiyat, kdv, kdvdahil, otv, otvdahil, tarih, aciklama) "
                                "VALUES(nextval('stokkartlari_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                sorgu.bindValue(0, barkod);
                sorgu.bindValue(1, value["kod"].toString());
                sorgu.bindValue(2, value["marka"].toString());
                sorgu.bindValue(3, value["ad"].toString());
                sorgu.bindValue(4, value["birim"].toString());
                sorgu.bindValue(5, value["miktar"].toVariant().value<float>());
                sorgu.bindValue(6, value["grup"].toString());
                sorgu.bindValue(7, value["afiyat"].toDouble());
                sorgu.bindValue(8, value["sfiyat"].toDouble());
                sorgu.bindValue(9, value["kdv"].toInt());
                sorgu.bindValue(10, value["kdvdahil"].toBool());
                sorgu.bindValue(11, value["otv"].toInt());
                sorgu.bindValue(12, value["otvdahil"].toBool());
                sorgu.bindValue(13, value["tarih"].toVariant().value<QDateTime>());
                sorgu.bindValue(14, value["aciklama"].toString());
                sorgu.exec();
                if(sorgu.lastError().isValid()){
                    qWarning() << sorgu.lastError().text();
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

