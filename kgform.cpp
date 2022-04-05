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
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

void KgForm::setBirimi(const QString &newBirimi)
{
    birimi = newBirimi;
    if(birimi == "METRE"){
        ui->label->setText("Metre giriniz");
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

