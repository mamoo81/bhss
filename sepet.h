#ifndef SEPET_H
#define SEPET_H

#include "stokkarti.h"
#include "urun.h"
#include "satisform.h"
//********************
#include <QMessageBox>
#include <QSqlDatabase>

class Sepet
{
private:
    QString ad;
    QHash<QString, Urun> urunler;

public:
    Sepet();

    double sepetToplamTutari();
    void sepetiSat(QString _Kullanici);
    void urunEkle(StokKarti _StokKarti, float _miktar);
    void urunArtir(QString _Barkod, float _miktar);
    void urunAzalt(QString _Barkod, float _miktar);
    void urunSil(QString _Barkod);
    void sepetiSil();
    bool urunSepetteVarmi(QString _Barkod);
    QHash<QString, Urun> sepetiGetir();
    Urun urunBilgileriniGetir(QString _Barkod);
};

#endif // SEPET_H
