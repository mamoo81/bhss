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
