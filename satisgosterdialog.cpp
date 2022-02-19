#include "satisgosterdialog.h"
#include "ui_satisgosterdialog.h"
#include "sepet.h"
#include "stokkarti.h"
#include "yazici.h"
//*********************
#include <QDebug>

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
    QStringList no = satisFaturaNo.split(" ");

    satisFaturaNo = no[0];
    satilmisSepet = vt->getSatis(satisFaturaNo);
    qr = vt->getIslemInfo(satisFaturaNo);
    switch (qr.value(11).toInt()) {
    case 2:
        this->setWindowTitle(satisFaturaNo + " nolu " + "SATIŞ" + " işlemi");
        break;
    case 3:
        this->setWindowTitle(satisFaturaNo + " nolu " + "İADE" + " işlemi");
        break;
    }
    cari = vt->getCariKart(qr.value(7).toString());
    ui->islemYapilanCariLabel->setText(cari.getAd());
    ui->odenenLabel->setText("₺" + QString::number(qr.value(5).toDouble(), 'f', 2));
    ui->kalanLabel->setText("₺" + QString::number(qr.value(6).toDouble(), 'f', 2));
    ui->tarihLabel->setText(qr.value(8).toDate().toString("dd.MM.yyyy") + " " + qr.value(8).toTime().toString("hh:mm"));
    ui->faturaTutarlabel->setText("₺" + QString::number(satilmisSepet.sepetToplamTutari(), 'f', 2));
    int satirIndex = 0;
    ui->sepetTableWidget->model()->removeRows(0, ui->sepetTableWidget->rowCount());
    foreach (auto urun, satilmisSepet.urunler) {
        ui->sepetTableWidget->insertRow(ui->sepetTableWidget->rowCount());
        ui->sepetTableWidget->setItem(satirIndex, 0, new QTableWidgetItem(urun.barkod));
        ui->sepetTableWidget->setItem(satirIndex, 1, new QTableWidgetItem(urun.ad));
        ui->sepetTableWidget->setItem(satirIndex, 2, new QTableWidgetItem(QString::number(urun.birimFiyat, 'f', 2)));
        ui->sepetTableWidget->setItem(satirIndex, 3, new QTableWidgetItem(urun.birim));
        ui->sepetTableWidget->setItem(satirIndex, 4, new QTableWidgetItem(QString::number(urun.miktar)));
        ui->sepetTableWidget->setItem(satirIndex, 5, new QTableWidgetItem(QString::number(urun.toplam, 'f', 2)));
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


void SatisGosterDialog::on_iadepushButton_clicked()
{

}

