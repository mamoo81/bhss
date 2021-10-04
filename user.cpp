#include "user.h"

User::User()
{

}

void User::setUserInfos(QString p_userID, QString p_userName, QString p_adi, QString p_cepNo)
{
    userID = p_userID;
    userName = p_userName;
    adi = p_adi;
    cepNo = p_cepNo;
}

QString User::getUserID()
{
    if(!userID.isEmpty() || !userID.isNull()){
        return userID;
    }
    else{
        return "";
    }
}

QString User::getUserName()
{
    if(!userName.isEmpty() || !userName.isNull()){
        return userName;
    }
    else {
        return "";
    }
}

QString User::getAdi()
{
    if(!adi.isEmpty() || !adi.isNull()){
        return adi;
    }
    else{
        return "";
    }
}

QString User::getCepNo()
{
    if(!cepNo.isEmpty() || !cepNo.isNull()){
        return cepNo;
    }
    else{
        return "";
    }
}
