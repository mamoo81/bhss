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
#include "cari.h"

Cari::Cari()
{

}

const QString &Cari::getAd() const
{
    return ad;
}

void Cari::setAd(const QString &newAd)
{
    ad = newAd;
}

int Cari::getId() const
{
    return id;
}

void Cari::setId(int newId)
{
    id = newId;
}

const QString &Cari::getVerigino() const
{
    return verigino;
}

void Cari::setVerigino(const QString &newVerigino)
{
    verigino = newVerigino;
}

const QString &Cari::getVergiDaire() const
{
    return vergiDaire;
}

void Cari::setVergiDaire(const QString &newVergiDaire)
{
    vergiDaire = newVergiDaire;
}

const QString &Cari::getTcNo() const
{
    return tcNo;
}

void Cari::setTcNo(const QString &newTcNo)
{
    tcNo = newTcNo;
}

const QString &Cari::getAdres() const
{
    return adres;
}

void Cari::setAdres(const QString &newAdres)
{
    adres = newAdres;
}

const QString &Cari::getIl() const
{
    return il;
}

void Cari::setIl(const QString &newIl)
{
    il = newIl;
}

const QString &Cari::getIlce() const
{
    return ilce;
}

void Cari::setIlce(const QString &newIlce)
{
    ilce = newIlce;
}

const QString &Cari::getMail() const
{
    return mail;
}

void Cari::setMail(const QString &newMail)
{
    mail = newMail;
}

const QDateTime &Cari::getTarih() const
{
    return tarih;
}

void Cari::setTarih(const QDateTime &newTarih)
{
    tarih = newTarih;
}

const int &Cari::getTip() const
{
    return tip;
}

void Cari::setTip(const int &newTip)
{
    tip = newTip;
}

const QString &Cari::getTelefon() const
{
    return telefon;
}

void Cari::setTelefon(const QString &newTelefon)
{
    telefon = newTelefon;
}

const QString &Cari::getAciklama() const
{
    return aciklama;
}

void Cari::setAciklama(const QString &newAciklama)
{
    aciklama = newAciklama;
}

const QString &Cari::getYetkili() const
{
    return yetkili;
}

void Cari::setYetkili(const QString &newYetkili)
{
    yetkili = newYetkili;
}
