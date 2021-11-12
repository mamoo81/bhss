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
    //yeni fatura numarası için faturalar_sequence'den son değeri alma
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.next();
    QString yeniFaturaNo = QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt() + 1);
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, yeniFaturaNo);
    sorgu.bindValue(1, _satisYapilanCariID);
    sorgu.bindValue(2, "SATIŞ");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, _satisYapanKullanici.getUserID());
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
    sorgu.bindValue(1, _satisYapilanCariID);
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
                "tarih TIMESTAMP NOT NULL)");
    sorgu.exec("CREATE SEQUENCE kullanicilar_sequence START WITH 100 INCREMENT BY 1 OWNED BY kullanicilar.id");
    if(!QString(sorgu.lastError().text()).isEmpty()){
        qDebug() << sorgu.lastError().text();
    }
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
    sorgu.prepare("SELECT * FROM kullanicilar WHERE username = ?");
    sorgu.bindValue(0, _UserName);
    sorgu.exec();
    if(sorgu.next()){
        u.setUserID(sorgu.value(0).toString());
        u.setUserName(sorgu.value(1).toString());
        u.setAd(sorgu.value(3).toString());
        u.setSoyad(sorgu.value(4).toString());
        u.setCepNo(sorgu.value(5).toString());
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
    sorgu.exec("SELECT username FROM kullanicilar");
    while (sorgu.next()) {
        users << sorgu.value(0).toString();
    }
    return users;
}

StokKarti Veritabani::getStokKarti(QString _Barkod)
{
    StokKarti kart;
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

void Veritabani::yeniStokKartiOlustur(StokKarti *_StokKarti, User *_Kullanici)
{
    sorgu.prepare("INSERT INTO stokkartlari (id, barkod, ad, birim, miktar, grup, afiyat, sfiyat, kdv, tarih, aciklama) "
                    "VALUES (nextval('stokkartlari_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, _StokKarti->getBarkod());
    sorgu.bindValue(1, _StokKarti->getAd());
    sorgu.bindValue(2, _StokKarti->getBirim());
    sorgu.bindValue(3, _StokKarti->getMiktar());
    sorgu.bindValue(4, _StokKarti->getGrup());
    sorgu.bindValue(5, _StokKarti->getAFiyat());
    sorgu.bindValue(6, _StokKarti->getSFiyat());
    sorgu.bindValue(7, _StokKarti->getKdv());
    sorgu.bindValue(8, _StokKarti->getTarih());
    sorgu.bindValue(9, _StokKarti->getAciklama() + " [" + _Kullanici->getUserName() + "]");
    if(sorgu.exec()){
        QMessageBox *msg = new QMessageBox(0);
        msg->setIcon(QMessageBox::Information);
        msg->setText("Başarılı");
        msg->setInformativeText("Yeni stok kartı oluşturuldu.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->setModal(true);
        msg->show();
    }
    else{
        QMessageBox *msg = new QMessageBox(0);
        msg->setIcon(QMessageBox::Critical);
        msg->setText("Hata");
        msg->setInformativeText("Yeni stok kartı oluşturulamadı.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->setModal(true);
        msg->show();
    }
}

void Veritabani::stokKartiniGuncelle(const QString _EskiStokKartiID, StokKarti *_YeniStokKarti, User *_Kullanici)
{
    sorgu.prepare("UPDATE stokkartlari SET barkod = ?, ad = ?, birim = ?, miktar = ?, grup = ?, afiyat = ?, sfiyat = ?, kdv = ?, tarih = ? , aciklama = ? "
                        "WHERE id = ?");
    sorgu.bindValue(0, _YeniStokKarti->getBarkod());
    sorgu.bindValue(1, _YeniStokKarti->getAd());
    sorgu.bindValue(2, _YeniStokKarti->getBirim());
    sorgu.bindValue(3, _YeniStokKarti->getMiktar());
    sorgu.bindValue(4, _YeniStokKarti->getGrup());
    sorgu.bindValue(5, _YeniStokKarti->getAFiyat());
    sorgu.bindValue(6, _YeniStokKarti->getSFiyat());
    sorgu.bindValue(7, _YeniStokKarti->getKdv());
    sorgu.bindValue(8, _YeniStokKarti->getTarih());
    sorgu.bindValue(9, _YeniStokKarti->getAciklama() + " " + _Kullanici->getUserName());
    sorgu.bindValue(10, _EskiStokKartiID);
    if(sorgu.exec()){
        QMessageBox *msg = new QMessageBox(0);
        msg->setIcon(QMessageBox::Information);
        msg->setText("Bilgi");
        msg->setInformativeText("Stok kartı güncellendi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->setModal(true);
        msg->show();
    }
    else{
        QMessageBox *msg = new QMessageBox(0);
        msg->setText("Hata");
        msg->setInformativeText("Stok kartı güncellenemedi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setIcon(QMessageBox::Information);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->setModal(true);
        msg->show();
    }
}

void Veritabani::stokKartiSil(QString _StokKartiID)
{
    sorgu.prepare("DELETE FROM stokkartlari WHERE id = ?");
    sorgu.bindValue(0, _StokKartiID);
    if(sorgu.exec()){
        QMessageBox *msg = new QMessageBox();
        msg->setIcon(QMessageBox::Information);
        msg->setText("Bilgi");
        msg->setInformativeText("Stok kartı silindi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->setModal(true);
        msg->show();
    }
    else{
        QMessageBox *msg = new QMessageBox();
        msg->setIcon(QMessageBox::Warning);
        msg->setText("Bilgi");
        msg->setInformativeText("Stok kartı silinemedi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->setModal(true);
        msg->show();
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
    sorgu.exec("SELECT grup FROM stokgruplari");
    QStringList liste;
    while (sorgu.next()) {
        liste.append(sorgu.value(0).toString());
    }
    return liste;
}

Sepet Veritabani::getSatis(QString _faturaNo)
{
    Sepet satis;
    QSqlQuery satisSorgu = QSqlQuery(db);
    satisSorgu.prepare("SELECT barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama FROM stokhareketleri WHERE islem_no = ?");
    satisSorgu.bindValue(0, _faturaNo);
    satisSorgu.exec();
    while (satisSorgu.next()) {
        StokKarti sk = getStokKarti(satisSorgu.value(0).toString());
        satis.urunEkle(sk, satisSorgu.value(3).toFloat());
    }
    return satis;
}

void Veritabani::kasadanParaCek(double _cekilecekTutar, User _kullanici)
{
    // kasada ki parayı güncelleme
    double kasadaKalanPara = _cekilecekTutar - getKasadakiPara();
    sorgu.prepare("UPDATE kasa SET para = ?");
    sorgu.bindValue(0, kasadaKalanPara);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qWarning() << "kasadanParaCek UPDATE hatası: " << sorgu.lastError().text();
    }
    //kasa hareketlerini girme
    sorgu.prepare("INSERT INTO kasahareketleri(id, miktar, kullanici, islem, tarih) VALUES (nextval('kasahareketleri_sequence'),?,?,?,?)");
    sorgu.bindValue(0, _cekilecekTutar);
    sorgu.bindValue(1, _kullanici.getUserName());
    sorgu.bindValue(2, "çıkış");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << "kasadanParaCek kasa hareketleri hatası: " << sorgu.lastError().text();
    }
}

