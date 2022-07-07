#include "etiketthread.h"

#include <QMutex>

EtiketThread::EtiketThread()
{

}

void EtiketThread::setKartlar(const QList<StokKarti> &value)
{
    kartlar = value;
}

void EtiketThread::run()
{
    foreach (StokKarti kart, kartlar) {
        // threadı ihtiyaç halinde sonlandırmak için.
        QMutex mutex;
        mutex.lock();
        if(this->stop){
            break;
        }
        mutex.unlock();

        etiketYazici.rafEtiketiBas(kart);
        emit yazdirilinca(1, kart.getBarkod());
        QThread::msleep(500);
    }
    emit yazdirmaBitince();
}
