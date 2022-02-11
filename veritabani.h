#ifndef VERITABANI_H
#define VERITABANI_H

#include "user.h"
#include "cari.h"
#include "stokkarti.h"
#include "sepet.h"
//************************
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QList>
#include <QListWidgetItem>

class Veritabani
{
public:
    Veritabani();
    ~Veritabani();

    QSqlDatabase db = QSqlDatabase::database("mhss_data");
    QSqlQuery sorgu = QSqlQuery(db);
    QSqlQueryModel *stokKartlariModel = new QSqlQueryModel();
    QSqlQueryModel *stokHareketleriModel = new QSqlQueryModel();
    QSqlQueryModel *cariKartIsımleriModel = new QSqlQueryModel();
    QSqlQueryModel *cariHareketleriModel = new QSqlQueryModel();
    QSqlQueryModel *kasaHareketlerimodel = new QSqlQueryModel();
    QStringList stokbirimleri;

    bool barkodVarmi(QString _Barkod);
    void baglan();

    QList<QString> GetUsers();
    QList<QString> users;
private:
    Sepet satilacakSepet;

public slots:
    StokKarti getStokKarti(QString _Barkod);
    bool setStokMiktari(User _kullanici, QString _stokKartiID, QString _islem, float _Miktar);
    User GetUserInfos(QString _UserName);
    QList<Cari> getCariKartlar();
    Cari getCariKart(QString _cariID);
    bool loginControl(QString _UserName, QString _Password);
    void satisYap(Sepet _satilacakSepet, User _satisYapanKullanici, int _satisYapilanCariID);
    QStringList getSonIslemler();
    void veritabaniOlustur();
    bool veritabaniVarmi();
    void yeniStokKartiOlustur(StokKarti _StokKarti, User *_Kullanici);
    void stokKartiniGuncelle(const QString _EskiStokKartiID, StokKarti _YeniStokKarti, User *_Kullanici);
    void stokKartiSil(QString _StokKartiID);
    QSqlQueryModel* getStokKartlari();
    QSqlQueryModel* getStokKartlari(QString query);
    QStringList stokGruplariGetir();
    double getKasadakiPara();
    Sepet getSatis(QString _faturaNo);
    void kasadanParaCek(double _cekilecekTutar, User _kullanici);
    double getGunlukCiro();
    void iadeAl(Sepet _iadeSepet, User _kullanici);
    void iadeAl(Sepet _iadeSepet, User _kullanici, Cari _iadeCari);
    QSqlQuery getIslemInfo(QString _faturaNo);
    void CreateNewUser(User _NewUser);
    void updateUser(User _NewUserInfos);
    void deleteUser(QString _DeletedUserName);
    QString sonIslemNumarasi();
    QSqlQueryModel *getCariKartIsimleri();
    void yeniCariKart(Cari _cariKart);
    bool cariKartDuzenle(Cari _cariKart);
    QStringList getCariTipleri();
    QStringList getIller();
    QStringList getIlceler(int _plaka);
    void stokGrupEkle(QString _eklenecekGrupAdi);
    void stokGrupSil(QString _silinecekGrupAdi);
    void stokBirimEkle(QString _birim);
    void stokBirimSil(QString _birim);
    QStringList getVergiDaireleri();
    QSqlQueryModel* getStokHareketleri(QString _barkod);
    QSqlQueryModel *getStokHareketleri(QString _barkod, QDateTime _baslangicTarih, QDateTime _bitisTarih);
    void stokHareketiEkle(User _kullanici, QString _barkod, QString _islem, float _miktar);
    QStringList getStokBirimleri();
    QSqlQueryModel *getCariHareketleri(QString _cariID);
    double getCariToplamAlacak(QString _cariID);
    double getCariToplamBorc(QString _cariID);
    QStringList getOdemeTipleri();
    void caridenTahsilatYap(QString _cariID, double _tutar, QDateTime _tarih, int _faturaTipi, int _odemeTipi, User _islemYapanKullanici, QString _evrakNo, QString _aciklama);
    void cariKartSil(QString _cariID);
    void cariyiAlacaklandır(QString _cariID, double _tutar, QDateTime _tarih, int _faturaTipi, int _odemeTipi, User _islemYapanKullanici, QString _evrakNo, QString _aciklama);
    void cariyeOdemeYap(QString _cariID, double _tutar, QDateTime _tarih, int _faturaTipi, int _odemeTipi, User _islemYapanKullanici, QString _evrakNo, QString _aciklama);
    void cariyiBorclandir(QString _cariID, double _tutar, QDateTime _tarih, int _faturaTipi, int _odemeTipi, User _islemYapanKullanici, QString _evrakNo, QString _aciklama);
    double getKasaToplamGiren(QDateTime _baslangicTarih, QDateTime _bitisTarih);
    double getKasaToplamCikan(QDateTime _baslangicTarih, QDateTime _bitisTarih);
    QSqlQueryModel *getKasaHareketleri(QDateTime _baslangicTarih, QDateTime _bitisTarih);
    double getNetKar(QDateTime _baslangicTarih, QDateTime _bitisTarih);
    int KasaHareketiEkle(User _user, QString _hareket, double _tutar, QString _aciklama, QDateTime _tarih, QString _evrakno, double _netKar);// kasa forumdan giriş/çıkış/düzeltme için
    int KasaHareketiEkle(User _user, QString _hareket, double _tutar, QString _aciklama, QDateTime _tarih, double _netKar);// sepetten satış için overload metod
    int kasaHareketiDuzenle(User _user, QString _hareketID, QString _hareket, double _tutar, QString _aciklama, QDateTime _tarih, QString _evrakNo);
    bool kasaHareketiSil(User _user, QString _hareketID, QString _hareket, double _tutar);
    bool veritabaniYedekle(QString _dirNameAndFileName);
    bool veritabaniYedektenGeriYukle(QString _dosyaYolu);
    bool veritabaniSifirla();
};

#endif // VERITABANI_H
