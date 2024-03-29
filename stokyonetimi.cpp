#include "sepet.h"
#include "stokyonetimi.h"

#include <QProcess>
#include <QStringList>

StokYonetimi::StokYonetimi()
{

}

StokYonetimi::~StokYonetimi()
{

}


bool StokYonetimi::barkodVarmi(const QString barkod)
{
    sorgu.prepare("SELECT barkod FROM stokkartlari WHERE barkod = ?");
    sorgu.bindValue(0, barkod);
    sorgu.exec();
    if(sorgu.next()){
        return true;
    }
    return false;
}

StokKarti StokYonetimi::getStokKarti(QString barkod)
{
    StokKarti kart = StokKarti();
    sorgu.prepare("SELECT stokkartlari.id, barkod, kod, ad, birim, miktar, grup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv, otv, kdvdahil, otvdahil, tarih, aciklama "
                    "FROM stokkartlari "
//                    "LEFT JOIN stokbirimleri ON stokkartlari.birim = stokbirimleri.id "
                    "WHERE barkod = ?");
    sorgu.bindValue(0, barkod);
    sorgu.exec();
    if(sorgu.next()){
        kart.setId(sorgu.value(0).toString());
        kart.setBarkod(sorgu.value(1).toString());
        kart.setKod(sorgu.value(2).toString());
        kart.setAd(sorgu.value(3).toString());
        kart.setBirim(sorgu.value(4).toInt());
        kart.setMiktar(sorgu.value(5).toFloat());
        kart.setGrup(sorgu.value(6).toInt());
        kart.setAFiyat(sorgu.value(7).toDouble());
        kart.setSFiyat(sorgu.value(8).toDouble());
        kart.setKdv(sorgu.value(9).toInt());
        kart.setOtv(sorgu.value(10).toInt());
        kart.setKdvdahil(sorgu.value(11).toBool());
        kart.setOtvdahil(sorgu.value(12).toBool());
        kart.setTarih(sorgu.value(13).toDateTime());
        kart.setAciklama(sorgu.value(14).toString());
    }
    else{
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    return kart;
}

bool StokYonetimi::setStokMiktari(const User kullanici, const StokKarti kart, StokHareketi hareket, float miktar)
{
//    bool cevap = false;
//    float mevcutStokMiktari = 0;
//    sorgu.prepare("SELECT miktar FROM stokkartlari WHERE id = ?");
//    sorgu.bindValue(0, kart.getId());
//    sorgu.exec();
//    if(sorgu.next()){
//        mevcutStokMiktari = sorgu.value(0).toFloat();
//    }
//    QString barkod = "";
//    sorgu.prepare("SELECT barkod FROM stokkartlari WHERE id = ?");
//    sorgu.bindValue(0, kart.getId());
//    sorgu.exec();
//    if(sorgu.next()){
//        barkod = sorgu.value(0).toString();
//    }
//    if(islem == "GİRİŞ"){
//        sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE id = ?");
//        sorgu.bindValue(0, mevcutStokMiktari + miktar);
//        sorgu.bindValue(1, kart.getId());
//        sorgu.exec();
//        if(sorgu.lastError().isValid()){
//            qDebug() << "stok kartı miktar ekleme/çıkarma hatası:\n" << sorgu.lastError().text();
//        }
//        stokHareketiEkle(kullanici, kart.getBarkod(), islem, miktar);
//        cevap = true;
//    }
//    else if(islem == "ÇIKIŞ"){
//        if(mevcutStokMiktari >= miktar){
//            sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE id = ?");
//            sorgu.bindValue(0, (mevcutStokMiktari - miktar));
//            sorgu.bindValue(1, kart.getId());
//            sorgu.exec();
//            if(sorgu.lastError().isValid()){
//                qDebug() << "stok kartı miktar ekleme/çıkarma hatası:\n" << sorgu.lastError().text();
//            }
//            stokHareketiEkle(kullanici, kart.getBarkod(), islem, miktar);
//            cevap = true;
//        }
//    }

    bool cevap = false;
    float mevcutStokMiktari = kart.getMiktar();

    switch (hareket) {
    case StokHareketi::Giris:
        sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE id = ?");
        sorgu.bindValue(0, mevcutStokMiktari + miktar);
        sorgu.bindValue(1, kart.getId());
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << "stok kartı miktar ekleme/çıkarma hatası:\n" << sorgu.lastError().text();
        }
        stokHareketiEkle(kullanici, kart.getBarkod(), hareket, miktar);
        cevap = true;
        break;
    case StokHareketi::Cikis:
        if(mevcutStokMiktari >= miktar){
            sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE id = ?");
            sorgu.bindValue(0, (mevcutStokMiktari - miktar));
            sorgu.bindValue(1, kart.getId());
            sorgu.exec();
            if(sorgu.lastError().isValid()){
                qDebug() << "stok kartı miktar ekleme/çıkarma hatası:\n" << sorgu.lastError().text();
            }
            stokHareketiEkle(kullanici, kart.getBarkod(), hareket, miktar);
            cevap = true;
        }
        break;
    case StokHareketi::Satis:
        break;
    case StokHareketi::Iade:
        break;
    }

    return cevap;
}

QSqlError StokYonetimi::yeniStokKartiOlustur(StokKarti stokKarti, User *kullanici)
{
    sorgu.prepare("INSERT INTO stokkartlari (id, barkod, kod, ad, birim, miktar, grup, afiyat, sfiyat, kdv, otv, kdvdahil, otvdahil, tarih, uretici, tedarikci, aciklama) "
                    "VALUES (nextval('stokkartlari_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, stokKarti.getBarkod());
    sorgu.bindValue(1, stokKarti.getKod());
    sorgu.bindValue(2, stokKarti.getAd());
    sorgu.bindValue(3, stokKarti.getBirim());
    sorgu.bindValue(4, stokKarti.getMiktar());
    sorgu.bindValue(5, stokKarti.getGrup());
    sorgu.bindValue(6, stokKarti.getAFiyat());
    sorgu.bindValue(7, stokKarti.getSFiyat());
    sorgu.bindValue(8, stokKarti.getKdv());
    sorgu.bindValue(9, stokKarti.getOtv());
    sorgu.bindValue(10, stokKarti.getKdvdahil());
    sorgu.bindValue(11, stokKarti.getOtvdahil());
    sorgu.bindValue(12, stokKarti.getTarih());
    sorgu.bindValue(13, stokKarti.getUretici().toInt());
    sorgu.bindValue(14, stokKarti.getTedarikci().toInt());
    sorgu.bindValue(15, stokKarti.getAciklama() + " [" + kullanici->getUserName() + "]");
    sorgu.exec();
    return sorgu.lastError();
}

QSqlError StokYonetimi::stokKartiniGuncelle(StokKarti duzenlenecekStokKarti, User *kullanici, bool fiyatDegistimi)
{
    // stok kartını güncelleme sorgusu/*
    // sorgu.prepare("UPDATE stokkartlari SET "
    //               "barkod = ?, "
    //               "kod = ?, "
    //               "ad = ?, "
    //               "birim = ?, "
    //               "miktar = ?, "
    //               "grup = ?, "
    //               "afiyat = ?, "
    //               "sfiyat = ?, "
    //               "kdv = ?, "
    //               "otv = ?, "
    //               "kdvdahil = ?, "
    //               "otvdahil = ?, "
    //               "tarih = ?, "
    //               "uretici = ?, "
    //               "tedarikci = ?, "
    //               "aciklama = ? "
    //               "WHERE id = ?;");
    // sorgu.bindValue(0, duzenlenecekStokKarti.getBarkod());
    // sorgu.bindValue(1, duzenlenecekStokKarti.getKod());
    // sorgu.bindValue(2, duzenlenecekStokKarti.getAd());
    // sorgu.bindValue(3, duzenlenecekStokKarti.getBirim());
    // sorgu.bindValue(4, duzenlenecekStokKarti.getMiktar());
    // sorgu.bindValue(5, duzenlenecekStokKarti.getGrup());
    // sorgu.bindValue(0, duzenlenecekStokKarti.getAFiyat());
    // sorgu.bindValue(1, duzenlenecekStokKarti.getSFiyat());
    // sorgu.bindValue(8, duzenlenecekStokKarti.getKdv());
    // sorgu.bindValue(9, duzenlenecekStokKarti.getOtv());
    // sorgu.bindValue(10, duzenlenecekStokKarti.getKdvdahil());
    // sorgu.bindValue(11, duzenlenecekStokKarti.getOtvdahil());
    // sorgu.bindValue(12, duzenlenecekStokKarti.getTarih());
    // sorgu.bindValue(13, duzenlenecekStokKarti.getUretici().toInt());
    // sorgu.bindValue(14, duzenlenecekStokKarti.getTedarikci().toInt());
    // sorgu.bindValue(15, duzenlenecekStokKarti.getAciklama() + " " + kullanici->getUserName());
    // sorgu.bindValue(2, duzenlenecekStokKarti.getId());
    sorgu.prepare("UPDATE stokkartlari SET afiyat = :afiyat, sfiyat = :sfiyat WHERE id = :id;");
    sorgu.bindValue(":afiyat", duzenlenecekStokKarti.getAFiyat());
    sorgu.bindValue(":sfiyat", duzenlenecekStokKarti.getSFiyat());
    sorgu.bindValue(":id", duzenlenecekStokKarti.getId());
    sorgu.exec();
    // qDebug() << sorgu.lastQuery();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }

    //fiyatı değişti ise carilere veresiye satışların tutarlarını düzeltme
    if(fiyatDegistimi){

        // cari hareketlerini güncel fiyatlardan gösterme için fatura tutarlarını düzenleme ( cari borç hesaplama güncel fiyattan ise)------------------------
        // güncel fiyattan hesaplanacak cariye yapılmış veresiye satışları alır
        sorgu.exec("SELECT fatura_no, toplamtutar, kalantutar FROM faturalar WHERE cari IN (SELECT id FROM carikartlar WHERE borc_hesaplama = 2) AND kalantutar NOT IN ('0') AND tipi = 2");
        if(sorgu.lastError().isValid()){
            qDebug() << qPrintable("güncellenen ürün veresiye satıldımı sorgu hatası: \n" + sorgu.lastError().text());
        }
        QStringList veresiyeFaturaNolar;
        QList<double> eskiToplamTutar;
        QList<double> eskiKalanTutar;
        while(sorgu.next()){
            veresiyeFaturaNolar.append(sorgu.value(0).toString());
            eskiToplamTutar.append(sorgu.value(1).toDouble());
            eskiKalanTutar.append(sorgu.value(2).toDouble());
        }
        for (int index = 0; index < veresiyeFaturaNolar.length(); ++index) {
            Sepet sepet = getSatis(veresiyeFaturaNolar.at(index));
            if(sepet.urunler.contains(duzenlenecekStokKarti.getBarkod())){
                sorgu.prepare("UPDATE faturalar SET toplamtutar = ?, kalantutar = ? WHERE fatura_no = ?");
                sorgu.bindValue(0, sepet.sepetToplamTutari());
                sorgu.bindValue(1, ((sepet.sepetToplamTutari() - eskiToplamTutar.at(index)) + eskiKalanTutar.at(index)));
                sorgu.bindValue(2, veresiyeFaturaNolar.at(index));
                sorgu.exec();
                if(sorgu.lastError().isValid()){
                    qDebug() << qPrintable("veresiyeTutarlariGuncelle() hatasi: \n" + sorgu.lastError().text());
                }
            }
        }
    }

    return sorgu.lastError();
}

Sepet StokYonetimi::getSatis(QString _faturaNo)
{
    Sepet satilmisSepet;
    QSqlQuery satisSorgu = QSqlQuery(db);// aşağıda while içinde ki satis.urunEkle() metodunda çağrılacak sorgu nesnesi ile karışmasın diye yeni query nesnesi oluşturdum.
    satisSorgu.prepare("SELECT barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, CAST(birim_f AS decimal), toplam_f, aciklama FROM stokhareketleri WHERE islem_no = ?");
    satisSorgu.bindValue(0, _faturaNo);
    satisSorgu.exec();
    while (satisSorgu.next()) {
        StokKarti sk = getStokKarti(satisSorgu.value(0).toString());
        satilmisSepet.urunEkle(sk, satisSorgu.value(3).toFloat()/*, sk.getSFiyat()*/);
    }
    // faturanın ödenen ve kalan tutar bilgisini alma.
    QSqlQuery islem = getIslemInfo(_faturaNo);
    satilmisSepet.setOdenenTutar(islem.value(4).toDouble());
    satilmisSepet.setKalanTutar(islem.value(5).toDouble());

    return satilmisSepet;
}

//Sepet StokYonetimi::getSatis(QString _faturaNo, Cari cari)// fatura yönetim classını stokyonetimi classına ekleyemediğim için bu metodu burayada kopyaladım.
//{
//    Sepet satilmisSepet;
//    QSqlQuery satisSorgu = QSqlQuery(db);// aşağıda while içinde ki satis.urunEkle() metodunda çağrılacak sorgu nesnesi ile karışmasın diye yeni query nesnesi oluşturdum.
//    satisSorgu.prepare("SELECT barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, CAST(birim_f AS decimal), toplam_f, aciklama FROM stokhareketleri WHERE islem_no = ?");
//    satisSorgu.bindValue(0, _faturaNo);
//    satisSorgu.exec();
//    if(cari.getGuncelBorcHesaplama()){
//        while (satisSorgu.next()) {
//            StokKarti sk = getStokKarti(satisSorgu.value(0).toString());
//            satilmisSepet.urunEkle(sk, satisSorgu.value(3).toFloat());
//            satilmisSepet.urunler[sk.getBarkod()].birimFiyat = satisSorgu.value(6).toDouble();
//        }
//    }
//    else{
//        while (satisSorgu.next()) {
//            StokKarti sk = getStokKarti(satisSorgu.value(0).toString());
//            satilmisSepet.urunEkle(sk, satisSorgu.value(3).toFloat(), satisSorgu.value(6).toDouble());
//        }
//    }
//    // faturanın ödenen ve kalan tutar bilgisini alma.
//    QSqlQuery islem = getIslemInfo(_faturaNo);
//    satilmisSepet.setOdenenTutar(islem.value(4).toDouble());
//    satilmisSepet.setKalanTutar(islem.value(5).toDouble());

//    return satilmisSepet;
//}

QSqlQuery StokYonetimi::getIslemInfo(QString _faturaNo)
{
    QSqlQuery islemSorgu = QSqlQuery(db);
    islemSorgu.prepare("SELECT id, fatura_no, kullanici, toplamtutar, odenentutar, kalantutar, tarih, tipi, cari FROM faturalar WHERE fatura_no = ?");
    islemSorgu.bindValue(0, _faturaNo);
    islemSorgu.exec();
    islemSorgu.next();
    return islemSorgu;
}

Cari StokYonetimi::getCariKart(QString cariID)
{
    Cari kart;
    sorgu.prepare("SELECT * FROM carikartlar WHERE id = ?");
    sorgu.bindValue(0, cariID);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    if(sorgu.next()){
        kart.setId(sorgu.value(0).toInt());
        kart.setAd(sorgu.value(1).toString());
        kart.setTip(sorgu.value(2).toInt());
        kart.setVerigino(sorgu.value(3).toString());
        kart.setVergiDaire(sorgu.value(4).toString());
        kart.setIl(sorgu.value(5).toString());
        kart.setIlce(sorgu.value(6).toString());
        kart.setAdres(sorgu.value(7).toString());
        kart.setMail(sorgu.value(8).toString());
        kart.setTelefon(sorgu.value(9).toString());
        kart.setTarih(sorgu.value(10).toDateTime());
        kart.setAciklama(sorgu.value(11).toString());
        kart.setYetkili(sorgu.value(12).toString());
        if(sorgu.value(13).toInt() == 2){// güncel fiyattan hesaplanacak ise
            kart.setGuncelBorcHesaplama(true);
        }
        else{
            kart.setGuncelBorcHesaplama(false);
        }
        return kart;
    }
    else{
        return kart;
    }
}

bool StokYonetimi::stokKartiSil(QString stokKartiID)
{
    sorgu.prepare("DELETE FROM stokkartlari WHERE id = ?");
    sorgu.bindValue(0, stokKartiID);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        return false;
    }
    return true;
}

void StokYonetimi::stokHareketiEkle(User kullanici, QString barkod, StokHareketi hareket, float miktar)
{
    sorgu.prepare("INSERT INTO stokhareketleri(barkod, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                    "VALUES (?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, barkod);
    sorgu.bindValue(1, hareket);
    sorgu.bindValue(2, miktar);
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, kullanici.getUserID());
    sorgu.bindValue(5, "STOK GİRİŞİ");
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
}

QSqlQueryModel *StokYonetimi::getStokKartlari()
{
    stokKartlariModel->setQuery("SELECT stokkartlari.id, barkod, kod, stokkartlari.ad, stokbirimleri.birim, miktar, stokgruplari.grup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv, otv, kdvdahil, otvdahil, stokkartlari.tarih, ureticiler.ad, carikartlar.ad, stokkartlari.aciklama FROM stokkartlari "
                                "LEFT JOIN stokbirimleri ON stokkartlari.birim = stokbirimleri.id "
                                "LEFT JOIN ureticiler ON stokkartlari.uretici = ureticiler.id "
                                "LEFT JOIN carikartlar ON stokkartlari.tedarikci = carikartlar.id "
                                "LEFT JOIN stokgruplari ON stokkartlari.grup = stokgruplari.id "
                                "ORDER BY stokkartlari.ad ASC", db);
    stokKartlariModel->setHeaderData(0, Qt::Horizontal, "ID");
    stokKartlariModel->setHeaderData(1, Qt::Horizontal, "Barkod");
    stokKartlariModel->setHeaderData(2, Qt::Horizontal, "Stok Kodu");
    stokKartlariModel->setHeaderData(3, Qt::Horizontal, "Adı");
    stokKartlariModel->setHeaderData(4, Qt::Horizontal, "Birim");
    stokKartlariModel->setHeaderData(5, Qt::Horizontal, "Miktar");
    stokKartlariModel->setHeaderData(6, Qt::Horizontal, "Stok Grup");
    stokKartlariModel->setHeaderData(7, Qt::Horizontal, "Alış Fiyat");
    stokKartlariModel->setHeaderData(8, Qt::Horizontal, "Satış Fiyat");
    stokKartlariModel->setHeaderData(9, Qt::Horizontal, "KDV");
    stokKartlariModel->setHeaderData(10, Qt::Horizontal, "ÖTV");
    stokKartlariModel->setHeaderData(11, Qt::Horizontal, "KDV dahil");
    stokKartlariModel->setHeaderData(12, Qt::Horizontal, "ÖTV dahil");
    stokKartlariModel->setHeaderData(13, Qt::Horizontal, "Gün. tarihi");
    stokKartlariModel->setHeaderData(14, Qt::Horizontal, "Üretici");
    stokKartlariModel->setHeaderData(15, Qt::Horizontal, "Tedarikçi");
    stokKartlariModel->setHeaderData(16, Qt::Horizontal, "Açıklama");
    return stokKartlariModel;
}

QSqlQueryModel *StokYonetimi::getStokKartlari(QString grupAdi)
{
    int grupID = getGrupID(grupAdi);

    QSqlQuery ququ = QSqlQuery(db);
    ququ.prepare("SELECT stokkartlari.id, barkod, kod, stokkartlari.ad, stokbirimleri.birim, miktar, stokgruplari.grup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv, otv, kdvdahil, otvdahil, stokkartlari.tarih, ureticiler.ad, carikartlar.ad, stokkartlari.aciklama FROM stokkartlari "
                 "LEFT JOIN stokbirimleri ON stokkartlari.birim = stokbirimleri.id "
                 "LEFT JOIN ureticiler ON stokkartlari.uretici = ureticiler.id "
                 "LEFT JOIN carikartlar ON stokkartlari.tedarikci = carikartlar.id "
                 "LEFT JOIN stokgruplari ON stokkartlari.grup = stokgruplari.id "
                 "WHERE stokkartlari.grup = ? "
                 "ORDER BY stokkartlari.ad ASC");
    ququ.bindValue(0, grupID);
    ququ.exec();
    stokKartlariModel->setQuery(ququ);

    // eski *****************************************
//     stokKartlariModel->setQuery("SELECT stokkartlari.id, barkod, kod, stokkartlari.ad, stokbirimleri.birim, miktar, stokgruplari.grup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv, otv, kdvdahil, otvdahil, stokkartlari.tarih, ureticiler.ad, carikartlar.ad, stokkartlari.aciklama FROM stokkartlari "
//                                "LEFT JOIN stokbirimleri ON stokkartlari.birim = stokbirimleri.id "
//                                "LEFT JOIN ureticiler ON stokkartlari.uretici = ureticiler.id "
//                                "LEFT JOIN carikartlar ON stokkartlari.tedarikci = carikartlar.id "
//                                "LEFT JOIN stokgruplari ON stokkartlari.grup = stokgruplari.id "
//                                "ORDER BY stokkartlari.ad ASC", db);
    // eski *****************************************

    stokKartlariModel->setHeaderData(0, Qt::Horizontal, "ID");
    stokKartlariModel->setHeaderData(1, Qt::Horizontal, "Barkod");
    stokKartlariModel->setHeaderData(2, Qt::Horizontal, "Stok Kodu");
    stokKartlariModel->setHeaderData(3, Qt::Horizontal, "Adı");
    stokKartlariModel->setHeaderData(4, Qt::Horizontal, "Birim");
    stokKartlariModel->setHeaderData(5, Qt::Horizontal, "Miktar");
    stokKartlariModel->setHeaderData(6, Qt::Horizontal, "Stok Grup");
    stokKartlariModel->setHeaderData(7, Qt::Horizontal, "Alış Fiyat");
    stokKartlariModel->setHeaderData(8, Qt::Horizontal, "Satış Fiyat");
    stokKartlariModel->setHeaderData(9, Qt::Horizontal, "KDV");
    stokKartlariModel->setHeaderData(10, Qt::Horizontal, "ÖTV");
    stokKartlariModel->setHeaderData(11, Qt::Horizontal, "KDV dahil");
    stokKartlariModel->setHeaderData(12, Qt::Horizontal, "ÖTV dahil");
    stokKartlariModel->setHeaderData(13, Qt::Horizontal, "Gün. tarihi");
    stokKartlariModel->setHeaderData(14, Qt::Horizontal, "Üretici");
    stokKartlariModel->setHeaderData(15, Qt::Horizontal, "Tedarikçi");
    stokKartlariModel->setHeaderData(16, Qt::Horizontal, "Açıklama");
    return stokKartlariModel;
}

QSqlQueryModel *StokYonetimi::getStokKartlari(QSqlQuery query)
{
    stokKartlariModel->setQuery(query);
    stokKartlariModel->setHeaderData(0, Qt::Horizontal, "Kod");
    stokKartlariModel->setHeaderData(1, Qt::Horizontal, "Barkod");
    stokKartlariModel->setHeaderData(2, Qt::Horizontal, "Ürün Adı");
    stokKartlariModel->setHeaderData(3, Qt::Horizontal, "Satış Fiyatı");
    return stokKartlariModel;
}

QSqlQueryModel *StokYonetimi::getStokKartlariEtiket()
{
    sorgu.prepare("SELECT barkod, ad, sfiyat, tarih FROM stokkartlari ORDER BY tarih DESC");
    sorgu.exec();
    stokKartlariModel->setQuery(sorgu);
    stokKartlariModel->setHeaderData(0, Qt::Horizontal, "Barkod");
    stokKartlariModel->setHeaderData(1, Qt::Horizontal, "Ürün Adı");
    stokKartlariModel->setHeaderData(2, Qt::Horizontal, "Satış Fiyatı");
    stokKartlariModel->setHeaderData(3, Qt::Horizontal, "Güncelleme Tarihi");
    if(stokKartlariModel->lastError().isValid()){
        qDebug() << qPrintable(stokKartlariModel->lastError().text());
    }
    return stokKartlariModel;
}

QStringList StokYonetimi::stokGruplariGetir()
{
    sorgu.exec("SELECT grup FROM stokgruplari");
    QStringList liste;
    while (sorgu.next()) {
        liste.append(sorgu.value(0).toString());
    }
    return liste;
}

int StokYonetimi::getGrupID(QString pGrup)
{
    sorgu.prepare("SELECT id FROM stokgruplari WHERE grup = ?");
    sorgu.bindValue(0, pGrup);
    sorgu.exec();
    if(sorgu.next()){
        return sorgu.value(0).toInt();
    }
    return 0;
}

QSqlQueryModel *StokYonetimi::getStokHareketleri(QString barkod)
{
    sorgu.prepare("SELECT * FROM stokhareketleri WHERE barkod = ?");
    sorgu.bindValue(0, barkod);
    sorgu.exec();
    stokHareketleriModel->setQuery(sorgu);
    return stokHareketleriModel;
}

QSqlQueryModel *StokYonetimi::getStokHareketleri(QString barkod, QDateTime baslangicTarih, QDateTime bitisTarih)
{
    sorgu.prepare("SELECT * FROM stokhareketleri WHERE barkod = ? AND tarih BETWEEN ? AND ? ORDER BY tarih DESC");
    sorgu.bindValue(0, barkod);
    sorgu.bindValue(1, baslangicTarih);
    sorgu.bindValue(2, bitisTarih.addDays(1));
    sorgu.exec();
    stokHareketleriModel->setQuery(sorgu);
    return stokHareketleriModel;
}

void StokYonetimi::stokGrupEkle(QString eklenecekGrupAdi)
{
    sorgu.prepare("INSERT INTO stokgruplari(id, grup) "
                    "VALUES (nextval('stokgruplari_id_seq'), ?)");
    sorgu.bindValue(0, eklenecekGrupAdi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qWarning() << "stok grup ekleme sorgu INSERT hatası: " << sorgu.lastError().text();
    }
}

void StokYonetimi::stokGrupSil(QString silinecekGrupAdi)
{
    sorgu.prepare("DELETE FROM stokgruplari WHERE grup = ?");
    sorgu.bindValue(0, silinecekGrupAdi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qWarning() << "stok grup silme sorgu DELETE hatası: " << sorgu.lastError().text();
    }
}

void StokYonetimi::stokBirimEkle(QString birim)
{
    sorgu.prepare("INSERT INTO stokbirimleri(id, birim) VALUES(nextval('stokbirimleri_sequence'), ?)");
    sorgu.bindValue(0, birim);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
}

void StokYonetimi::stokBirimSil(QString birim)
{
    sorgu.prepare("DELETE FROM stokbirimleri WHERE birim = ?");
    sorgu.bindValue(0, birim);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << "stok birim silme hatası: \n" << sorgu.lastError().text();
    }
}

QStringList StokYonetimi::getStokBirimleri()
{
    QStringList stokbirimleri;
    stokbirimleri.clear();
    stokbirimleri.append("Birim Seçin...");
    sorgu.exec("SELECT * FROM stokbirimleri");
    while (sorgu.next()) {
        stokbirimleri.append(sorgu.value(1).toString());
    }
    return stokbirimleri;
}

int StokYonetimi::getBirimID(QString pBirim)
{
    sorgu.prepare("SELECT id FROM stokbirimleri WHERE birim = ?");
    sorgu.bindValue(0, pBirim);
    sorgu.exec();
    if(sorgu.next()){
        return sorgu.value(0).toInt();
    }
    return 0;
}

QString StokYonetimi::getBirimAd(int birimID)
{
    sorgu.prepare("SELECT birim FROM stokbirimleri WHERE id = ?");
    sorgu.bindValue(0, birimID);
    sorgu.exec();
    if(sorgu.next()){
        return sorgu.value(0).toString();
    }
    else{
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    return QString();
}

QHash<QString, float> StokYonetimi::getgunlukAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart)
{
    sonTarih = sonTarih.addDays(1);// bir gün eklemezsem saati 00:00:00 aldığı için 1 gün eksik hesaplıyor.

    sorgu.prepare("SELECT SUM(islem_miktari), DATE_TRUNC('day', tarih::timestamp) FROM stokhareketleri "
                  "WHERE tarih BETWEEN ? AND ? AND barkod = ? "
                  "GROUP BY DATE_TRUNC('day', tarih::timestamp) ORDER BY DATE_TRUNC('day', tarih::timestamp)");
    sorgu.bindValue(0, ilkTarih);
    sorgu.bindValue(1, sonTarih);
    sorgu.bindValue(2, kart.getBarkod());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    QHash<QString, float> adetler;
    while (sorgu.next()) {
        adetler.insert(sorgu.value(1).toDate().toString(), sorgu.value(0).toFloat());
    }
    return adetler;
}

QHash<QString, float> StokYonetimi::getAylikAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart)
{
    sonTarih = sonTarih.addMonths(1);// bir ay eklemezsem saati 00:00:00 aldığı için 1 ay eksik hesaplıyor.

    sorgu.prepare("SELECT SUM(islem_miktari), DATE_TRUNC('month', tarih::timestamp) FROM stokhareketleri "
                  "WHERE tarih BETWEEN ? AND ? AND barkod = ? "
                  "GROUP BY DATE_TRUNC('month', tarih::timestamp) ORDER BY DATE_TRUNC('month', tarih::timestamp)");
    sorgu.bindValue(0, ilkTarih);
    sorgu.bindValue(1, sonTarih);
    sorgu.bindValue(2, kart.getBarkod());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    QHash<QString, float> adetler;
    while (sorgu.next()) {
        adetler.insert(sorgu.value(1).toDate().toString("MM.yyyy MMMM"), sorgu.value(0).toFloat());
    }
    return adetler;
}

QHash<QString, float> StokYonetimi::getYillikAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart)
{
    sonTarih = sonTarih.addYears(1);// bir yıl eklemezsem saati 00:00:00 aldığı için 1 yıl eksik hesaplıyor.

    sorgu.prepare("SELECT SUM(islem_miktari), DATE_TRUNC('year', tarih::timestamp) FROM stokhareketleri "
                  "WHERE tarih BETWEEN ? AND ? AND barkod = ? "
                  "GROUP BY DATE_TRUNC('year', tarih::timestamp) ORDER BY DATE_TRUNC('year', tarih::timestamp)");
    sorgu.bindValue(0, ilkTarih);
    sorgu.bindValue(1, sonTarih);
    sorgu.bindValue(2, kart.getBarkod());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    QHash<QString, float> adetler;
    while (sorgu.next()) {
        adetler.insert(sorgu.value(1).toDate().toString("yyyy"), sorgu.value(0).toFloat());
    }
    return adetler;
}

QStringList StokYonetimi::getUreticiler()
{
    QStringList liste;
    liste.append("Üretici seçin...");
    sorgu.exec("SELECT ad FROM ureticiler ORDER BY ad ASC");
    while (sorgu.next()) {
        liste.append(sorgu.value(0).toString());
    }
    return liste;
}

int StokYonetimi::getUreticiID(QString ureticiAd)
{
    sorgu.prepare("SELECT id FROM ureticiler WHERE ad = ?");
    sorgu.bindValue(0, ureticiAd);
    sorgu.exec();
    if(!sorgu.next()){
        return 0;
    }
    return sorgu.value(0).toInt();
}

QString StokYonetimi::getUreticiAD(int ID)
{
    sorgu.prepare("SELECT ad FROM ureticiler WHERE id = ?");
    sorgu.bindValue(0, ID);
    sorgu.exec();
    if(sorgu.lastError().isValid() || !sorgu.next()){
        return 0;
    }
    return sorgu.value(0).toString();
}

QStringList StokYonetimi::getTedarikciler()
{
    QStringList liste;
    liste.append("Tedarikçi seçin...");
    sorgu.exec("SELECT ad FROM carikartlar WHERE tip = 2 ORDER BY ad ASC");
    while (sorgu.next()) {
        liste.append(sorgu.value(0).toString());
    }
    return liste;
}

int StokYonetimi::getTedarikciID(QString tedarikciAd)
{
    sorgu.prepare("SELECT id FROM carikartlar WHERE tip = 2 AND ad = ?");
    sorgu.bindValue(0, tedarikciAd);
    sorgu.exec();
    if(!sorgu.next()){
        return 0;
    }
    return sorgu.value(0).toInt();
}

bool StokYonetimi::csvAktar(QString dosyaYolu)
{
    QFile csvDosya(dosyaYolu);
    csvDosya.open(QIODevice::WriteOnly);
    csvDosya.setPermissions(QFileDevice::WriteUser | QFileDevice::ReadUser);
    QString cmd = QString("psql -U postgres -d mhss_data -c \"copy (SELECT barkod, kod, ad, birim, cast (miktar as float), grup, cast (afiyat as decimal), cast (sfiyat as decimal), kdv, kdvdahil, otv, otvdahil, mensei FROM stokkartlari) TO STDIN with CSV HEADER DELIMITER ',';\" > ") + dosyaYolu;
//    qDebug() << "csv aktarma komutu: " << qPrintable(cmd);
    int exitCode = system(qPrintable(cmd));
    if(exitCode != QProcess::NormalExit){
        return false;
    }
    return true;
}
