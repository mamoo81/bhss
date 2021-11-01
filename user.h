#ifndef USER_H
#define USER_H

#include <QString>

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

private:
    QString userID;
    QString userName;
    QString ad;
    QString soyad;
    QString cepNo;
};

#endif // USER_H
