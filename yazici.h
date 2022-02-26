#ifndef YAZICI_H
#define YAZICI_H
#include "sepet.h"
#include "user.h"
#include "cari.h"
#include "veritabani.h"
//********************************
#include <QString>

class Yazici
{
public:
    Yazici();

    Veritabani vt = Veritabani();
    void fisBas(QString _fisNo, Sepet _sepet);
    QString sirketAdi;
    QString sirketAdresi;
    QString yaziciModel;
    QString yazdirmaKomut;
    void setKullanici(const User &newKullanici);

public slots:
    void cikisRaporuBas(User _user);
    void tahsilatMakbuzuBas(User _user, Cari _cari, const double _tutar, QString _islemNo, QDateTime _islemTarihi, QString _aciklama);
private:

    User kullanici;
};

#endif // YAZICI_H
