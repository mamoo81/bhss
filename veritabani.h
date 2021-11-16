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

    bool barkodVarmi(QString _Barkod);
    void baglan();

    QList<QString> GetUsers();
    QList<QString> users;
private:
    Sepet satilacakSepet;

public slots:
    StokKarti getStokKarti(QString _Barkod);
    User GetUserInfos(QString _UserName);
    QList<Cari> getCariKartlar();
    Cari getCariKart(QString _cariID);
    bool loginControl(QString _UserName, QString _Password);
    void satisYap(Sepet _satilacakSepet, User _satisYapanKullanici, int _satisYapilanCariID);
    QStringList getSonIslemler();
    void veritabaniOlustur();
    bool veritabaniVarmi();
    void yeniStokKartiOlustur(StokKarti *_StokKarti, User *_Kullanici);
    void stokKartiniGuncelle(const QString _EskiStokKartiID, StokKarti *_YeniStokKarti, User *_Kullanici);
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
};

#endif // VERITABANI_H
