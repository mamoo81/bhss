#include "kgform.h"
#include "ui_kgform.h"
//**********************
#include <QMessageBox>
#include <QSerialPort>
#include <QDebug>

KgForm::KgForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KgForm)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::Dialog);

    QSerialPort serial;
        serial.setPortName("ttyUSB0");
        if(!serial.setBaudRate(QSerialPort::Baud9600))
            qDebug() << serial.errorString();
        if(!serial.setDataBits(QSerialPort::Data7))
            qDebug() << serial.errorString();
        if(!serial.setParity(QSerialPort::EvenParity))
            qDebug() << serial.errorString();
        if(!serial.setFlowControl(QSerialPort::HardwareControl))
            qDebug() << serial.errorString();
        if(!serial.setStopBits(QSerialPort::OneStop))
            qDebug() << serial.errorString();
        if(!serial.open(QIODevice::ReadOnly))
            qDebug() << serial.errorString();
        QObject::connect(&serial, &QSerialPort::readyRead, [&]
        {
            //this is called when readyRead() is emitted
            qDebug() << "New data available: " << serial.bytesAvailable();
            QByteArray datas = serial.readAll();
            qDebug() << datas;
            if(datas.at(0) == 'S'){

            }

        });

    ui->doubleSpinBox->setFocus();
}

KgForm::~KgForm()
{
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


