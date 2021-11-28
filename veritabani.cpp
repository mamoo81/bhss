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

}

Veritabani::~Veritabani()
{

}

bool Veritabani::barkodVarmi(QString _Barkod)
{
    QSqlQuery sorgu = QSqlQuery(db);
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

bool Veritabani::loginControl(QString _UserName, QString _Password)
{
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("SELECT username, password FROM kullanicilar WHERE username = ?");
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
void Veritabani::satisYap(Sepet _satilacakSepet, User _satisYapanKullanici, int _satisYapilanCariID)
{
    QSqlQuery sorgu = QSqlQuery(db);
    //yeni fatura numarası için faturalar_sequence'den son değeri alma
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.next();
    QString yeniFaturaNo = QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt() + 1);
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, yeniFaturaNo);
    sorgu.bindValue(1, _satisYapilanCariID);
    sorgu.bindValue(2, "SATIŞ");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, _satisYapanKullanici.getUserID());
    sorgu.bindValue(5, _satilacakSepet.sepetToplamTutari());
    sorgu.bindValue(6, _satilacakSepet.getOdenenTutar());
    sorgu.bindValue(7, (_satilacakSepet.getKalanTutar()));
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    //kasaya artı para girme
    sorgu.exec("SELECT * FROM kasa");
    sorgu.next();
    double suankiPara = sorgu.value(1).toDouble();
    double guncelKasaPara = suankiPara + _satilacakSepet.sepetToplamTutari();
    sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
    sorgu.bindValue(0, guncelKasaPara);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    //kasa hareketlerini girme
    sorgu.prepare("INSERT INTO kasahareketleri(id, miktar, kullanici, islem, tarih) VALUES (nextval('kasahareketleri_sequence'),?,?,?,?)");
    sorgu.bindValue(0, _satilacakSepet.sepetToplamTutari());
    sorgu.bindValue(1, _satisYapanKullanici.getUserID());
    sorgu.bindValue(2, "giriş");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    //sepetteki ürünlerin stok hareketlerine girişi
    foreach (auto urun, _satilacakSepet.urunler) {
        sorgu.prepare("INSERT INTO stokhareketleri(barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, urun.barkod);
        sorgu.bindValue(1, yeniFaturaNo);
        sorgu.bindValue(2, "SATIŞ");
        sorgu.bindValue(3, urun.miktar);
        sorgu.bindValue(4, QDateTime::currentDateTime());
        sorgu.bindValue(5, _satisYapanKullanici.getUserID());
        sorgu.bindValue(6, "SATIŞ");
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << sorgu.lastError().text();
        }
    }
    //sepetteki ürünlerin stoklardan düşülmesi
    foreach (auto urun, _satilacakSepet.urunler) {
        sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE barkod = ?");
        sorgu.bindValue(0, urun.stokMiktari - urun.miktar);
        sorgu.bindValue(1, urun.barkod);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << sorgu.lastError().text();
        }
    }
}

QStringList Veritabani::getSonIslemler()
{
    QStringList islemler;
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.exec("SELECT fatura_no, tarih FROM faturalar WHERE tarih::date = now()::date ORDER BY fatura_no DESC");
    while(sorgu.next()){
        islemler.append(sorgu.value(0).toString() + " " + sorgu.value(1).toTime().toString("hh:mm:ss"));
    }
    return islemler;
}

double Veritabani::getKasadakiPara()
{
    sorgu.exec("SELECT * FROM kasa");
    sorgu.next();
    return sorgu.value(1).toDouble();
}

bool Veritabani::veritabaniVarmi()
{
    QSqlQuery sorgu = QSqlQuery(db);
    //mhss_data veritabanı varmı kontrol
    sorgu.exec("SELECT datname FROM pg_database WHERE datname = 'mhss_data'");
    if(!sorgu.next()){
        return false;
    }
    else{
        db.setDatabaseName("mhss_data");
        db.close();
        db.open();
    }
    return true;
}

void Veritabani::veritabaniOlustur()
{
    QSqlQuery sorgu = QSqlQuery(db);
    //veritabanını oluşturma
    sorgu.exec("CREATE DATABASE mhss_data OWNER postgres");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    db.setDatabaseName("mhss_data");
    db.close();
    db.open();
    //kullanıcılar tablosunu oluşturma
    sorgu.exec("CREATE TABLE kullanicilar("
                "id BIGSERIAL PRIMARY KEY,"
                "username VARCHAR(32) NOT NULL,"
                "password VARCHAR(32) NOT NULL,"
                "ad TEXT,"
                "soyad TEXT,"
                "cepno VARCHAR(10),"
                "tarih TIMESTAMP NOT NULL,"
                "kasayetki BOOLEAN DEFAULT FALSE,"
                "iadeyetki BOOLEAN DEFAULT FALSE,"
                "stokyetki BOOLEAN DEFAULT FALSE)");
    sorgu.exec("CREATE SEQUENCE kullanicilar_sequence START WITH 100 INCREMENT BY 1 OWNED BY kullanicilar.id");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    //kullanıcılar tablosuna admin kullanıcısını ekleme
    sorgu.prepare("INSERT INTO kullanicilar(id, username, password, ad, soyad, cepno, tarih, kasayetki, iadeyetki, stokyetki) "
                    "VALUES (nextval('kullanicilar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, "admin");
    sorgu.bindValue(1, "admin");
    sorgu.bindValue(2, "ADMİN");
    sorgu.bindValue(3, "ADMİN");
    sorgu.bindValue(4, "0000000000");
    sorgu.bindValue(5, QDate::currentDate());
    sorgu.bindValue(6, true);
    sorgu.bindValue(7, true);
    sorgu.bindValue(8, true);
    sorgu.exec();
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
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
                "kdv SERIAL,"
                "tarih TIMESTAMP NOT NULL,"
                "aciklama TEXT)");
    sorgu.exec("CREATE SEQUENCE stokkartlari_sequence START WITH 10000 INCREMENT BY 1 OWNED BY stokkartlari.id");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    //stokhareketleri tablosu oluşturma.
    sorgu.exec("CREATE TABLE stokhareketleri("
                "barkod VARCHAR(13) NOT NULL,"
                "islem_no VARCHAR,"
                "islem_turu VARCHAR NOT NULL,"
                "islem_miktari DECIMAL(18,3) NOT NULL,"
                "tarih TIMESTAMP NOT NULL,"
                "kullanici BIGSERIAL NOT NULL,"
                "aciklama TEXT)");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    //faturalar tablosu olusturma
    sorgu.exec("CREATE TABLE faturalar("
                "id BIGSERIAL PRIMARY KEY,"
                "fatura_no TEXT NOT NULL,"
                "kullanici BIGSERIAL NOT NULL,"
                "durum BOOLEAN DEFAULT FALSE,"
                "toplamtutar DECIMAL(18,3) NOT NULL DEFAULT 0,"
                "odenentutar DECIMAL(18,3) NOT NULL DEFAULT 0,"
                "kalantutar DECIMAL(18,3) NOT NULL DEFAULT 0,"
                "cari BIGSERIAL NOT NULL,"
                "tipi TEXT NOT NULL,"
                "tarih TIMESTAMP NOT NULL)");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.exec("CREATE SEQUENCE faturalar_sequence START WITH 1000 INCREMENT BY 1 OWNED BY faturalar.id");
    sorgu.exec("SELECT setval('faturalar_sequence', 1000)");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    //stokgruplari tablosu oluşturma
    sorgu.exec("CREATE TABLE stokgruplari("
                "id BIGSERIAL PRIMARY KEY NOT NULL,"
                "grup TEXT NOT NULL)");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.exec("CREATE SEQUENCE stokgruplari_sequence START WITH 100 INCREMENT BY 1 OWNED BY stokgruplari.id");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'GIDA')");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'TÜTÜN')");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'ALKOLSÜZ İÇECEK')");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'ALKOLLÜ İÇECEK')");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'TEMİZLİK')");
    sorgu.exec("INSERT INTO stokgruplari(id, grup) VALUES(nextval('stokgruplari_sequence'), 'MANAV')");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    //carikartlar tablosu oluşturma
    sorgu.exec("CREATE TABLE carikartlar("
                "id BIGSERIAL PRIMARY KEY NOT NULL,"
                "ad TEXT NOT NULL,"
                "tip BIGSERIAL NOT NULL,"
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
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.exec("CREATE SEQUENCE carikartlar_sequence START WITH 1000 INCREMENT BY 1 OWNED BY carikartlar.id");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
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
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    // caritipleri tablosu oluşturma
    sorgu.exec("CREATE TABLE caritipleri ("
                    "id BIGSERIAL PRIMARY KEY NOT NULL,"
                    "tip TEXT NOT NULL)");
    sorgu.exec("CREATE SEQUENCE caritipleri_sequence START WITH 1 INCREMENT BY 1 OWNED BY caritipleri.id");
    sorgu.exec("INSERT INTO caritipleri(id, tip) VALUES(nextval('caritipleri_sequence'), 'MÜŞTERİ')");
    sorgu.exec("INSERT INTO caritipleri(id, tip) VALUES(nextval('caritipleri_sequence'), 'TOPTANCI')");
    // kasa tablosu oluşturma
    sorgu.exec("CREATE TABLE kasa("
                "id BIGSERIAL PRIMARY KEY NOT NULL,"
                "para DECIMAL(18,3) NOT NULL)");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.exec("INSERT INTO kasa (id, para) VALUES('1','0')");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    // kasa hareketleri tablosu oluşturma
    sorgu.exec("CREATE TABLE kasahareketleri("
                  "id BIGSERIAL PRIMARY KEY NOT NULL,"
                  "miktar DECIMAL(18,3) NOT NULL,"
                  "kullanici BIGSERIAL NOT NULL,"
                  "islem TEXT,"
                  "tarih TIMESTAMP NOT NULL)");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.exec("CREATE SEQUENCE kasahareketleri_sequence START WITH 1000 INCREMENT BY 1 OWNED BY kasahareketleri.id");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
    QMessageBox msg(0);
    msg.setText("localhost'ta veritabanı oluşturuldu");
    msg.setWindowTitle("Bilgi");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, "Tamam");
    msg.setIcon(QMessageBox::Information);
    msg.exec();
}

User Veritabani::GetUserInfos(QString _UserName)
{
    User u;
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("SELECT * FROM kullanicilar WHERE username = ?");
    sorgu.bindValue(0, _UserName);
    sorgu.exec();
    if(sorgu.next()){
        u.setUserID(sorgu.value(0).toString());
        u.setUserName(sorgu.value(1).toString());
        u.setPassWord(sorgu.value(2).toString());
        u.setAd(sorgu.value(3).toString());
        u.setSoyad(sorgu.value(4).toString());
        u.setCepNo(sorgu.value(5).toString());
        u.setTarih(sorgu.value(6).toDateTime());
        u.setKasaYetki(sorgu.value(7).toBool());
        u.setIadeYetki(sorgu.value(8).toBool());
        u.setStokYetki(sorgu.value(9).toBool());
    }
    return u;
}

void Veritabani::updateUser(User _NewUserInfos)
{
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("UPDATE kullanicilar SET password = ?, ad = ?, soyad = ?, cepno = ?, tarih = ?, kasayetki = ?, iadeyetki = ?, stokyetki = ? "
                    "WHERE id = ?");
    sorgu.bindValue(0, _NewUserInfos.getPassWord());
    sorgu.bindValue(1, _NewUserInfos.getAd());
    sorgu.bindValue(2, _NewUserInfos.getSoyad());
    sorgu.bindValue(3, _NewUserInfos.getCepNo());
    sorgu.bindValue(4, _NewUserInfos.getTarih());
    sorgu.bindValue(5, _NewUserInfos.getKasaYetki());
    sorgu.bindValue(6, _NewUserInfos.getIadeYetki());
    sorgu.bindValue(7, _NewUserInfos.getStokYetki());
    sorgu.bindValue(8, _NewUserInfos.getUserID());
    if(sorgu.exec()){
        QMessageBox *msg = new QMessageBox(0);
        msg->setIcon(QMessageBox::Information);
        msg->setWindowTitle("Başarılı");
        msg->setText("Kullanıcı güncellendi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->exec();
    }
    else{
        if(sorgu.lastError().isValid()){
            QMessageBox *msg = new QMessageBox(0);
            msg->setIcon(QMessageBox::Information);
            msg->setWindowTitle("Hata");
            msg->setText("Güncelleme Başarısız!");
            msg->setInformativeText(sorgu.lastError().text());
            msg->setStandardButtons(QMessageBox::Ok);
            msg->setDefaultButton(QMessageBox::Ok);
            msg->setButtonText(QMessageBox::Ok, "Tamam");
            msg->exec();
        }
    }
}

void Veritabani::CreateNewUser(User _NewUser)
{
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("INSERT INTO kullanicilar(id, username, password, ad, soyad, cepno, tarih, kasayetki, iadeyetki, stokyetki)"
                    " VALUES(nextval('kullanicilar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, _NewUser.getUserName());
    sorgu.bindValue(1, _NewUser.getPassWord());
    sorgu.bindValue(2, _NewUser.getAd());
    sorgu.bindValue(3, _NewUser.getSoyad());
    sorgu.bindValue(4, _NewUser.getCepNo());
    sorgu.bindValue(5, _NewUser.getTarih());
    sorgu.bindValue(6, _NewUser.getKasaYetki());
    sorgu.bindValue(7, _NewUser.getIadeYetki());
    sorgu.bindValue(8, _NewUser.getStokYetki());
    if(sorgu.exec()){
        QMessageBox *msg = new QMessageBox(0);
        msg->setIcon(QMessageBox::Information);
        msg->setWindowTitle("Başarılı");
        msg->setText("Yeni kullanıcı oluşturuldu.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->exec();
    }
    else{
        if(sorgu.lastError().isValid()){
            QMessageBox *msg = new QMessageBox(0);
            msg->setIcon(QMessageBox::Information);
            msg->setWindowTitle("Hata");
            msg->setText("Yeni kullanıcı oluşturulamadı!");
            msg->setInformativeText(sorgu.lastError().text());
            msg->setStandardButtons(QMessageBox::Ok);
            msg->setDefaultButton(QMessageBox::Ok);
            msg->setButtonText(QMessageBox::Ok, "Tamam");
            msg->exec();
        }
    }
}

void Veritabani::deleteUser(QString _DeletedUserName)
{
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("DELETE FROM kullanicilar WHERE username = ?");
    sorgu.bindValue(0, _DeletedUserName);
    if(sorgu.exec()){
        QMessageBox *msg = new QMessageBox(0);
        msg->setIcon(QMessageBox::Information);
        msg->setWindowTitle("Başarılı");
        msg->setText(_DeletedUserName + " kullanıcısı silindi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->exec();
    }
    else{
        if(sorgu.lastError().isValid()){
            QMessageBox *msg = new QMessageBox(0);
            msg->setIcon(QMessageBox::Information);
            msg->setWindowTitle("Hata");
            msg->setText("Kullanıcı silinemedi!");
            msg->setInformativeText(sorgu.lastError().text());
            msg->setStandardButtons(QMessageBox::Ok);
            msg->setDefaultButton(QMessageBox::Ok);
            msg->setButtonText(QMessageBox::Ok, "Tamam");
            msg->exec();
        }
    }
}

QList<Cari> Veritabani::getCariKartlar()
{
    QList<Cari> kartlar;
    QSqlQuery sorgu = QSqlQuery(db);
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

Cari Veritabani::getCariKart(QString _cariID)
{
    Cari kart;
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("SELECT * FROM carikartlar WHERE id = ?");
    sorgu.bindValue(0, _cariID);
    sorgu.exec();
    if(sorgu.next()){
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
        return kart;
    }
    else{
        return kart;
    }
}

QList<QString> Veritabani::GetUsers()
{
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.exec("SELECT username FROM kullanicilar");
    while (sorgu.next()) {
        users << sorgu.value(0).toString();
    }
    return users;
}

StokKarti Veritabani::getStokKarti(QString _Barkod)
{
    StokKarti kart = StokKarti();
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("SELECT id, barkod, ad, birim, miktar, grup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv, tarih, aciklama FROM stokkartlari WHERE barkod = ?");
    sorgu.bindValue(0, _Barkod);
    sorgu.exec();
    if(sorgu.next()){
        kart.setId(sorgu.value(0).toString());
        kart.setBarkod(sorgu.value(1).toString());
        kart.setAd(sorgu.value(2).toString());
        kart.setBirim(sorgu.value(3).toString());
        kart.setMiktar(sorgu.value(4).toFloat());
        kart.setGrup(sorgu.value(5).toString());
        kart.setAFiyat(sorgu.value(6).toDouble());
        kart.setSFiyat(sorgu.value(7).toDouble());
        kart.setKdv(sorgu.value(8).toInt());
        kart.setTarih(sorgu.value(9).toDateTime());
        kart.setAciklama(sorgu.value(10).toString());
    }
    return kart;
}

void Veritabani::yeniStokKartiOlustur(StokKarti _StokKarti, User *_Kullanici)
{
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("INSERT INTO stokkartlari (id, barkod, ad, birim, miktar, grup, afiyat, sfiyat, kdv, tarih, aciklama) "
                    "VALUES (nextval('stokkartlari_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, _StokKarti.getBarkod());
    sorgu.bindValue(1, _StokKarti.getAd());
    sorgu.bindValue(2, _StokKarti.getBirim());
    sorgu.bindValue(3, _StokKarti.getMiktar());
    sorgu.bindValue(4, _StokKarti.getGrup());
    sorgu.bindValue(5, _StokKarti.getAFiyat());
    sorgu.bindValue(6, _StokKarti.getSFiyat());
    sorgu.bindValue(7, _StokKarti.getKdv());
    sorgu.bindValue(8, _StokKarti.getTarih());
    sorgu.bindValue(9, _StokKarti.getAciklama() + " [" + _Kullanici->getUserName() + "]");
    if(sorgu.exec()){
        QMessageBox *msg = new QMessageBox(0);
        msg->setIcon(QMessageBox::Information);
        msg->setWindowTitle("Başarılı");
        msg->setText("Yeni stok kartı oluşturuldu.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->exec();
    }
    else{
        QMessageBox *msg = new QMessageBox(0);
        msg->setIcon(QMessageBox::Critical);
        msg->setWindowTitle("Hata");
        msg->setText("Yeni stok kartı oluşturulamadı.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->exec();
    }
}

void Veritabani::stokKartiniGuncelle(const QString _EskiStokKartiID, StokKarti _YeniStokKarti, User *_Kullanici)
{
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("UPDATE stokkartlari SET barkod = ?, ad = ?, birim = ?, miktar = ?, grup = ?, afiyat = ?, sfiyat = ?, kdv = ?, tarih = ? , aciklama = ? "
                        "WHERE id = ?");
    sorgu.bindValue(0, _YeniStokKarti.getBarkod());
    sorgu.bindValue(1, _YeniStokKarti.getAd());
    sorgu.bindValue(2, _YeniStokKarti.getBirim());
    sorgu.bindValue(3, _YeniStokKarti.getMiktar());
    sorgu.bindValue(4, _YeniStokKarti.getGrup());
    sorgu.bindValue(5, _YeniStokKarti.getAFiyat());
    sorgu.bindValue(6, _YeniStokKarti.getSFiyat());
    sorgu.bindValue(7, _YeniStokKarti.getKdv());
    sorgu.bindValue(8, _YeniStokKarti.getTarih());
    sorgu.bindValue(9, _YeniStokKarti.getAciklama() + " " + _Kullanici->getUserName());
    sorgu.bindValue(10, _EskiStokKartiID);
    if(sorgu.exec()){
        QMessageBox *msg = new QMessageBox();
        msg->setIcon(QMessageBox::Information);
        msg->setWindowTitle("Bilgi");
        msg->setText("Stok kartı güncellendi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->exec();
    }
    else{
        QMessageBox *msg = new QMessageBox();
        msg->setWindowTitle("Hata");
        msg->setText("Stok kartı güncellenemedi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setIcon(QMessageBox::Information);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->exec();
    }
}

void Veritabani::stokKartiSil(QString _StokKartiID)
{
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("DELETE FROM stokkartlari WHERE id = ?");
    sorgu.bindValue(0, _StokKartiID);
    if(sorgu.exec()){
        QMessageBox *msg = new QMessageBox();
        msg->setIcon(QMessageBox::Information);
        msg->setWindowTitle("Bilgi");
        msg->setText("Stok kartı silindi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->exec();
    }
    else{
        QMessageBox *msg = new QMessageBox();
        msg->setIcon(QMessageBox::Warning);
        msg->setWindowTitle("Bilgi");
        msg->setText("Stok kartı silinemedi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->exec();
    }
}

QSqlQueryModel *Veritabani::getStokKartlari()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setHeaderData(0, Qt::Horizontal, "Stok ID");
    model->setHeaderData(1, Qt::Horizontal, "Barkod");
    model->setHeaderData(2, Qt::Horizontal, "Adı");
    model->setHeaderData(3, Qt::Horizontal, "Birim");
    model->setHeaderData(4, Qt::Horizontal, "Miktar");
    model->setHeaderData(5, Qt::Horizontal, "Stok Grup");
    model->setHeaderData(6, Qt::Horizontal, "Alış Fiyat");
    model->setHeaderData(7, Qt::Horizontal, "Satış Fiyat");
    model->setHeaderData(8, Qt::Horizontal, "Gün. tarihi");
    model->setHeaderData(9, Qt::Horizontal, "Açıklama");
    model->setQuery("SELECT id, barkod, ad, birim, miktar, grup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv, tarih, aciklama FROM stokkartlari ORDER BY ad ASC", db);
    return model;
}

QSqlQueryModel *Veritabani::getStokKartlari(QString query)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setHeaderData(0, Qt::Horizontal, "Barkod");
    model->setHeaderData(1, Qt::Horizontal, "Ürün Adı");
    model->setQuery(query, db);
    return model;
}

QStringList Veritabani::stokGruplariGetir()
{
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.exec("SELECT grup FROM stokgruplari");
    QStringList liste;
    while (sorgu.next()) {
        liste.append(sorgu.value(0).toString());
    }
    return liste;
}

Sepet Veritabani::getSatis(QString _faturaNo)
{
    Sepet satilmisSepet;
    QSqlQuery satisSorgu = QSqlQuery(db);// aşağıda while içinde ki satis.urunEkle() metodunda çağrılacak sorgu nesnesi ile karışmasın diye yeni query nesnesi oluşturdum.
    satisSorgu.prepare("SELECT barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama FROM stokhareketleri WHERE islem_no = ?");
    satisSorgu.bindValue(0, _faturaNo);
    satisSorgu.exec();
    while (satisSorgu.next()) {
        StokKarti sk = getStokKarti(satisSorgu.value(0).toString());
        satilmisSepet.urunEkle(sk, satisSorgu.value(3).toFloat());
    }
    return satilmisSepet;
}

QSqlQuery Veritabani::getIslemInfo(QString _faturaNo)
{
    QSqlQuery islemSorgu = QSqlQuery(db);
    islemSorgu.prepare("SELECT * FROM faturalar WHERE fatura_no = ?");
    islemSorgu.bindValue(0, _faturaNo);
    islemSorgu.exec();
    islemSorgu.next();
    return islemSorgu;
}

void Veritabani::kasadanParaCek(double _cekilecekTutar, User _kullanici)
{
    QSqlQuery sorgu = QSqlQuery(db);
    // kasada ki parayı güncelleme
    double kasadaKalanPara = getKasadakiPara() - _cekilecekTutar;
    sorgu.prepare("UPDATE kasa SET para = ?");
    sorgu.bindValue(0, kasadaKalanPara);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qWarning() << "kasadanParaCek UPDATE hatası: " << sorgu.lastError().text();
    }
    //kasa hareketlerini girme
    sorgu.prepare("INSERT INTO kasahareketleri(id, miktar, kullanici, islem, tarih) VALUES (nextval('kasahareketleri_sequence'),?,?,?,?)");
    sorgu.bindValue(0, _cekilecekTutar);
    sorgu.bindValue(1, _kullanici.getUserID());
    sorgu.bindValue(2, "çıkış");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << "kasadanParaCek kasa hareketleri hatası: " << sorgu.lastError().text();
    }
}

double Veritabani::getGunlukCiro()
{
    QSqlQuery sorgu = QSqlQuery(db);
    sorgu.prepare("SELECT SUM(miktar) FROM kasahareketleri WHERE tarih > ? AND islem IN(?,?)");
    sorgu.bindValue(0, QDateTime::currentDateTime().date());
    sorgu.bindValue(1, "giriş");
    sorgu.bindValue(2, "iade");
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qWarning() << sorgu.lastError().text();
    }
    sorgu.next();
    return sorgu.value(0).toDouble();
}

void Veritabani::iadeAl(Sepet _iadeSepet, User _kullanici)
{
    QSqlQuery sorgu = QSqlQuery(db);
    // iade faturano alımı ve yeni oluşturma.
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.next();
    QString iadeFaturaNo = QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt() + 1);;
    // iade fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, iadeFaturaNo);
    sorgu.bindValue(1, 1000);// DİREKT cari id
    sorgu.bindValue(2, "İADE");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, _kullanici.getUserID());
    sorgu.bindValue(5, _iadeSepet.sepetToplamTutari());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    // kasaya eksi para girme
    sorgu.exec("SELECT * FROM kasa");
    sorgu.next();
    double suankiPara = sorgu.value(1).toDouble();
    double guncelKasaPara = suankiPara - _iadeSepet.sepetToplamTutari();
    sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
    sorgu.bindValue(0, guncelKasaPara);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    //kasa hareketlerini girme
    sorgu.prepare("INSERT INTO kasahareketleri(id, miktar, kullanici, islem, tarih) VALUES (nextval('kasahareketleri_sequence'),?,?,?,?)");
    sorgu.bindValue(0, -_iadeSepet.sepetToplamTutari());// eksi değer kaydediyorum
    sorgu.bindValue(1, 1000);// DİREKT cari id
    sorgu.bindValue(2, "iade");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    //sepetteki iade ürünlerin stok hareketlerine girişi
    foreach (auto urun, _iadeSepet.urunler) {
        sorgu.prepare("INSERT INTO stokhareketleri(barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, urun.barkod);
        sorgu.bindValue(1, iadeFaturaNo);
        sorgu.bindValue(2, "İADE");
        sorgu.bindValue(3, urun.miktar);
        sorgu.bindValue(4, QDateTime::currentDateTime());
        sorgu.bindValue(5, _kullanici.getUserID());
        sorgu.bindValue(6, "İADE");
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << sorgu.lastError().text();
        }
    }
    // sepetteki iade ürünlerin stoğa eklenmesi
    foreach (Urun urun, _iadeSepet.urunler) {
        sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE barkod = ?");
        sorgu.bindValue(0, urun.miktar + urun.stokMiktari);
        sorgu.bindValue(1, urun.barkod);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qWarning() << "iade ürün stoğa geri ekleme hatası: " << sorgu.lastError().text();
        }
    }
}

void Veritabani::iadeAl(Sepet _iadeSepet, User _kullanici, Cari _iadeCari)
{
    QSqlQuery sorgu = QSqlQuery(db);
    // iade faturano alımı ve yeni oluşturma.
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.next();
    QString iadeFaturaNo = QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt() + 1);;
    // iade fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, iadeFaturaNo);
    sorgu.bindValue(1, _iadeCari.getId());
    sorgu.bindValue(2, "İADE");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, _kullanici.getUserID());
    sorgu.bindValue(5, _iadeSepet.sepetToplamTutari());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    // kasaya eksi para girme
    sorgu.exec("SELECT * FROM kasa");
    sorgu.next();
    double suankiPara = sorgu.value(1).toDouble();
    double guncelKasaPara = suankiPara - _iadeSepet.sepetToplamTutari();
    sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
    sorgu.bindValue(0, guncelKasaPara);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    //kasa hareketlerini girme
    sorgu.prepare("INSERT INTO kasahareketleri(id, miktar, kullanici, islem, tarih) VALUES (nextval('kasahareketleri_sequence'),?,?,?,?)");
    sorgu.bindValue(0, _iadeSepet.sepetToplamTutari());
    sorgu.bindValue(1, _iadeCari.getId());
    sorgu.bindValue(2, "iade");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    //sepetteki iade ürünlerin stok hareketlerine girişi
    foreach (auto urun, _iadeSepet.urunler) {
        sorgu.prepare("INSERT INTO stokhareketleri(barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, urun.barkod);
        sorgu.bindValue(1, iadeFaturaNo);
        sorgu.bindValue(2, "İADE");
        sorgu.bindValue(3, urun.miktar);
        sorgu.bindValue(4, QDateTime::currentDateTime());
        sorgu.bindValue(5, _kullanici.getUserID());
        sorgu.bindValue(6, "İADE");
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << sorgu.lastError().text();
        }
    }
    // sepetteki iade ürünlerin stoğa eklenmesi
    foreach (Urun urun, _iadeSepet.urunler) {
        sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE barkod = ?");
        sorgu.bindValue(0, urun.miktar + urun.stokMiktari);
        sorgu.bindValue(1, urun.barkod);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qWarning() << "iade ürün stoğa geri ekleme hatası: " << sorgu.lastError().text();
        }
    }
}

