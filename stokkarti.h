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
#ifndef STOKKARTI_H
#define STOKKARTI_H

#include <QString>
#include <QPixmap>
#include <QDateTime>
#include <QStandardPaths>
#include <QImage>
#include <QFile>
#include <QFileInfo>
#include <QImage>

class StokKarti
{

public:
    StokKarti();

    enum Birimler{
        Adet = 1,
        Kilogram = 2,
        Koli = 3,
        Palet = 4,
        Paket = 5,
        Metre = 6,
        Metrekare = 7,
        Metreküp = 8,
        Litre = 9
    };

    const QString &getId() const;
    void setId(const QString &newId);

    const QString &getBarkod() const;
    void setBarkod(const QString &newBarkod);

    const QString &getAd() const;
    void setAd(const QString &newAd);

    const int &getBirim() const;
    void setBirim(const int &newBirim);

    float getMiktar() const;
    void setMiktar(float newMiktar);

    const int &getGrup() const;
    void setGrup(const int &newGrup);

    double getAFiyat() const;
    void setAFiyat(double newAFiyat);

    double getSFiyat() const;
    void setSFiyat(double newSFiyat);

    int getKdv() const;
    void setKdv(int newKdv);

    const QDateTime &getTarih() const;
    void setTarih(const QDateTime &newTarih);

    const QString &getAciklama() const;
    void setAciklama(const QString &newAciklama);

    const QString &getKod() const;
    void setKod(const QString &newKod);

    int getOtv() const;
    void setOtv(int newOtv);

    bool getKdvdahil() const;
    void setKdvdahil(bool newKdvdahil);

    bool getOtvdahil() const;
    void setOtvdahil(bool newOtvdahil);

    QString getUretici() const;
    void setUretici(QString newUretici);

    QString getTedarikci() const;
    void setTedarikci(QString newTedarikci);

    const QPixmap &getResim() const;
    void setResim(const QPixmap &newResim);

    QImage getBarkodImg() const;

private:
    QString id;
    QString kod;
    QString barkod;
    QString ad;
    int birim;
    float miktar;
    int grup;
    double aFiyat;
    double sFiyat;
    int kdv;
    int otv;
    bool kdvdahil;
    bool otvdahil;
    QDateTime tarih;
    QString aciklama;
    QString uretici;
    QString tedarikci;
    QPixmap resim;
};

#endif // STOKKARTI_H
