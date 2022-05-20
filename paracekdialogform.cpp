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
#include "paracekdialogform.h"
#include "ui_paracekdialogform.h"
#include "user.h"
#include "veritabani.h"

ParaCekDialogForm::ParaCekDialogForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParaCekDialogForm)
{
    ui->setupUi(this);
    ui->CiroLabel->setText("₺" + QString::number(vt.getGunlukCiro()));
    ui->doubleSpinBox->setFocus();
}

ParaCekDialogForm::~ParaCekDialogForm()
{
    delete ui;
}

void ParaCekDialogForm::setKasadakiPara(double newKasadakiPara)
{
    ui->KasadakiParalabel->setText("₺" + QString::number(newKasadakiPara));
    kasadakiPara = newKasadakiPara;
}

void ParaCekDialogForm::on_IptalpushButton_clicked()
{
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle("Bilgi");
    msg.setText(QString("Kasadan para çekimi iptal edildi."));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, "Tamam");
    msg.setDefaultButton(QMessageBox::Ok);
    msg.exec();
    this->close();
}


void ParaCekDialogForm::on_CekpushButton_clicked()
{
    if(kasadakiPara >= ui->doubleSpinBox->value()){
        vt.kasadanParaCek(ui->doubleSpinBox->value(), kull);
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setWindowTitle("Bilgi");
        msg.setText(QString("Kasadan ₺%1 lira çekildi.").arg(QString::number(ui->doubleSpinBox->value())));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
        this->close();
    }
    else{
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle("Bilgi");
        msg.setText(QString("Kasada ki miktardan yüksek olamaz.").arg(QString::number(ui->doubleSpinBox->value())));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
    }
}

void ParaCekDialogForm::setKull(const User &newKull)
{
    kull = newKull;
}


void ParaCekDialogForm::on_doubleSpinBox_valueChanged(double curValue)
{
    ui->kalanParaLabel->setText("₺" + QString::number(kasadakiPara - curValue));
}

