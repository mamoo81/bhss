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
#ifndef YAZICI_H
#define YAZICI_H
#include "sepet.h"
#include "user.h"
#include "cari.h"
#include "veritabani.h"
#include "stokkarti.h"
//********************************
#include <QString>
#include <QPrinter>

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
    QString etiketYazici;
    void setKullanici(const User &newKullanici);

    void rafEtiketiBas(StokKarti kart);

public slots:
    void cikisRaporuBas(User _user);
    void tahsilatMakbuzuBas(User _user, Cari _cari, const double _tutar, QString _islemNo, QDateTime _islemTarihi, QString _aciklama);

signals:
    void yazdirildi(QString _barkod);
private:

    User kullanici;
    QPrinter *printer = new QPrinter();
};

#endif // YAZICI_H
