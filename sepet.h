/*####################################
MIT LICENCE                          #
######################################
Copyright 2021 Mehmet AKDEMİR        #
bilgi@basat.dev                      #
######################################
Permission is hereby granted, free of charge,
to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef SEPET_H
#define SEPET_H
#include "stokkarti.h"
#include "urun.h"
//********************
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlTableModel>

class Sepet
{

private:
    QString ad;
    double odenenTutar;
    double kalanTutar;

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
    double getOdenenTutar() const;
    void setOdenenTutar(double newOdenenTutar);
    double getKalanTutar() const;
    void setKalanTutar(double newKalanTutar);

public slots:
    double getSepettekiKazanc();
};

#endif // SEPET_H
