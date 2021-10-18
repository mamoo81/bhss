#ifndef VERITABANI_H
#define VERITABANI_H

#include "user.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>

class Veritabani
{
public:
    Veritabani();
    ~Veritabani();
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mhss");
    QSqlQuery sorgu = QSqlQuery(db);
    bool barkodVarmi(QString _Barkod);
    void baglan();
    User GetUserInfos(QString _UserName);
    QList<QString> GetUsers();
    QList<QString> users;
private:

public slots:
    bool loginControl(QString _UserName, QString _Password);
};

#endif // VERITABANI_H
