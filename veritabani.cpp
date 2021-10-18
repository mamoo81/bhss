#include "veritabani.h"
//**************************
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

Veritabani::Veritabani()
{
    db.setDatabaseName("mhss_data");
    db.setHostName("localhost");
    db.setUserName("admin");
    db.setPassword("admin");
    if(!db.isDriverAvailable("QPSQL")){
        QMessageBox::information(0, "Hata", "SQL servisini kontrol edin.", QMessageBox::Ok);
    }
}

