#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

class User
{
public:
    User();


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

private:
    QString userID;
    QString userName;
    QString passWord;
    QString ad;
    QString soyad;
    QString cepNo;
    QDateTime tarih;
    bool kasaYetki;
    bool iadeYetki;
    bool stokYetki;
};

#endif // USER_H
