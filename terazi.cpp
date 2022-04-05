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
    serial.setPortName(genelAyarlar.value("port").toString());
    qDebug() << serial.portName();
    genelAyarlar.endGroup();

    if(!serial.setBaudRate(QSerialPort::Baud9600))
        qDebug() << serial.errorString();
    if(!serial.setDataBits(QSerialPort::Data8))
        qDebug() << serial.errorString();
    if(!serial.setParity(QSerialPort::NoParity))
        qDebug() << serial.errorString();
    if(!serial.setStopBits(QSerialPort::OneStop))
        qDebug() << serial.errorString();
    if(!serial.open(QIODevice::ReadOnly))
        qDebug() << serial.errorString();
    if(!serial.setFlowControl(QSerialPort::NoFlowControl))
        qDebug() << serial.errorString();
//    qDebug() << serial.bytesAvailable();
    while(serial.isOpen())
    {
        QMutex mutex;
        mutex.lock();
        if(this->stop) break;
        mutex.unlock();
        if(!serial.waitForReadyRead(-1)) //block until new data arrives
            qDebug() << "error: " << serial.errorString();
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
