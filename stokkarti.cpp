#include "stokkarti.h"

StokKarti::StokKarti()
{

}

const QString &StokKarti::getId() const
{
    return id;
}

void StokKarti::setId(const QString &newId)
{
    id = newId;
}

const QString &StokKarti::getBarkod() const
{
    return barkod;
}

void StokKarti::setBarkod(const QString &newBarkod)
{
    barkod = newBarkod;
}

const QString &StokKarti::getAd() const
{
    return ad;
}

void StokKarti::setAd(const QString &newAd)
{
    ad = newAd;
}

const QString &StokKarti::getBirim() const
{
    return birim;
}

void StokKarti::setBirim(const QString &newBirim)
{
    birim = newBirim;
}

float StokKarti::getMiktar() const
{
    return miktar;
}

void StokKarti::setMiktar(float newMiktar)
{
    miktar = newMiktar;
}

const QString &StokKarti::getGrup() const
{
    return grup;
}

void StokKarti::setGrup(const QString &newGrup)
{
    grup = newGrup;
}

double StokKarti::getAFiyat() const
{
    return aFiyat;
}

void StokKarti::setAFiyat(double newAFiyat)
{
    aFiyat = newAFiyat;
}

double StokKarti::getSFiyat() const
{
    return sFiyat;
}

void StokKarti::setSFiyat(double newSFiyat)
{
    sFiyat = newSFiyat;
}

int StokKarti::getKdv() const
{
    return kdv;
}

void StokKarti::setKdv(int newKdv)
{
    kdv = newKdv;
}

const QDateTime &StokKarti::getTarih() const
{
    return tarih;
}

void StokKarti::setTarih(const QDateTime &newTarih)
{
    tarih = newTarih;
}

const QString &StokKarti::getAciklama() const
{
    return aciklama;
}

void StokKarti::setAciklama(const QString &newAciklama)
{
    aciklama = newAciklama;
}
