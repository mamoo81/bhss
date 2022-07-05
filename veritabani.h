/*####################################
MIT LICENCE                          #
######################################
Copyright 2021 Mehmet AKDEMİR        #
bilgi@basat.dev                      #
######################################
Permission is hereby granted, free of charge,
to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
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

    QStringList GetUsers();
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
    QString yeniFaturaNo();
    bool faturayiSil(QString _faturaNo);
    QStringList getSonIslemler();
    bool veritabaniVarmi();
    QSqlError yeniStokKartiOlustur(StokKarti _StokKarti, User *_Kullanici);
    void stokKartiniGuncelle(const QString _EskiStokKartiID, StokKarti _YeniStokKarti, User *_Kullanici);
    void stokKartiSil(QString _StokKartiID);
    QSqlQueryModel* getStokKartlari();
    QSqlQueryModel* getStokKartlari(QString query);
    QSqlQueryModel* getStokKartlariEtiket();
    QStringList stokGruplariGetir();
    int getGrupID(QString pGrup);
    double getKasadakiPara();
    Sepet getSatis(QString _faturaNo);
    void kasadanParaCek(double _cekilecekTutar, User _kullanici);
    double getGunlukCiro();
    void iadeAl(Sepet _iadeSepet, User _kullanici);
    void iadeAl(Sepet _iadeSepet, User _kullanici, Cari _iadeCari, QString _faturaNo);
    bool iadeAlinmismi(QString FaturaNo);
    QSqlQuery getIslemInfo(QString _faturaNo);
    bool CreateNewUser(User _NewUser);
    void updateUser(User _NewUserInfos);
    void deleteUser(QString _DeletedUserName);
    QString sonIslemNumarasi();
    QSqlQueryModel *getCariKartIsimleri();
    bool yeniCariKart(Cari _cariKart);
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
    int getBirimID(QString pBirim);
    QString getBirimAd(int birimID);
    QSqlQueryModel *getCariHareketleri(QString _cariID);
    double getCariToplamAlacak(QString _cariID);
    double getCariToplamBorc(QString _cariID);
    double getCarilerToplamAlacak();
    double getcarilerToplamBorc();
    bool cariHareketiSil(QString _faturaNo, User _kullanici, Cari _cari);
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
    bool kasaHareketiSil(QString _hareketID, QString _hareket, double _tutar);
    bool veritabaniYedekle(QString _dirNameAndFileName);
    bool veritabaniYedektenGeriYukle(QString _dosyaYolu);
    bool veritabaniSifirla();
    void setHizliButon(StokKarti _stokKarti);
    void setOturum(User _user);
    QSqlQuery getOturum();
    void oturumSonlandir();
    QStringList getUreticiler();
    int getUreticiID(QString _ureticiAd);
    QStringList getTedarikciler();
    int getTedarikciID(QString _tedarikciAd);
    QStringList getTeraziler();
    QStringList getTeraziModeller(QString Marka);
    QHash<QString, float> getgunlukAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart);
    QHash<QString, float> getAylikAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart);
    QHash<QString, float> getYillikAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart);
};

#endif // VERITABANI_H
