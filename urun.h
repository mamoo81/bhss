#ifndef URUN_H
#define URUN_H

#include <QString>

class Urun
{
public:
    Urun();

    QString barkod;
    QString ad;
    double alisFiyat;
    double birimFiyat;
    QString birim;
    float miktar= 0;
    float stokMiktari = 0;
    double toplam = 0;
};

#endif // URUN_H
