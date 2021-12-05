#include "satisgosterdialog.h"
#include "ui_satisgosterdialog.h"
#include "veritabani.h"
#include "sepet.h"
#include "stokkarti.h"
#include "cari.h"
#include "yazici.h"
//*********************
#include <QSqlQuery>

SatisGosterDialog::SatisGosterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SatisGosterDialog)
{
    ui->setupUi(this);

    initTableWidgets();
}

SatisGosterDialog::~SatisGosterDialog()
{
    delete ui;
}

void SatisGosterDialog::initTableWidgets()
{
    ui->sepetTableWidget->setColumnWidth(0,135);
    ui->sepetTableWidget->setColumnWidth(1,200);
    ui->sepetTableWidget->setColumnWidth(2,80);
    ui->sepetTableWidget->setColumnWidth(3,88);
    ui->sepetTableWidget->setColumnWidth(4,60);
    ui->sepetTableWidget->setColumnWidth(5,100);
}

void SatisGosterDialog::sepetiCek()
{
    satisFaturaNo.truncate(10);
    Veritabani vt = Veritabani();
    satilmisSepet = vt.getSatis(satisFaturaNo);
    QSqlQuery qr = vt.getIslemInfo(satisFaturaNo);
    this->setWindowTitle(satisFaturaNo + " nolu " + qr.value(8).toString() + " işlemi");
    Cari cari = vt.getCariKart(qr.value(7).toString());
    ui->islemYapilanCariLabel->setText(cari.getAd());
    ui->odenenLabel->setText("₺" + qr.value(5).toString());
    ui->kalanLabel->setText("₺" + qr.value(6).toString());
    ui->tarihLabel->setText(qr.value(9).toDate().toString("dd.MM.yyyy") + " " + qr.value(9).toTime().toString("hh:mm"));
    ui->faturaTutarlabel->setText("₺" + QString::number(satilmisSepet.sepetToplamTutari()));
    int satirIndex = 0;
    foreach (auto urun, satilmisSepet.urunler) {
        ui->sepetTableWidget->insertRow(ui->sepetTableWidget->rowCount());
        ui->sepetTableWidget->setItem(satirIndex, 0, new QTableWidgetItem(urun.barkod));
        ui->sepetTableWidget->setItem(satirIndex, 1, new QTableWidgetItem(urun.ad));
        ui->sepetTableWidget->setItem(satirIndex, 2, new QTableWidgetItem(QString::number(urun.birimFiyat)));
        ui->sepetTableWidget->setItem(satirIndex, 3, new QTableWidgetItem(urun.birim));
        ui->sepetTableWidget->setItem(satirIndex, 4, new QTableWidgetItem(QString::number(urun.miktar)));
        ui->sepetTableWidget->setItem(satirIndex, 5, new QTableWidgetItem(QString::number(urun.toplam)));
        satirIndex++;
    }
}

void SatisGosterDialog::setSatisFaturaNo(const QString &newSatisFaturaNo)
{
    satisFaturaNo = newSatisFaturaNo;
}


void SatisGosterDialog::on_kapatpushButton_clicked()
{
    this->close();
}


void SatisGosterDialog::on_FisYazdirpushButton_clicked()
{
    fis.fisBas(satisFaturaNo, satilmisSepet);
    QMessageBox msg(this);
    msg.setWindowTitle("Bilgi");
    msg.setIcon(QMessageBox::Information);
    msg.setText("Bilgi fişi yazdırıldı.");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, "Tamam");
    msg.exec();
}

