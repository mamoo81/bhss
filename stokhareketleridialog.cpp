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
        // msg.setButtonText(QMessageBox::Ok, "Tamam");
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
        // msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

void StokHareketleriDialog::setStokKarti(const StokKarti &newStokKarti)
{
    kart = newStokKarti;
    stokKartiBarkod = newStokKarti.getBarkod();
    ui->bitisdateEdit->setDateTime(QDateTime::currentDateTime());
    ui->baslangicdateEdit->setDate(QDate(2022, 1, 1));
    ui->baslangicdateEdit->setTime(QTime());//boş Qtime atayınca saati 00:00:00 yapar
    getHareketler(stokKartiBarkod, ui->baslangicdateEdit->dateTime(), QDateTime::currentDateTime());
}

void StokHareketleriDialog::getHareketler(QString _barkod, QDateTime _baslangicTarih, QDateTime _bitisTarih)
{
    ui->stokhareketleritableView->setModel(stokYonetimi.getStokHareketleri(_barkod, _baslangicTarih, _bitisTarih));
    ui->stokhareketleritableView->setSortingEnabled(true);
    ui->stokhareketleritableView->resizeColumnsToContents();
    ui->stokhareketleritableView->clearSelection();
}

void StokHareketleriDialog::on_pushButton_clicked()
{
    SatisGrafigiForm *grafik = new SatisGrafigiForm(this);
    grafik->setStokKarti(kart);
    grafik->exec();
    delete grafik;
}

