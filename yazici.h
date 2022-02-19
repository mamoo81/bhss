#ifndef YAZICI_H
#define YAZICI_H
#include "sepet.h"
#include "user.h"
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
    void setKullanici(const User &newKullanici);

public slots:
    void cikisRaporuBas(User _user);
private:

    User kullanici;
};

#endif // YAZICI_H
