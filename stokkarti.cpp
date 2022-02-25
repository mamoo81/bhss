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
    QFile resimDosya(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/mhss/urunler-image/" + barkod + ".jpg");
    if(resimDosya.exists()){
        QImage img(QFileInfo(resimDosya).absoluteFilePath());
        setResim(QPixmap::fromImage(img));
    }
    else{
        QImage img(":/images/ui/box.png");
        setResim(QPixmap::fromImage(img));
    }
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

const QString &StokKarti::getKod() const
{
    return kod;
}

void StokKarti::setKod(const QString &newKod)
{
    kod = newKod;
}

int StokKarti::getOtv() const
{
    return otv;
}

void StokKarti::setOtv(int newOtv)
{
    otv = newOtv;
}

bool StokKarti::getKdvdahil() const
{
    return kdvdahil;
}

void StokKarti::setKdvdahil(bool newKdvdahil)
{
    kdvdahil = newKdvdahil;
}

bool StokKarti::getOtvdahil() const
{
    return otvdahil;
}

void StokKarti::setOtvdahil(bool newOtvdahil)
{
    otvdahil = newOtvdahil;
}

int StokKarti::getUretici() const
{
    return uretici;
}

void StokKarti::setUretici(int newUretici)
{
    uretici = newUretici;
}

int StokKarti::getTedarikci() const
{
    return tedarikci;
}

void StokKarti::setTedarikci(int newTedarikci)
{
    tedarikci = newTedarikci;
}

const QPixmap &StokKarti::getResim() const
{
    return resim;
}

void StokKarti::setResim(const QPixmap &newResim)
{
    resim = newResim;
}
