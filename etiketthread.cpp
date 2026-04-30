#include "etiketthread.h"

#include <QSettings>

EtiketThread::EtiketThread()
{
    connect(this, &QThread::finished, this, &QObject::deleteLater);
}

void EtiketThread::setKartlar(const QList<StokKarti> &value)
{
    kartlar = value;
}

void EtiketThread::setKagitTipi(int newKagitTipi)
{
    kagitTipi = newKagitTipi;
}

void EtiketThread::run()
{
    for (StokKarti kart : kartlar) {
        // threadı ihtiyaç halinde sonlandırmak için.
        if(stop.load()){
            break;
        }

        etiketYazici.rafEtiketiBas(kart, kagitTipi);
        emit yazdirilinca(1, kart.getBarkod());
        QThread::msleep(500);
    }
    emit yazdirmaBitince();
}
