#ifndef FATURA_H
#define FATURA_H
#include "stokkarti.h"
#include "cari.h"
//-----------------------------------
#include <QDate>
#include <QObject>
#include <QHash>

class Fatura : QObject
{
    Q_OBJECT
public:
    Fatura();

private:
    QDate faturaTarihi;
    QString faturaNumarasi;
    QHash<QString, StokKarti> faturaKalemleri;
    double faturaToplamTutari;
    double faturaToplami;
    double kdvToplami;

};

#endif // FATURA_H
