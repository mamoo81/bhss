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
#include <QDateTime>

Veritabani::Veritabani()
{
    db.setHostName("localhost");
    db.setUserName("postgres");
    db.setPassword("postgres");
}

Veritabani::~Veritabani()
{

}

bool Veritabani::barkodVarmi(QString _Barkod)
{
    db.setDatabaseName("mhss_data");
    db.open();
    sorgu.prepare("SELECT barkod FROM stokkartlari WHERE barkod = ?");
    sorgu.bindValue(0, _Barkod);
    sorgu.exec();
    if(sorgu.next()){
        db.close();
        return true;
    }
    else{
        return false;
    }
}

bool Veritabani::loginControl(QString _UserName, QString _Password)
{
    db.setDatabaseName("mhss_data");
    db.open();
    sorgu.prepare("SELECT username, password FROM kullanicilar WHERE username = ?");
    sorgu.bindValue(0, _UserName);
    sorgu.exec();
    if(sorgu.next()){
        if(sorgu.value(0) == _Password){
            db.close();
            return true;
        }
        else{
            db.close();
            QMessageBox::warning(0, "Uyarı", "Şifre hatalı", QMessageBox::Ok);
            return false;
        }
    }
    else{
        db.close();
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
void Veritabani::satisYap(Sepet _satilacakSepet, User _satisYapanKullanici, int _satisYapilanCariID)
{
    db.setDatabaseName("mhss_data");
    //yeni fatura numarası için faturalar_sequence'den son değeri alma
    db.open();
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    sorgu.next();
    QString yeniFaturaNo = QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toInt() + 1);
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar (id, fatura_no, cari, tipi, fatura_tarih, kullanici) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, yeniFaturaNo);
    sorgu.bindValue(1, _satisYapilanCariID);
    sorgu.bindValue(2, "SATIŞ");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, _satisYapanKullanici.getUserID());
    sorgu.exec();
    //sepetteki ürünlerin stok hareketlerine girişi
    foreach (auto urun, _satilacakSepet.urunler) {
        sorgu.prepare("INSERT INTO stokhareketleri (barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, urun.barkod);
        sorgu.bindValue(1, yeniFaturaNo);
        sorgu.bindValue(2, "SATIŞ");
        sorgu.bindValue(3, urun.miktar);
        sorgu.bindValue(4, QDateTime::currentDateTime());
        sorgu.bindValue(5, _satisYapanKullanici.getUserID());
        sorgu.bindValue(6, "SATIŞ");
        sorgu.exec();
    }
    //sepetteki ürünlerin stoklardan düşülmesi
    foreach (auto urun, _satilacakSepet.urunler) {
        sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE barkod = ?");
        sorgu.bindValue(0, urun.stokMiktari - urun.miktar);
        sorgu.bindValue(1, urun.barkod);
        sorgu.exec();
    }
    db.close();
}

QStringList Veritabani::getSonIslemler()
{
    db.setDatabaseName("mhss_data");
    db.open();
    QStringList islemler;
    sorgu.exec("SELECT fatura_no, fatura_tarih FROM faturalar WHERE fatura_tarih::date = now()::date ORDER BY fatura_no DESC");
    while(sorgu.next()){
        islemler.append(sorgu.value(0).toString() + " " + sorgu.value(1).toTime().toString("hh:mm:ss"));
    }
    db.close();
    return islemler;
}

bool Veritabani::veritabaniVarmi()
{
    db.open();
    //mhss_data veritabanı varmı kontrol
    sorgu.exec("SELECT datname FROM pg_database WHERE datname = 'mhss_data'");
    if(!sorgu.next()){
        QMessageBox msg(0);
        msg.setText("Uyarı");
        msg.setInformativeText("Veritabanı bulunamadı.\n\nSıfırdan oluşturmak ister misiniz?");
        msg.setIcon(QMessageBox::Question);
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        int cevap = msg.exec();
        switch (cevap) {
        case QMessageBox::Yes:
            veritabaniOlustur();
            break;
        case QMessageBox::No:
            return false;
            break;
        }
    }
    db.close();
    return true;
}

void Veritabani::veritabaniOlustur()
{
    db.open();
    //veritabanını oluşturma
    sorgu.exec("CREATE DATABASE mhss_data OWNER postgres");
    db.close();
    db.setDatabaseName("mhss_data");
    db.open();
    //kullanıcılar tablosunu oluşturma
    sorgu.exec("CREATE TABLE kullanicilar("
                "id BIGSERIAL PRIMARY KEY,"
                "username VARCHAR(32) NOT NULL,"
                "password VARCHAR(32) NOT NULL,"
                "ad TEXT,"
                "soyad TEXT,"
                "cepno VARCHAR(10),"
                "tarih TIMESTAMP NOT NULL)");
    sorgu.exec("CREATE SEQUENCE kullanicilar_sequence START WITH 100 INCREMENT BY 1 OWNED BY kullanicilar.id");
    //kullanıcılar tablosuna admin kullanıcısını ekleme
    sorgu.prepare("INSERT INTO kullanicilar(id, username, password, ad, soyad, cepno, tarih) "
                    "VALUES (nextval('kullanicilar_sequence'), ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, "admin");
    sorgu.bindValue(1, "admin");
    sorgu.bindValue(2, "ADMİN");
    sorgu.bindValue(3, "ADMİN");
    sorgu.bindValue(4, "0000000000");
    sorgu.bindValue(5, QDate::currentDate());
    sorgu.exec();
    //stokkartlari tablosunu oluşturma.
    sorgu.exec("CREATE TABLE stokkartlari("
                "id BIGSERIAL PRIMARY KEY,"
                "barkod VARCHAR(13) NOT NULL,"
                "ad TEXT NOT NULL,"
                "birim VARCHAR(8) NOT NULL,"
                "miktar DECIMAL(18,3) NOT NULL,"
                "grup VARCHAR(32) NOT NULL,"
                "afiyat MONEY NOT NULL,"
                "sfiyat MONEY NOT NULL,"
                "kdv1 SERIAL,"
                "kdv2 SERIAL,"
                "kdv3 SERIAL,"
                "tarih TIMESTAMP NOT NULL,"
                "aciklama TEXT)");
    sorgu.exec("CREATE SEQUENCE stokkartlari_sequence START WITH 10000 INCREMENT BY 1 OWNED BY stokkartlari.id");
    //stokhareketleri tablosu oluşturma.
    sorgu.exec("CREATE TABLE stokhareketleri("
                "barkod VARCHAR(13) NOT NULL,"
                "islem_no VARCHAR,"
                "islem_turu VARCHAR NOT NULL,"
                "islem_miktari DECIMAL(18,3) NOT NULL,"
                "tarih TIMESTAMP NOT NULL,"
                "kullanici BIGSERIAL NOT NULL,"
                "aciklama TEXT)");
    //faturalar tablosu olusturma
    sorgu.exec("CREATE TABLE faturalar("
                "id BIGSERIAL PRIMARY KEY,"
                "fatura_no TEXT NOT NULL,"
                "durum BOOLEAN DEFAULT FALSE,"
                "toplamtutar MONEY NOT NULL DEFAULT 0,"
                "odenentutar MONEY NOT NULL DEFAULT 0,"
                "kalantutar MONEY NOT NULL DEFAULT 0,"
                "cari BIGSERIAL NOT NULL,"
                "tipi TEXT NOT NULL,"
                "tarih TIMESTAMP NOT NULL)");
    sorgu.exec("CREATE SEQUENCE faturalar_sequence START WITH 1000 INCREMENT BY 1 OWNED BY faturalar.id");
    //stokgruplari tablosu oluşturma
    sorgu.exec("CREATE TABLE stokgruplari("
                "id BIGSERIAL PRIMARY KEY NOT NULL,"
                "grup TEXT NOT NULL)");
    sorgu.exec("CREATE SEQUENCE stokgruplari_sequence START WITH 100 INCREMENT BY 1 OWNED BY stokgruplari.id");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'GIDA')");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'TÜTÜN')");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'ALKOLSÜZ İÇECEK')");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'ALKOLLÜ İÇECEK')");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'TEMİZLİK')");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'MANAV')");
    //carikartlar tablosu oluşturma
    sorgu.exec("CREATE TABLE carikartlar("
                "id BIGSERIAL PRIMARY KEY NOT NULL,"
                "ad TEXT NOT NULL,"
                "tc VARCHAR(11) NOT NULL,"
                "vergi_no TEXT NOT NULL,"
                "vergi_daire TEXT NOT NULL,"
                "il TEXT,"
                "ilce TEXT,"
                "adres TEXT,"
                "mail TEXT,"
                "telefon VARCHAR(10) DEFAULT 0000000000,"
                "tarih DATE NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                "aciklama TEXT)");
    qDebug() << sorgu.lastError().text();
    sorgu.exec("CREATE SEQUENCE carikartlar_sequence START WITH 1000 INCREMENT BY 1 OWNED BY carikartlar.id");
    qDebug() << sorgu.lastError().text();
    sorgu.prepare("INSERT INTO carikartlar (id, ad, tc, vergi_no, vergi_daire, il, ilce, adres, mail, telefon, tarih, aciklama) "
                    "VALUES (nextval('carikartlar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, "DİREKT");
    sorgu.bindValue(1, "00000000000");
    sorgu.bindValue(2, "");
    sorgu.bindValue(3, "");
    sorgu.bindValue(4, "");
    sorgu.bindValue(5, "");
    sorgu.bindValue(6, "");
    sorgu.bindValue(7, "");
    sorgu.bindValue(8, "");
    sorgu.bindValue(9, QDate::currentDate());
    sorgu.bindValue(10, "");
    sorgu.exec();
    qDebug() << sorgu.lastError().text();
    db.close();
}

User Veritabani::GetUserInfos(QString _UserName)
{
    db.setDatabaseName("mhss_data");
    db.open();
    User u;
    sorgu.prepare("SELECT * FROM kullanicilar WHERE username = ?");
    sorgu.bindValue(0, _UserName);
    sorgu.exec();
    if(sorgu.next()){
        u.setUserInfos(sorgu.value(0).toString(), sorgu.value(1).toString(), sorgu.value(3).toString(), sorgu.value(5).toString());
    }
    db.close();
    return u;
}

QList<Cari> Veritabani::getCariKartlar()
{
    db.setDatabaseName("mhss_data");
    db.open();
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
    db.close();
    return kartlar;
}

QList<QString> Veritabani::GetUsers()
{
    db.setDatabaseName("mhss_data");
    db.open();
    sorgu.exec("SELECT username FROM kullanicilar");
    while (sorgu.next()) {
        users << sorgu.value(0).toString();
    }
    db.close();
    return users;
}

StokKarti Veritabani::getStokKarti(QString _Barkod)
{
    StokKarti kart;
    db.setDatabaseName("mhss_data");
    db.open();
    sorgu.prepare("SELECT stokid, barkod, ad, birim, miktar, stokgrup, afiyat, sfiyat, kdv1, kdv2, kdv3, songuntarih, aciklama FROM stokkartlari WHERE barkod = ?");
    sorgu.bindValue(0, _Barkod);
    sorgu.exec();
    if(sorgu.next()){
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
    }
    db.close();
    return kart;
}

void Veritabani::yeniStokKartiOlustur()
{

}

