#include "veritabani.h"
#include "user.h"
//**************************
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVariant>

Veritabani::Veritabani()
{
    db.setDatabaseName("mhss_data");
    db.setHostName("localhost");
    db.setUserName("admin");
    db.setPassword("admin");
    if(!db.isDriverAvailable("QPSQL")){
        QMessageBox::information(0, "Hata", "SQL servisini kontrol edin.", QMessageBox::Ok);
    }
    baglan();
}

bool Veritabani::barkodVarmi(QString _Barkod)
{
    sorgu.prepare("SELECT barkod FROM stokkartlari WHERE barkod = ?");
    sorgu.bindValue(0, _Barkod);
    sorgu.exec();
    if(sorgu.next()){
        return true;
    }
    else{
        return false;
    }
}

void Veritabani::baglan()
{
    if(!db.isOpen()){
        db.open();
    }
}

bool Veritabani::loginControl(QString _UserName, QString _Password)
{
    sorgu.prepare("SELECT kulusername, kulpassword FROM kullanicilar WHERE kulusername = ?");
    sorgu.bindValue(0, _UserName);
    sorgu.exec();
    if(sorgu.next()){
        if(sorgu.value(0) == _Password){
            return true;
        }
        else{
            QMessageBox::warning(0, "Uyarı", "Şifre hatalı", QMessageBox::Ok);
            return false;
        }
    }
    else{
        QMessageBox::warning(0, "Uyarı", "Böyle bir kullanıcı yok", QMessageBox::Ok);
        return false;
    }
}

User Veritabani::GetUserInfos(QString _UserName)
{
    User u;
    sorgu = QSqlQuery(db);
    sorgu.prepare("SELECT kulid, kulusername, kuladi, kulcep FROM kullanicilar WHERE kulusername = ?");
    sorgu.bindValue(0, _UserName);
    sorgu.exec();
    if(sorgu.next()){
        u.setUserInfos(sorgu.value(0).toString(), sorgu.value(1).toString(), sorgu.value(2).toString(), sorgu.value(3).toString());
    }
    qDebug() << sorgu.lastError();
    return u;
}

QList<QString> Veritabani::GetUsers()
{
    sorgu.exec("SELECT kulusername FROM kullanicilar");
    while (sorgu.next()) {
        users << sorgu.value(0).toString();
    }
    return users;
}

