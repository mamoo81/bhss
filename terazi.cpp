#include "terazi.h"

#include <QDebug>

Terazi::Terazi()
{

}

void Terazi::run()
{
    double agirlik = 0;
    QSerialPort serial;
    //terazi ayarların okunması
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    genelAyarlar.beginGroup("terazi");
    serial.setPortName(genelAyarlar.value("port").toString());// seriport adını ayarlama
    // seriport ayarlarını okuma ve setleme
    if(!genelAyarlar.value("baudrate").isNull()){// ilgili ayar ini dosyasında null değilse
        if(genelAyarlar.value("baudrate").toString() == "1200"){
            if(!serial.setBaudRate(QSerialPort::Baud1200))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("baudrate").toString() == "2400"){
            if(!serial.setBaudRate(QSerialPort::Baud2400))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("baudrate").toString() == "4800"){
            if(!serial.setBaudRate(QSerialPort::Baud4800))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("baudrate").toString() == "9600"){
            if(!serial.setBaudRate(QSerialPort::Baud9600))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("baudrate").toString() == "19200"){
            if(!serial.setBaudRate(QSerialPort::Baud19200))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("baudrate").toString() == "38400"){
            if(!serial.setBaudRate(QSerialPort::Baud38400))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("baudrate").toString() == "57600"){
            if(!serial.setBaudRate(QSerialPort::Baud57600))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("baudrate").toString() == "115200"){
            if(!serial.setBaudRate(QSerialPort::Baud115200))
                qDebug() << serial.errorString();
        }
    }
    else{ // null ise default ayar setleme
        if(!serial.setBaudRate(QSerialPort::Baud9600))
            qDebug() << serial.errorString();
    }

    if(!genelAyarlar.value("databits").isNull()){// ilgili ayar ini dosyasında null değilse
        if(genelAyarlar.value("databits").toString() == "Data5"){
            if(!serial.setDataBits(QSerialPort::Data5))
                qDebug() << serial.errorString();
        }

        else if(genelAyarlar.value("databits").toString() == "Data6"){
            if(!serial.setDataBits(QSerialPort::Data6))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("databits").toString() == "Data7"){
            if(!serial.setDataBits(QSerialPort::Data7))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("databits").toString() == "Data8"){
            if(!serial.setDataBits(QSerialPort::Data8))
                qDebug() << serial.errorString();
        }
    }
    else{ // null ise default ayar setleme
        if(!serial.setDataBits(QSerialPort::Data8))
            qDebug() << serial.errorString();
    }

    if(!genelAyarlar.value("parity").isNull()){// ilgili ayar ini dosyasında null değilse
        if(genelAyarlar.value("parity").toString() == "NoParity"){
            if(!serial.setParity(QSerialPort::NoParity))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("parity").toString() == "EvenParity"){
            if(!serial.setParity(QSerialPort::EvenParity))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("parity").toString() == "OddParity"){
            if(!serial.setParity(QSerialPort::OddParity))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("parity").toString() == "SpaceParity"){
            if(!serial.setParity(QSerialPort::SpaceParity))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("parity").toString() == "MarkParity"){
            if(!serial.setParity(QSerialPort::MarkParity))
                qDebug() << serial.errorString();
        }
    }
    else{ // null ise default ayar setleme
        if(!serial.setParity(QSerialPort::NoParity))
            qDebug() << serial.errorString();
    }

    if(!genelAyarlar.value("stopbits").isNull()){// ilgili ayar ini dosyasında null değilse
        if(genelAyarlar.value("stopbits").toString() == "OneStop"){
            if(!serial.setStopBits(QSerialPort::OneStop))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("stopbits").toString() == "OneAndHalfStop"){
            if(!serial.setStopBits(QSerialPort::OneAndHalfStop))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("stopbits").toString() == "TwoStop"){
            if(!serial.setStopBits(QSerialPort::TwoStop))
                qDebug() << serial.errorString();
        }
    }
    else{ // null ise default ayar setleme
        if(!serial.setStopBits(QSerialPort::OneStop))
            qDebug() << serial.errorString();
    }

    if(!genelAyarlar.value("flowcontrol").isNull()){// ilgili ayar ini dosyasında null değilse
        if(genelAyarlar.value("flowcontrol").toString() == "NoFlowControl"){
            if(!serial.setFlowControl(QSerialPort::NoFlowControl))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("flowcontrol").toString() == "HardwareControl"){
            if(!serial.setFlowControl(QSerialPort::HardwareControl))
                qDebug() << serial.errorString();
        }
        else if(genelAyarlar.value("flowcontrol").toString() == "SoftwareControl"){
            if(!serial.setFlowControl(QSerialPort::SoftwareControl))
                qDebug() << serial.errorString();
        }
    }
    else{ // null ise default ayar setleme
        if(!serial.setFlowControl(QSerialPort::NoFlowControl))
            qDebug() << serial.errorString();
    }

    genelAyarlar.endGroup();
    // seriport ayarlarının ini dosyasından okuma bitiş

    if(!serial.open(QIODevice::ReadOnly))
        qDebug() << serial.errorString();

//    qDebug() << serial.bytesAvailable();
    while(serial.isOpen())
    {
        QMutex mutex;
        mutex.lock();
        if(this->stop) break;
        mutex.unlock();
        if(!serial.waitForReadyRead(-1)){ //block until new data arrives
            qDebug() << "error: " << serial.errorString();
            break;
        }
        else{
//            qDebug() << "New data available: " << serial.bytesAvailable();
            QString data = serial.readAll();

            if(data.size() >= 8){
                data.chop(2);
                data.remove(0,2);
                data.truncate(8);
            }
            if(data.contains("S")){
                data.replace(",", ".");
                agirlik = data.mid(3,5).toDouble();
                emit StabilAgirlikOkununca(agirlik);
            }
            else{
                data.replace(",", ".");
                agirlik = data.mid(3,5).toDouble();
                emit AgirlikOkununca(agirlik);
            }
            data.clear();
        }
    }
}
