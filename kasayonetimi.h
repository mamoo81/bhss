#ifndef KASAYONETIMI_H
#define KASAYONETIMI_H

#include "user.h"

#include <QObject>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>



class KasaYonetimi
{
public:
    KasaYonetimi();
    ~KasaYonetimi();

    enum KasaHareketi
    {
        Giris = 1,
        Cikis = 2,
        Satis = 3,
        Iade = 4,
        BankaVirman = 5
    };

    QSqlDatabase db = QSqlDatabase::database("mhss_data");
    QSqlQuery sorgu = QSqlQuery(db);
    QSqlQueryModel *kasaHareketlerimodel = new QSqlQueryModel();

    double getKasaToplamGiren(QDateTime baslangicTarih, QDateTime bitisTarih);
    double getKasaToplamCikan(QDateTime baslangicTarih, QDateTime bitisTarih);
    QSqlQueryModel *getKasaHareketleri(QDateTime baslangicTarih, QDateTime bitisTarih);
    double getNetKar(QDateTime baslangicTarih, QDateTime bitisTarih);
    int KasaHareketiEkle(User user, KasaHareketi hareket, double tutar, QString aciklama, QDateTime tarih, QString evrakno, double netKar);// kasa forumdan giriş/çıkış/düzeltme için
    int KasaHareketiEkle(User user, KasaHareketi hareket, double tutar, QString aciklama, QDateTime tarih, double netKar);// sepetten satış için overload metod
    int kasaHareketiDuzenle(User user, QString hareketID, KasaHareketi hareket, double tutar, QString aciklama, QDateTime tarih, QString evrakNo);
    bool kasaHareketiSil(QString hareketID, QString hareket, double tutar);
    void kasadanParaCek(double cekilecekTutar, User kullanici);
    double getKasadakiPara();
    double getGunlukCiro();

    QString enumToString(KasaYonetimi::KasaHareketi value);
    KasaYonetimi::KasaHareketi enumFromString(QString value);
};

#endif // KASAYONETIMI_H
