#ifndef YAZICI_H
#define YAZICI_H
#include "sepet.h"
//********************************
#include <QString>


class Yazici
{
public:
    Yazici();

    void fisBas(QString _fisNo, Sepet _sepet);
    QString sirketAdi;
    QString sirketAdresi;
    QString yaziciModel;
    QString yazdirmaKomut;
private:

};

#endif // YAZICI_H
