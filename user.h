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
#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

class User
{
public:
    User();
    User(QDateTime);

    const QString &getUserID() const;
    void setUserID(const QString &newUserID);

    const QString &getUserName() const;
    void setUserName(const QString &newUserName);

    const QString &getAd() const;
    void setAd(const QString &newAd);

    const QString &getSoyad() const;
    void setSoyad(const QString &newSoyad);

    const QString &getCepNo() const;
    void setCepNo(const QString &newCepNo);

    const QDateTime &getTarih() const;
    void setTarih(const QDateTime &newTarih);

    bool getKasaYetki() const;
    void setKasaYetki(bool newKasaYetki);

    bool getIadeYetki() const;
    void setIadeYetki(bool newIadeYetki);

    bool getStokYetki() const;
    void setStokYetki(bool newStokYetki);

    const QString &getPassWord() const;
    void setPassWord(const QString &newPassWord);

    bool getCariyetki() const;
    void setCariyetki(bool newCariyetki);

    bool getAyaryetki() const;
    void setAyaryetki(bool newAyaryetki);

    const QDateTime &getGirisTarih() const;
    void setGirisTarih(const QDateTime &newGirisTarih);

private:
    QString userID;
    QString userName;
    QString passWord;
    QString ad;
    QString soyad;
    QString cepNo;
    QDateTime tarih;
    QDateTime _girisTarih;
    bool kasaYetki;
    bool iadeYetki;
    bool stokYetki;
    bool cariyetki;
    bool ayaryetki;
};

#endif // USER_H
