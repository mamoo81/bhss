#ifndef CARIYONETIMI_H
#define CARIYONETIMI_H

#include "cari.h"
#include "stokyonetimi.h"
#include "faturayonetimi.h"
#include "kasayonetimi.h"
#include "cari.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

class CariYonetimi
{
public:
    CariYonetimi();

    QSqlDatabase db = QSqlDatabase::database("mhss_data");
    QSqlQuery sorgu = QSqlQuery(db);
    FaturaYonetimi faturaYonetimi = FaturaYonetimi();
    StokYonetimi stokYonetimi = StokYonetimi();
    KasaYonetimi kasaYonetimi = KasaYonetimi();
    QSqlQueryModel *cariKartIsımleriModel = new QSqlQueryModel();
    QSqlQueryModel *cariHareketleriModel = new QSqlQueryModel();

    Cari getCariKart(QString cariID);
    QList<Cari> getCariKartlar();
    QSqlQueryModel *getCariKartIsimleri();
    bool yeniCariKart(Cari cariKart);
    bool cariKartDuzenle(Cari cariKart);
    QSqlQueryModel *getCariHareketleri(QString cariID);
    QStringList getCariTipleri();
    QStringList getVergiDaireleri();
    double getCariToplamAlacak(QString cariID);
    double getCariToplamBorc(QString cariID, bool guncel);
    double getCarilerToplamAlacak();
    double getcarilerToplamBorc(bool guncel, QDateTime startDate, QDateTime endDate);
    bool cariHareketiSil(QString faturaNo, User kullanici, Cari cari);
    void caridenTahsilatYap(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama);
    void cariKartSil(QString cariID);
    void cariyiAlacaklandır(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama);
    void cariyeOdemeYap(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama);
    void cariyiBorclandir(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama);
};

#endif // CARIYONETIMI_H
