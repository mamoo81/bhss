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


