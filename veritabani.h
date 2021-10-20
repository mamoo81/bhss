#ifndef VERITABANI_H
#define VERITABANI_H

#include "user.h"
#include "stokkarti.h"

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

    QList<QString> GetUsers();
    QList<QString> users;
private:

public slots:
    StokKarti getStokKarti(QString _Barkod);
    User GetUserInfos(QString _UserName);
    bool loginControl(QString _UserName, QString _Password);
};

#endif // VERITABANI_H
