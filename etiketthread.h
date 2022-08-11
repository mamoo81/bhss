#ifndef ETIKETTHREAD_H
#define ETIKETTHREAD_H
#include "stokkarti.h"
#include "yazici.h"

#include <QThread>

class EtiketThread : public QThread
{
    Q_OBJECT

    void run();

public:
    EtiketThread();

    Yazici etiketYazici = Yazici();

    bool stop = false;
    void setKartlar(const QList<StokKarti> &value);

    void setKagitTipi(int newKagitTipi);

private:
    QList<StokKarti> kartlar;
    int kagitTipi;

signals:
    void yazdirilinca(int, QString);
    void yazdirmaBitince();
};

#endif // ETIKETTHREAD_H
