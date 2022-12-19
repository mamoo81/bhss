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
#ifndef CARI_H
#define CARI_H

#include <QString>
#include <QDateTime>
#include <QLocale>

class Cari
{
public:
    Cari();

    enum BorcHesaplama {
        SatildigiFiyattan = 1,
        GuncelFiyattan = 2
    };


    const QString &getAd() const;
    void setAd(const QString &newAd);

    int getId() const;
    void setId(int newId);

    const QString &getVerigino() const;
    void setVerigino(const QString &newVerigino);
    const QString &getVergiDaire() const;
    void setVergiDaire(const QString &newVergiDaire);
    const QString &getTcNo() const;
    void setTcNo(const QString &newTcNo);
    const QString &getAdres() const;
    void setAdres(const QString &newAdres);
    const QString &getIl() const;
    void setIl(const QString &newIl);
    const QString &getIlce() const;
    void setIlce(const QString &newIlce);
    const QString &getMail() const;
    void setMail(const QString &newMail);
    const QDateTime &getTarih() const;
    void setTarih(const QDateTime &newTarih);

    const int &getTip() const;
    void setTip(const int &newTip);

    const QString &getTelefon() const;
    void setTelefon(const QString &newTelefon);

    const QString &getAciklama() const;
    void setAciklama(const QString &newAciklama);

    const QString &getYetkili() const;
    void setYetkili(const QString &newYetkili);

    bool getGuncelBorcHesaplama() const;
    void setGuncelBorcHesaplama(bool newGuncelBorcHesaplama);

private:
    int id;
    QString ad;
    QString yetkili;
    int tip;
    QString verigino;
    QString vergiDaire;
    QString tcNo;
    QString adres;
    QString il;
    QString ilce;
    QString mail;
    QString telefon;
    QDateTime tarih;
    QString aciklama;
    bool guncelBorcHesaplama;

    QLocale turkce;

};

#endif // CARI_H
