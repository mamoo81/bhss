#include "stokhareketleridialog.h"
#include "ui_stokhareketleridialog.h"
#include "satisgrafigiform.h"
//***************************************
#include <QMessageBox>

StokHareketleriDialog::StokHareketleriDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StokHareketleriDialog)
{
    ui->setupUi(this);

}

StokHareketleriDialog::~StokHareketleriDialog()
{
    delete ui;
}

void StokHareketleriDialog::on_bitisdateEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if(dateTime.date() >= ui->baslangicdateEdit->date()){
        QDateTime bitTarih = dateTime;
        if(dateTime.date() != QDate::currentDate()){
            bitTarih.setTime(QTime(23, 59, 999));
        }
        else{
            bitTarih = QDateTime::currentDateTime();
        }
        getHareketler(stokKartiBarkod, ui->baslangicdateEdit->dateTime(), bitTarih);
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Bitiş tarihi Başlangıç tarihinden küçük olamaz!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}


void StokHareketleriDialog::on_baslangicdateEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if(dateTime.date() <= ui->bitisdateEdit->date()){
        auto basTarih = dateTime;
        basTarih.setTime(QTime());
        getHareketler(stokKartiBarkod, basTarih, ui->bitisdateEdit->dateTime());
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Başlangıç tarihi bitiş tarihinden büyük olamaz!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

void StokHareketleriDialog::setStokBarkod(const QString &newStokBarkod)
{
    stokKartiBarkod = newStokBarkod;
    ui->bitisdateEdit->setDateTime(QDateTime::currentDateTime());
    ui->baslangicdateEdit->setDate(QDate(2022, 1, 1));
    ui->baslangicdateEdit->setTime(QTime());//boş Qtime atayınca saati 00:00:00 yapar
    getHareketler(stokKartiBarkod, ui->baslangicdateEdit->dateTime(), QDateTime::currentDateTime());
}

void StokHareketleriDialog::getHareketler(QString _barkod, QDateTime _baslangicTarih, QDateTime _bitisTarih)
{
    ui->stokhareketleritableView->setModel(vt.getStokHareketleri(_barkod, _baslangicTarih, _bitisTarih));
    ui->stokhareketleritableView->setSortingEnabled(true);
    ui->stokhareketleritableView->resizeColumnsToContents();
    ui->stokhareketleritableView->clearSelection();
}

void StokHareketleriDialog::on_pushButton_clicked()
{
    SatisGrafigiForm *grafik = new SatisGrafigiForm(this);
    grafik->exec();
    delete grafik;
}

