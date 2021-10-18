#ifndef VERITABANI_H
#define VERITABANI_H

#include <QSqlDatabase>
#include <QSqlQuery>

class Veritabani
{
public:
    Veritabani();
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mhss");

};

#endif // VERITABANI_H
