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
#include "kgform.h"
#include "ui_kgform.h"
//**********************
#include <QMessageBox>
#include <QSerialPort>
#include <QDebug>
#include <QThread>

KgForm::KgForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KgForm)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::Dialog);
    terazi = new Terazi();
    connect(terazi, SIGNAL(StabilAgirlikOkununca(double)), this, SLOT(StabilOlcumGelince(double)));
    connect(terazi, SIGNAL(AgirlikOkununca(double)), this, SLOT(OlcumGelince(double)));
    terazi->start();
    ui->doubleSpinBox->setFocus();
}

void KgForm::StabilOlcumGelince(double olcum)
{
    ui->terazilcdNumber->display(QString::number(olcum, 'f', 3));
    ui->Stabillabel->setText("Stabil");
}

void KgForm::OlcumGelince(double olcum)
{
    ui->terazilcdNumber->display(QString::number(olcum, 'f', 3));
    ui->Stabillabel->setText("Ölçüm Stabil Değil!");
}

KgForm::~KgForm()
{
    terazi->quit();
    terazi->stop = true;
    disconnect(terazi, SIGNAL(StabilAgirlikOkununca(double)), this, SLOT(StabilOlcumGelince(double)));
    disconnect(terazi, SIGNAL(AgirlikOkununca(double)), this, SLOT(OlcumGelince(double)));
    delete ui;
}

float KgForm::getGirilenKg()
{
    return girilenKilo;
}

void KgForm::on_pushButton_clicked()
{
    if(ui->doubleSpinBox->value() > 0){
        girilenKilo = ui->doubleSpinBox->value();
        terazi->quit();
        terazi->stop = true;
        disconnect(terazi, SIGNAL(StabilAgirlikOkununca(double)), this, SLOT(StabilOlcumGelince(double)));
        disconnect(terazi, SIGNAL(AgirlikOkununca(double)), this, SLOT(OlcumGelince(double)));
        this->close();
    }
    else if(ui->doubleSpinBox->value() == 0 && ui->terazilcdNumber->value() > 0){
        girilenKilo = ui->terazilcdNumber->value();
        terazi->quit();
        terazi->stop = true;
        disconnect(terazi, SIGNAL(StabilAgirlikOkununca(double)), this, SLOT(StabilOlcumGelince(double)));
        disconnect(terazi, SIGNAL(AgirlikOkununca(double)), this, SLOT(OlcumGelince(double)));
        this->close();
    }
    else{
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Lütfen 0 'dan büyük bir değer giriniz.");
        msg.setStandardButtons(QMessageBox::Ok);
        // msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

void KgForm::setBirimi(const QString &newBirimi)
{
    birimi = newBirimi;
    if(birimi == "METRE"){
        ui->label->setText("Metre giriniz");
    }
    if(birimi == "METREKARE"){
        ui->label->setText("Metrekare giriniz");
    }
    if(birimi == "METREKÜP"){
        ui->label->setText("Metreküp giriniz");
    }
    if(birimi == "LİTRE"){
        ui->label->setText("Litre giriniz");
    }
}

void KgForm::on_pushButton_2_clicked()
{
    terazi->quit();
    terazi->stop = true;
    disconnect(terazi, SIGNAL(StabilAgirlikOkununca(double)), this, SLOT(StabilOlcumGelince(double)));
    disconnect(terazi, SIGNAL(AgirlikOkununca(double)), this, SLOT(OlcumGelince(double)));
    this->close();
}

