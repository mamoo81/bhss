#include "kasayonetimi.h"

#include <QMessageBox>
#include <QDebug>

KasaYonetimi::KasaYonetimi()
{

}

KasaYonetimi::~KasaYonetimi()
{

}

double KasaYonetimi::getKasaToplamGiren(QDateTime baslangicTarih, QDateTime bitisTarih)
{
    sorgu.prepare("SELECT SUM(CAST(miktar AS DECIMAL)) FROM kasahareketleri WHERE islem = 1 AND tarih BETWEEN ? AND ?"); // islem = 1 = GİRİŞ
    sorgu.bindValue(0, baslangicTarih);
    sorgu.bindValue(1, bitisTarih);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
        return 0;
    }
    else{
        if(sorgu.next()){
            return sorgu.value(0).toDouble();
        }
        else{
            return 0;
        }
    }
}

double KasaYonetimi::getKasaToplamCikan(QDateTime baslangicTarih, QDateTime bitisTarih)
{
    sorgu.prepare("SELECT SUM(CAST(miktar AS DECIMAL)) FROM kasahareketleri WHERE islem IN ('2','4') AND tarih BETWEEN ? AND ?");
    sorgu.bindValue(0, baslangicTarih);
    sorgu.bindValue(1, bitisTarih);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
        return 0;
    }
    else{
        if(sorgu.next()){
            return sorgu.value(0).toDouble();
        }
        else{
            return 0;
        }
    }
}

QSqlQueryModel *KasaYonetimi::getKasaHareketleri(QDateTime baslangicTarih, QDateTime bitisTarih)
{
    sorgu.prepare("SELECT kasahareketleri.id, kasahareketadlari.ad, CAST(miktar AS DECIMAL), kasahareketleri.tarih, kullanicilar.username, evrakno, kasahareketleri.aciklama FROM kasahareketleri "
                    "INNER JOIN kullanicilar ON kasahareketleri.kullanici = kullanicilar.id "
                    "INNER JOIN kasahareketadlari ON kasahareketleri.islem = kasahareketadlari.id "
                    "WHERE kasahareketleri.tarih BETWEEN ? AND ? "
                    "ORDER BY kasahareketleri.id DESC");
    sorgu.bindValue(0, baslangicTarih);
    sorgu.bindValue(1, bitisTarih);
    sorgu.exec();
    kasaHareketlerimodel->clear();
    kasaHareketlerimodel->setQuery(sorgu);
    kasaHareketlerimodel->setHeaderData(0, Qt::Horizontal, "ID");
    kasaHareketlerimodel->setHeaderData(1, Qt::Horizontal, "Hareket");
    kasaHareketlerimodel->setHeaderData(2, Qt::Horizontal, "Miktar");
    kasaHareketlerimodel->setHeaderData(3, Qt::Horizontal, "Tarih");
    kasaHareketlerimodel->setHeaderData(4, Qt::Horizontal, "Kullanici");
    kasaHareketlerimodel->setHeaderData(5, Qt::Horizontal, "Evrak No");
    kasaHareketlerimodel->setHeaderData(6, Qt::Horizontal, "Açıklama");
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
        return NULL;
    }
    else{
        return kasaHareketlerimodel;
    }
}

double KasaYonetimi::getNetKar(QDateTime baslangicTarih, QDateTime bitisTarih)
{
    sorgu.prepare("SELECT SUM(CAST(kar AS DECIMAL)) FROM kasahareketleri WHERE tarih BETWEEN ? AND ?");
    sorgu.bindValue(0, baslangicTarih);
    sorgu.bindValue(1, bitisTarih);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
        return 0;
    }
    else{
        if(sorgu.next()){
            return sorgu.value(0).toDouble();
        }
        else{
            return 0;
        }
    }
}

int KasaYonetimi::KasaHareketiEkle(User user, KasaHareketi hareket, double tutar, QString aciklama, QDateTime tarih, QString evrakno, double netKar)
{
    // KASAYA PARA GİRİŞ/ÇIKIŞ İŞLEMİ BAŞLANGIÇ
    sorgu.exec("select para FROM kasa");
    double mevcutPara = 0;
    double sonPara = 0;
    if(sorgu.next()){
        mevcutPara = sorgu.value(0).toDouble();

        switch (hareket) {
        case KasaHareketi::Giris:
            sonPara = mevcutPara + tutar;
            sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
            sorgu.bindValue(0, sonPara);
            sorgu.exec();
            if(sorgu.lastError().isValid()){
                qDebug() << "kasahareketiekle() hata:\n" << qPrintable(sorgu.lastError().text());
            }
            break;
        case KasaHareketi::Cikis:
            sonPara = mevcutPara - tutar;
            if(sonPara < 0){//KASADAKİ PARA EKSİ DEĞERE DÜŞMESİN
                QMessageBox msg(0);
                msg.setWindowTitle("Dikkat");
                msg.setIcon(QMessageBox::Warning);
                msg.setText("Kasada ki miktardan fazla çıkış yapamazsınız!");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
                return 0;
            }
            else{
                sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
                sorgu.bindValue(0, sonPara);
                sorgu.exec();
                if(sorgu.lastError().isValid()){
                    qDebug() << "kasahareketiekle() hata:\n" << qPrintable(sorgu.lastError().text());
                }
            }
            break;
        case KasaHareketi::Satis:
            break;
        case KasaHareketi::Iade:
            break;
        case KasaHareketi::BankaVirman:
            break;
        }
        //HAREKET EKLEME BAŞLANGICI
        sorgu.prepare("INSERT INTO kasahareketleri (id, miktar, kullanici, islem, tarih, kar, evrakno, aciklama) "
                        "VALUES (nextval('kasahareketleri_sequence'), ?, ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, tutar);
        sorgu.bindValue(1, user.getUserID().toInt());
        sorgu.bindValue(2, hareket);
        sorgu.bindValue(3, tarih);
        sorgu.bindValue(4, netKar);
        sorgu.bindValue(5, evrakno);
        sorgu.bindValue(6, aciklama);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << qPrintable(sorgu.lastError().text());
        }
        QMessageBox msg(0);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        if(hareket == KasaHareketi::Giris){// hareket giriş ise
            msg.setText("Kasaya " + QString::number(tutar, 'f', 2) + " TL giriş yapıldı.");
        }
        else if(hareket == KasaHareketi::Cikis){// hareket çıkış ise
            msg.setText("Kasadan " + QString::number(tutar, 'f', 2) + " TL çıkış yapıldı.");
        }
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        return 1;
    }
    return 0;
}

int KasaYonetimi::KasaHareketiEkle(User user, KasaHareketi hareket, double tutar, QString aciklama, QDateTime tarih, double netKar)
{
    // KASAYA PARA GİRİŞ/ÇIKIŞ İŞLEMİ BAŞLANGIÇ
    sorgu.exec("select para FROM kasa");
    double mevcutPara = 0;
    double sonPara = 0;
    if(sorgu.next()){
        mevcutPara = sorgu.value(0).toDouble();

        switch (hareket) {
        case KasaHareketi::Giris:
            sonPara = mevcutPara + tutar;
            sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
            sorgu.bindValue(0, sonPara);
            sorgu.exec();
            if(sorgu.lastError().isValid()){
                qDebug() << "kasahareketiekle() hata:\n" << qPrintable(sorgu.lastError().text());
            }
            break;
        case KasaHareketi::Cikis:
            sonPara = mevcutPara - tutar;
            sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
            sorgu.bindValue(0, sonPara);
            sorgu.exec();
            if(sorgu.lastError().isValid()){
                qDebug() << "kasahareketiekle() hata:\n" << qPrintable(sorgu.lastError().text());
            }
            break;
        case KasaHareketi::Satis:
            break;
        case KasaHareketi::Iade:
            break;
        case KasaHareketi::BankaVirman:
            break;
        }

        //HAREKET EKLEME BAŞLANGICI
        sorgu.prepare("INSERT INTO kasahareketleri (id, miktar, kullanici, islem, tarih, kar, aciklama) "
                        "VALUES (nextval('kasahareketleri_sequence'), ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, tutar);
        sorgu.bindValue(1, user.getUserID().toInt());
        sorgu.bindValue(2, hareket);
        sorgu.bindValue(3, tarih);
        switch (hareket) {
        case KasaHareketi::Giris:
            sorgu.bindValue(4, netKar);
            break;
        case KasaHareketi::Cikis:
            sorgu.bindValue(4, (netKar * -1));
            break;
        case KasaHareketi::Satis:
            break;
        case KasaHareketi::Iade:
            break;
        case KasaHareketi::BankaVirman:
            break;
        }
        sorgu.bindValue(5, aciklama);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << qPrintable(sorgu.lastError().text());
        }
    }
    return 1;
}

int KasaYonetimi::kasaHareketiDuzenle(User user, QString hareketID, KasaHareketi hareket, double tutar, QString aciklama, QDateTime tarih, QString evrakNo)
{
    //KASA HAREKETİNİ DÜZELTMEDEN ÖNCE, düzeltilecek işlemin şuan ki HAREKET TUTARINI AL. metodun EN SON da +/- YÖNDE kasadaki paraya ekle
    sorgu.prepare("SELECT islem, CAST(miktar AS DECIMAL) FROM kasahareketleri WHERE id = ?");
    sorgu.bindValue(0, hareketID);
    sorgu.exec();
    double oncekiTutar = 0;
    KasaHareketi oncekiHareket = KasaHareketi::Giris;
    if(sorgu.next()){
        oncekiTutar = sorgu.value(1).toDouble();
        oncekiHareket = enumFromString(sorgu.value(0).toString());
    }
    //----------------------------------------
    //KASA HAREKETLERİ TABLOSUNU DÜZELTME
    sorgu.prepare("UPDATE kasahareketleri "
                    "SET miktar = ?, kullanici = ?, islem = ?, tarih = ?, evrakno = ?, aciklama = ?");
    sorgu.bindValue(0, tutar);
    sorgu.bindValue(1, user.getUserID());
    sorgu.bindValue(2, hareket);
    sorgu.bindValue(3, tarih);
    sorgu.bindValue(4, evrakNo);
    sorgu.bindValue(5, "İŞLEM DÜZELTME:" + aciklama);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    // DÜZELTME YAPILACAK HAREKET DEĞİŞTİ İSE TUTARI KASAda ki paraya EKLE/ÇIKAR
    if(hareket != oncekiHareket){
        switch (oncekiHareket) {
            case KasaHareketi::Giris:{
                double suankiPara = getKasadakiPara();
                double guncelPara = suankiPara - oncekiTutar;
                sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
                sorgu.bindValue(0, guncelPara);
                sorgu.exec();
                guncelPara = getKasadakiPara();
                guncelPara -= tutar;
                sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
                sorgu.bindValue(0, guncelPara);
                sorgu.exec();
                break;
            }
            case KasaHareketi::Cikis:{
                double suankiPara = getKasadakiPara();
                double guncelPara = suankiPara + oncekiTutar;
                sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
                sorgu.bindValue(0, guncelPara);
                sorgu.exec();
                guncelPara = getKasadakiPara();
                guncelPara += tutar;
                sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
                sorgu.bindValue(0, guncelPara);
                sorgu.exec();
                break;
            }
            case KasaHareketi::Satis:{
                break;
            }
            case KasaHareketi::Iade:{
                break;
            }
            case KasaHareketi::BankaVirman:{
                break;
            }
        }
    }
    return 1;
}

bool KasaYonetimi::kasaHareketiSil(QString hareketID, QString hareket, double tutar)
{
    // kasahareketleri tablosundan kayıdı silme
    sorgu.prepare("DELETE FROM kasahareketleri WHERE id = ?");
    sorgu.bindValue(0, hareketID);
    sorgu.exec();
    //kasadan silinen işlemin parasını düşme.
    if(hareket == "GİRİŞ"){
        double suankiPara = getKasadakiPara();
        double guncelPara = suankiPara - tutar;
        sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
        sorgu.bindValue(0, guncelPara);
        sorgu.exec();
    }
    else if(hareket == "ÇIKIŞ"){
        double suankiPara = getKasadakiPara();
        double guncelPara = suankiPara + tutar;
        sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
        sorgu.bindValue(0, guncelPara);
        sorgu.exec();
    }
    return true;
}

void KasaYonetimi::kasadanParaCek(double cekilecekTutar, User kullanici)
{
    // kasada ki parayı güncelleme
    double kasadaKalanPara = getKasadakiPara() - cekilecekTutar;
    sorgu.prepare("UPDATE kasa SET para = ?");
    sorgu.bindValue(0, kasadaKalanPara);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qWarning() << "kasadanParaCek UPDATE hatası:\n" << qPrintable(sorgu.lastError().text());
    }
    //kasa hareketlerini girme
    sorgu.prepare("INSERT INTO kasahareketleri(id, miktar, kullanici, islem, tarih) VALUES (nextval('kasahareketleri_sequence'),?,?,?,?)");
    sorgu.bindValue(0, cekilecekTutar);
    sorgu.bindValue(1, kullanici.getUserID());
    sorgu.bindValue(2, "ÇIKIŞ");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << "kasadanParaCek kasa hareketleri hatası:\n" << qPrintable(sorgu.lastError().text());
    }
}

double KasaYonetimi::getKasadakiPara()
{
    sorgu.exec("SELECT * FROM kasa");
    sorgu.next();
    if(sorgu.lastError().isValid()){
        return 0;
    }
    return sorgu.value(1).toDouble();
}

double KasaYonetimi::getGunlukCiro()
{
    sorgu.prepare("SELECT SUM(miktar) FROM kasahareketleri WHERE tarih > ? AND islem IN(?,?)");
    sorgu.bindValue(0, QDateTime::currentDateTime().date());
    sorgu.bindValue(1, "GİRİŞ");
    sorgu.bindValue(2, "İADE");
    sorgu.exec();
    sorgu.next();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
        return 0;
    }
    return sorgu.value(0).toDouble();
}

QString KasaYonetimi::enumToString(KasaYonetimi::KasaHareketi value)
{
    switch (value) {
    case KasaYonetimi::KasaHareketi::Giris:
        return QString("GİRİŞ");
        break;
    case KasaYonetimi::KasaHareketi::Cikis:
        return QString("ÇIKIŞ");
        break;
    case KasaYonetimi::KasaHareketi::Satis:
        return QString("SATIŞ");
        break;
    case KasaYonetimi::KasaHareketi::Iade:
        return QString("İADE");
        break;
    default:
        return QString();
        break;
    }
}

KasaYonetimi::KasaHareketi KasaYonetimi::enumFromString(QString value)
{
    if(value == "Giris"){
        return KasaHareketi::Giris;
    }
    else if(value == "Cikis"){
        return KasaHareketi::Cikis;
    }
    else if(value == "Satis"){
        return KasaHareketi::Satis;
    }
    else if(value == "Iade"){
        return KasaHareketi::Iade;
    }
    else if(value == "BankaVirman"){
        return KasaHareketi::BankaVirman;
    }
}
