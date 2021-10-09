#include "stokkarti.h"

StokKarti::StokKarti()
{

}

void StokKarti::setID(QString p_ID)
{
    id = p_ID;
}

QString StokKarti::getID()
{
    if(!id.isEmpty() || !id.isNull()){
        return id;
    }
    else{
        return "";
    }
}

void StokKarti::setBarkod(QString p_Barkod)
{
    barkod = p_Barkod;
}

QString StokKarti::getBarkod()
{
    if(!barkod.isEmpty() || !barkod.isNull()){
        return barkod;
    }
    else{
        return "";
    }
}

void StokKarti::setAd(QString p_Ad)
{
    ad = p_Ad;
}

QString StokKarti::getAd()
{
    if(!ad.isEmpty() || !ad.isNull()){
        return ad;
    }
    else{
        return "";
    }
}

void StokKarti::setBirim(QString p_Birim)
{
    birim = p_Birim;
}

QString StokKarti::getBirim()
{
    if(!birim.isEmpty() || !birim.isNull()){
        return birim;
    }
    else{
        return "";
    }
}

void StokKarti::setMiktar(QString p_Miktar)
{
    miktar = p_Miktar;
}

QString StokKarti::getMiktar()
{
    if(!miktar.isEmpty() || !miktar.isNull()){
        return miktar;
    }
    else{
        return "";
    }
}

void StokKarti::setGrup(QString p_Grup)
{
    grup = p_Grup;
}

QString StokKarti::getGrup()
{
    if(!grup.isEmpty() || !grup.isNull()){
        return grup;
    }
    else{
        return "";
    }
}

void StokKarti::setAFiyat(QString p_AFiyat)
{
    aFiyat = p_AFiyat;
}

QString StokKarti::getAfiyat() const
{
    return aFiyat;
}

void StokKarti::setSFiyat(QString p_SFiyat)
{
    sFiyat = p_SFiyat;
}

QString StokKarti::getSFiyat() const
{
    return sFiyat;
}

void StokKarti::setKDV1(int p_KDV1)
{
    kdv1 = p_KDV1;
}

int StokKarti::getKDV1()
{
    return kdv1;
}

void StokKarti::setKDV2(int p_KDV2)
{
    kdv2 = p_KDV2;
}

int StokKarti::getKDV2()
{
    return kdv2;
}

void StokKarti::setKDV3(int p_KDV3)
{
    kdv3 = p_KDV3;
}

int StokKarti::getKDV3()
{
    return kdv3;
}

void StokKarti::setSonGunTarih(QDateTime p_SonGunTarih)
{
    sonGunTarih = p_SonGunTarih;
}

QDateTime StokKarti::getSonGunTarih()
{
    if(!sonGunTarih.isNull()){
        return sonGunTarih;
    }
    else{
        return QDateTime::currentDateTime();
    }
}

void StokKarti::setAciklama(QString p_Aciklama)
{
    aciklama = p_Aciklama;
}

QString StokKarti::getAciklama()
{
    if(!aciklama.isEmpty() || !aciklama.isNull()){
        return aciklama;
    }
    else{
        return "";
    }
}
