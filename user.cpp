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

