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
    QMutex mutex;
    mutex.lock();
    if(this->stop) break;
    mutex.unlock();
}
