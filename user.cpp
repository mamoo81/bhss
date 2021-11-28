#include "user.h"

User::User()
{

}

const QString &User::getUserID() const
{
    return userID;
}

void User::setUserID(const QString &newUserID)
{
    userID = newUserID;
}

const QString &User::getUserName() const
{
    return userName;
}

void User::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

const QString &User::getAd() const
{
    return ad;
}

void User::setAd(const QString &newAd)
{
    ad = newAd;
}

const QString &User::getSoyad() const
{
    return soyad;
}

void User::setSoyad(const QString &newSoyad)
{
    soyad = newSoyad;
}

const QString &User::getCepNo() const
{
    return cepNo;
}

void User::setCepNo(const QString &newCepNo)
{
    cepNo = newCepNo;
}

const QDateTime &User::getTarih() const
{
    return tarih;
}

void User::setTarih(const QDateTime &newTarih)
{
    tarih = newTarih;
}

bool User::getKasaYetki() const
{
    return kasaYetki;
}

void User::setKasaYetki(bool newKasaYetki)
{
    kasaYetki = newKasaYetki;
}

bool User::getIadeYetki() const
{
    return iadeYetki;
}

void User::setIadeYetki(bool newIadeYetki)
{
    iadeYetki = newIadeYetki;
}

bool User::getStokYetki() const
{
    return stokYetki;
}

void User::setStokYetki(bool newStokYetki)
{
    stokYetki = newStokYetki;
}

const QString &User::getPassWord() const
{
    return passWord;
}

void User::setPassWord(const QString &newPassWord)
{
    passWord = newPassWord;
}

