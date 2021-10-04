#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();
    void setUserInfos(QString p_userID, QString p_userName, QString p_adi, QString p_cepNo);
    QString getUserID();
    QString getUserName();
    QString getAdi();
    QString getCepNo();
private:
    QString userID,userName,adi,cepNo;
};

#endif // USER_H
