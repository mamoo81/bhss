#include "loginform.h"
//***********************
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mhss_data");
    db.setHostName("localhost");
    db.setUserName("postgres");
    db.setPassword("postgres");
    if(!db.open())
        qFatal("veritabanı bağlantı hatası: %s",
            qPrintable(db.lastError().text()));
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "mhss_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    LoginForm w;
    w.show();
    return a.exec();
}
