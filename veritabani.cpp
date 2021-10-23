#include "veritabani.h"
#include "user.h"
#include "stokkarti.h"
//**************************
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVariant>

Veritabani::Veritabani()
{
    db.setDatabaseName("mhss_data");
    db.setHostName("localhost");
    db.setUserName("admin");
    db.setPassword("admin");
    if(!db.isDriverAvailable("QPSQL")){
        QMessageBox::information(0, "Hata", "SQL servisini kontrol edin.", QMessageBox::Ok);
    }
    baglan();
}

Veritabani::~Veritabani()
{

}

bool Veritabani::barkodVarmi(QString _Barkod)
{
    sorgu.prepare("SELECT barkod FROM stokkartlari WHERE barkod = ?");
    sorgu.bindValue(0, _Barkod);
    sorgu.exec();
    if(sorgu.next()){
        return true;
    }
    else{
        return false;
    }
}

void Veritabani::baglan()
{
    if(!db.isOpen()){
        db.open();
    }
}

bool Veritabani::loginControl(QString _UserName, QString _Password)
{
    sorgu.prepare("SELECT kulusername, kulpassword FROM kullanicilar WHERE kulusername = ?");
    sorgu.bindValue(0, _UserName);
    sorgu.exec();
    if(sorgu.next()){
        if(sorgu.value(0) == _Password){
            return true;
        }
        else{
            QMessageBox::warning(0, "Uyarı", "Şifre hatalı", QMessageBox::Ok);
            return false;
        }
    }
    else{
        QMessageBox::warning(0, "Uyarı", "Böyle bir kullanıcı yok", QMessageBox::Ok);
        return false;
    }
}
/**
 * @brief Veritabani::satisYap sepet satış işlemlerini yapar db kaydeder.
 * @param _SatilacakSepet Satılacak sepet parametresi
 * @param _Kullanici Satışı yapan kullanıcı parametresi
 * @param _SatisYapilacakCari Satış yapılacak cari id parametresi
 */
void Veritabani::satisYap(Sepet _satilacakSepet, QString _satisYapanKullanici, QString _satisYapilanCari)
{
    sorgu.exec("SELECT nextval('faturalar_sequence')");
    sorgu.next();
    QString yeniFaturaNo = QDate::currentDate().toString("ddMMyy") + sorgu.value(0).toString();
    qDebug() << yeniFaturaNo;
//    sorgu.prepare("UPDATE stokkartlari SET miktar WHERE barkod = ?")
}

User Veritabani::GetUserInfos(QString _UserName)
{
    User u;
    sorgu.prepare("SELECT * FROM kullanicilar WHERE kulusername = ?");
    sorgu.bindValue(0, _UserName);
    sorgu.exec();
    if(sorgu.next()){
        u.setUserInfos(sorgu.value(0).toString(), sorgu.value(1).toString(), sorgu.value(3).toString(), sorgu.value(5).toString());
    }
    return u;
}

QList<Cari> Veritabani::getCariKartlar()
{
    QList<Cari> kartlar;
    sorgu.exec("SELECT * FROM carikartlar");
    while (sorgu.next()) {
        Cari kart;
        kart.setId(sorgu.value(0).toInt());
        kart.setAd(sorgu.value(1).toString());
        kart.setVerigino(sorgu.value(2).toString());
        kart.setVergiDaire(sorgu.value(3).toString());
        kart.setTcNo(sorgu.value(4).toString());
        kart.setAdresNo(sorgu.value(5).toString());
        kart.setAdres(sorgu.value(6).toString());
        kart.setIl(sorgu.value(7).toString());
        kart.setIlce(sorgu.value(8).toString());
        kart.setMail(sorgu.value(9).toString());
        kart.setCep(sorgu.value(10).toString());
        kart.setTarih(sorgu.value(11).toDateTime());
        kartlar.append(kart);
    }
    return kartlar;
}

QList<QString> Veritabani::GetUsers()
{
    sorgu.exec("SELECT kulusername FROM kullanicilar");
    while (sorgu.next()) {
        users << sorgu.value(0).toString();
    }
    return users;
}

StokKarti Veritabani::getStokKarti(QString _Barkod)
{
    sorgu.prepare("SELECT stokid, barkod, ad, birim, miktar, stokgrup, afiyat, sfiyat, kdv1, kdv2, kdv3, songuntarih, aciklama FROM stokkartlari WHERE barkod = ?");
    sorgu.bindValue(0, _Barkod);
    sorgu.exec();
    if(sorgu.next()){
        StokKarti kart;
        kart.setID(sorgu.value(0).toString());
        kart.setBarkod(sorgu.value(1).toString());
        kart.setAd(sorgu.value(2).toString());
        kart.setBirim(sorgu.value(3).toString());
        kart.setMiktar(sorgu.value(4).toFloat());
        kart.setGrup(sorgu.value(5).toString());
        kart.setAFiyat(sorgu.value(6).toDouble());
        kart.setSFiyat(sorgu.value(7).toDouble());
        kart.setKDV1(sorgu.value(8).toInt());
        kart.setKDV2(sorgu.value(9).toInt());
        kart.setKDV3(sorgu.value(10).toInt());
        kart.setSonGunTarih(sorgu.value(11).toDateTime());
        kart.setAciklama(sorgu.value(12).toString());
        return kart;
    }
}

