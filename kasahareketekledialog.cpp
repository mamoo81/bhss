/*######################################
*MIT LICENCE                          #
*######################################
*Copyright 2021 Mehmet AKDEMİR        #
*bilgi@basat.dev                      #
*######################################
*Permission is hereby granted, free of charge,
*to any person obtaining a copy of this software and associated documentation files (the "Software"),
*to deal in the Software without restriction, including without limitation the rights to use, copy,
*modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
*and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
*INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
*DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "kasahareketekledialog.h"
#include "ui_kasahareketekledialog.h"
//**************************************
#include <QMessageBox>

KasaHareketEkleDialog::KasaHareketEkleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KasaHareketEkleDialog)
{
    ui->setupUi(this);

    ui->tarihdateEdit->setDateTime(QDateTime::currentDateTime());
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
    ui->HareketcomboBox->setCurrentIndex(hareket);
}

void KasaHareketEkleDialog::on_KaydetpushButton_clicked()
{
    if(ui->tutardoubleSpinBox->value() > 1){
        vt->KasaHareketiEkle(kullanici, ui->HareketcomboBox->currentText(), ui->tutardoubleSpinBox->value(), ui->AciklamaplainTextEdit->placeholderText(), ui->tarihdateEdit->dateTime(), ui->EvrakNolineEdit->text());
        QMessageBox msg(this);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Kasaya " + QString::number(ui->tutardoubleSpinBox->value(), 'f', 2) + " TL giriş yapıldı.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        this->close();
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Tutar 0 dan büyük olmalı.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}


void KasaHareketEkleDialog::on_iptalpushButton_clicked()
{
    this->close();
}

