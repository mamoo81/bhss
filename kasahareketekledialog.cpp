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
#include "kasahareketekledialog.h"
#include "ui_kasahareketekledialog.h"
//**************************************
#include <QMessageBox>
#include <QDebug>
#include <QThread>

KasaHareketEkleDialog::KasaHareketEkleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KasaHareketEkleDialog)
{
    ui->setupUi(this);
}

KasaHareketEkleDialog::~KasaHareketEkleDialog()
{
    delete ui;
}

void KasaHareketEkleDialog::setKullanici(User newKullanici)
{
    ui->Kullanicilabel->setText(newKullanici.getUserName());
    kullanici = newKullanici;
}

void KasaHareketEkleDialog::setHareket(int newHareket)
{
    hareket = newHareket;
    if(hareket == 0){
        oncekiHareket = "GİRİŞ";
        ui->KullaniciCikisipushButton->setVisible(false);
    }
    else if(hareket == 1){
        oncekiHareket = "ÇIKIŞ";
        ui->KullaniciCikisipushButton->setVisible(true);
    }
    ui->HareketcomboBox->setCurrentIndex(hareket);
}

void KasaHareketEkleDialog::on_KaydetpushButton_clicked()
{
    if(ui->tutardoubleSpinBox->value() > 1){
        if(!hareketDuzenle){
            int sonuc = kasaYonetimi.KasaHareketiEkle(kullanici, ui->HareketcomboBox->currentText(), ui->tutardoubleSpinBox->value(), ui->AciklamaplainTextEdit->toPlainText(), ui->tarihdateEdit->dateTime(), ui->EvrakNolineEdit->text(), 0);
            switch (sonuc) {
            case 1:
                this->close();
                break;
            case 0:
                ui->tutardoubleSpinBox->setFocus();
                ui->tutardoubleSpinBox->selectAll();
                break;
            }
        }
        else{
            int sonuc = kasaYonetimi.kasaHareketiDuzenle(kullanici, hareketID, ui->HareketcomboBox->currentText(), ui->tutardoubleSpinBox->value(), ui->AciklamaplainTextEdit->toPlainText(), ui->tarihdateEdit->dateTime(), ui->EvrakNolineEdit->text());
            switch (sonuc) {
            case 1:
                this->close();
                break;
            }
        }
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Tutar en az 1,00 TL den büyük olmalı.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}


void KasaHareketEkleDialog::on_iptalpushButton_clicked()
{
    this->close();
}

void KasaHareketEkleDialog::setHareketID(const QString &newHareketID)
{
    hareketID = newHareketID;
}

void KasaHareketEkleDialog::setEvrakNo(const QString &newEvrakNo)
{
    evrakNo = newEvrakNo;
    if(hareketDuzenle){
        ui->EvrakNolineEdit->setText(evrakNo);
    }
}

void KasaHareketEkleDialog::setTutar(double newTutar)
{
    tutar = newTutar;
    if(hareketDuzenle){
        ui->tutardoubleSpinBox->setValue(tutar);
    }
}

void KasaHareketEkleDialog::setAciklama(const QString &newAciklama)
{
    aciklama = newAciklama;
    if(hareketDuzenle){
        ui->AciklamaplainTextEdit->setPlainText(aciklama);
    }
}

void KasaHareketEkleDialog::sethareketDuzenle(bool newhareketDuzenle)
{
    hareketDuzenle = newhareketDuzenle;
}

void KasaHareketEkleDialog::setTarih(const QDateTime newTarih)
{
    tarih = newTarih;
    if(hareketDuzenle){
        ui->tarihdateEdit->setDateTime(tarih);
    }
    else{
        ui->tarihdateEdit->setDateTime(QDateTime::currentDateTime());
    }
}

void KasaHareketEkleDialog::on_tarihdateEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if(QDateTime::currentDateTime() < dateTime){
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Kasa giriş/çıkış işlemini ileri bir tarihe yapamazsınız!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        QThread::msleep(100);
        ui->KaydetpushButton->setEnabled(false);
    }
    else{
        ui->KaydetpushButton->setEnabled(true);
    }
}


void KasaHareketEkleDialog::on_KullaniciCikisipushButton_clicked()
{
    double cekilecekPara = kasaYonetimi.getKasadakiPara() - 100;

    ui->tutardoubleSpinBox->setValue(cekilecekPara);

}


void KasaHareketEkleDialog::on_HareketcomboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->KullaniciCikisipushButton->setVisible(false);
        break;
    case 1:
        ui->KullaniciCikisipushButton->setVisible(true);
    }
}

