#ifndef FATURAYONETIMI_H
#define FATURAYONETIMI_H
#include "sepet.h"
#include "stokkarti.h"
#include "stokyonetimi.h"
#include "kasayonetimi.h"
#include "cari.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

class FaturaYonetimi
{
public:
    FaturaYonetimi();
    ~FaturaYonetimi();

    QSqlDatabase db = QSqlDatabase::database("mhss_data");
    QSqlQuery sorgu = QSqlQuery(db);
    StokYonetimi stokYonetimi = StokYonetimi();
    KasaYonetimi kasaYonetimi = KasaYonetimi();

    void satisYap(Sepet satilacakSepet, User satisYapanKullanici, int satisYapilanCariID);

    Sepet getSatis(QString _faturaNo);
    Sepet getSatis(QString _faturaNo, Cari cari);
    QSqlQuery getIslemInfo(QString _faturaNo);
    void iadeAl(Sepet iadeSepet, User kullanici);
    void iadeAl(Sepet iadeSepet, User kullanici, Cari iadeCari, QString faturaNo);
    bool iadeAlinmismi(QString FaturaNo);
    bool faturayiSil(QString faturaNo);
    QString yeniFaturaNo();
    QString sonIslemNumarasi();
};

#endif // FATURAYONETIMI_H
