#ifndef ETIKETTHREAD_H
#define ETIKETTHREAD_H
#include "stokkarti.h"

#include <QThread>

class EtiketThread : public QThread
{
    Q_OBJECT

    void run();

public:
    EtiketThread();

    bool stop = false;
    void setKartlar(const QList<StokKarti> &value);

private:
    QList<StokKarti> kartlar;

signals:
    void yazdirilinca(int);
};

#endif // ETIKETTHREAD_H
