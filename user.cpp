#include "user.h"

User::User()
{
    setGirisTarih(QDateTime::currentDateTime());
}

User::User(QDateTime _tarih)
{
    setGirisTarih(_tarih);
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

bool User::getCariyetki() const
{
    return cariyetki;
}

void User::setCariyetki(bool newCariyetki)
{
    cariyetki = newCariyetki;
}

bool User::getAyaryetki() const
{
    return ayaryetki;
}

void User::setAyaryetki(bool newAyaryetki)
{
    ayaryetki = newAyaryetki;
}

const QDateTime &User::getGirisTarih() const
{
    return _girisTarih;
}

void User::setGirisTarih(const QDateTime &newGirisTarih)
{
    _girisTarih = newGirisTarih;
}


