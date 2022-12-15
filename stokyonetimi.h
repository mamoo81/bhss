#ifndef STOKYONETIMI_H
#define STOKYONETIMI_H

#include "stokkarti.h"
#include "user.h"
#include "stokkarti.h"
//******************************
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QSqlQueryModel>

class StokYonetimi
{

public:
    StokYonetimi();
    ~StokYonetimi();

    enum StokHareketi
    {
        Giris = 0,
        Cikis = 1,
        Satis = 2,
        Iade = 3
    };

    QSqlDatabase db = QSqlDatabase::database("mhss_data");
    QSqlQuery sorgu = QSqlQuery(db);
    QSqlQueryModel *stokKartlariModel = new QSqlQueryModel();
    QSqlQueryModel *stokHareketleriModel = new QSqlQueryModel();

    bool barkodVarmi(const QString barkod);
    StokKarti getStokKarti(QString barkod);
    bool setStokMiktari(const User kullanici, const StokKarti kart, StokHareketi islem, float miktar);
    QSqlError yeniStokKartiOlustur(StokKarti stokKarti, User *kullanici);
    QSqlError stokKartiniGuncelle(StokKarti duzenlenecekStokKarti, User *kullanici);
//    void veresiyeTutarlariGuncelle(QString barkod);
    bool stokKartiSil(QString stokKartiID);
    void stokHareketiEkle(User kullanici, QString barkod, StokHareketi hareket, float miktar);
    QSqlQueryModel *getStokKartlari();
    QSqlQueryModel *getStokKartlari(QString grupAdi);
    QSqlQueryModel *getStokKartlari(QSqlQuery query);
    QSqlQueryModel *getStokKartlariEtiket();
    QStringList stokGruplariGetir();
    int getGrupID(QString pGrup);
    QSqlQueryModel *getStokHareketleri(QString barkod);
    QSqlQueryModel *getStokHareketleri(QString barkod, QDateTime baslangicTarih, QDateTime bitisTarih);
    void stokGrupEkle(QString eklenecekGrupAdi);
    void stokGrupSil(QString silinecekGrupAdi);
    void stokBirimEkle(QString birim);
    void stokBirimSil(QString birim);
    QStringList getStokBirimleri();
    int getBirimID(QString pBirim);
    QString getBirimAd(int birimID);
    QHash<QString, float> getgunlukAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart);
    QHash<QString, float> getAylikAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart);
    QHash<QString, float> getYillikAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart);
    QStringList getUreticiler();
    int getUreticiID(QString ureticiAd);
    QString getUreticiAD(int ID);
    QStringList getTedarikciler();
    int getTedarikciID(QString tedarikciAd);
    bool csvAktar(QString dosyaYolu);

};

#endif // STOKYONETIMI_H
