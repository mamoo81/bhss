#ifndef SEPET_H
#define SEPET_H

#include "stokkarti.h"
#include "urun.h"
//********************
#include <QMessageBox>
#include <QSqlDatabase>

class Sepet
{
private:
    QString ad;


public:
    Sepet();

    QHash<QString, Urun> urunler;

    double sepetToplamTutari();
    bool sepetBosmu();
    void urunEkle(StokKarti _StokKarti, float _miktar);
    void urunArtir(QString _Barkod, float _miktar);
    void urunAzalt(QString _Barkod, float _miktar);
    void urunSil(QString _Barkod);
    void urunAdetCarp(QString _Barkod, float _miktar);
    void sepetiSil();
    bool urunSepetteVarmi(QString _Barkod);
    QHash<QString, Urun> sepetiGetir();
    Urun urunBilgileriniGetir(QString _Barkod);
    double paraYuvarla(double _toplam);
};

#endif // SEPET_H
