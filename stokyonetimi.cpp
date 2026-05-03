#include "sepet.h"
#include "stokyonetimi.h"

#include <QProcess>
#include <QStringList>
#include <QList>

StokYonetimi::StokYonetimi()
{

}

StokYonetimi::~StokYonetimi()
{

}


bool StokYonetimi::barkodVarmi(const QString barkod)
{
    QSqlQuery query(db);
    query.prepare("SELECT barkod FROM stokkartlari WHERE barkod = ?");
    query.bindValue(0, barkod);
    query.exec();
    if(query.next()){
        return true;
    }
    return false;
}

StokKarti StokYonetimi::getStokKarti(QString barkod)
{
    QSqlQuery query(db);
    StokKarti kart = StokKarti();
    QString sql = QString("SELECT stokkartlari.id, barkod, kod, ad, birim, miktar, grup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv, otv, kdvdahil, otvdahil, tarih, aciklama "
                          "FROM stokkartlari "
                          "WHERE barkod = '%1'")
                      .arg(barkod.replace("'", "''"));
    query.exec(sql);
    if(query.next()){
        kart.setId(query.value(0).toString());
        kart.setBarkod(query.value(1).toString());
        kart.setKod(query.value(2).toString());
        kart.setAd(query.value(3).toString());
        kart.setBirim(query.value(4).toInt());
        kart.setMiktar(query.value(5).toFloat());
        kart.setGrup(query.value(6).toInt());
        kart.setAFiyat(query.value(7).toDouble());
        kart.setSFiyat(query.value(8).toDouble());
        kart.setKdv(query.value(9).toInt());
        kart.setOtv(query.value(10).toInt());
        kart.setKdvdahil(query.value(11).toBool());
        kart.setOtvdahil(query.value(12).toBool());
        kart.setTarih(query.value(13).toDateTime());
        kart.setAciklama(query.value(14).toString());
    }
    else{
        qDebug() << qPrintable(query.lastError().text());
    }
    return kart;
}

bool StokYonetimi::setStokMiktari(const User kullanici, const StokKarti kart, StokHareketi hareket, float miktar)
{
    QSqlQuery query(db);
    Q_UNUSED(kullanici)
    //    bool cevap = false;
//    float mevcutStokMiktari = 0;
//    query.prepare("SELECT miktar FROM stokkartlari WHERE id = ?");
//    query.bindValue(0, kart.getId());
//    query.exec();
//    if(query.next()){
//        mevcutStokMiktari = query.value(0).toFloat();
//    }
//    QString barkod = "";
//    query.prepare("SELECT barkod FROM stokkartlari WHERE id = ?");
//    query.bindValue(0, kart.getId());
//    query.exec();
//    if(query.next()){
//        barkod = query.value(0).toString();
//    }
//    if(islem == "GİRİŞ"){
//        query.prepare("UPDATE stokkartlari SET miktar = ? WHERE id = ?");
//        query.bindValue(0, mevcutStokMiktari + miktar);
//        query.bindValue(1, kart.getId());
//        query.exec();
//        if(query.lastError().isValid()){
//            qDebug() << "stok kartı miktar ekleme/çıkarma hatası:\n" << query.lastError().text();
//        }
//        stokHareketiEkle(kullanici, kart.getBarkod(), islem, miktar);
//        cevap = true;
//    }
//    else if(islem == "ÇIKIŞ"){
//        if(mevcutStokMiktari >= miktar){
//            query.prepare("UPDATE stokkartlari SET miktar = ? WHERE id = ?");
//            query.bindValue(0, (mevcutStokMiktari - miktar));
//            query.bindValue(1, kart.getId());
//            query.exec();
//            if(query.lastError().isValid()){
//                qDebug() << "stok kartı miktar ekleme/çıkarma hatası:\n" << query.lastError().text();
//            }
//            stokHareketiEkle(kullanici, kart.getBarkod(), islem, miktar);
//            cevap = true;
//        }
//    }

    bool cevap = false;
    float mevcutStokMiktari = kart.getMiktar();

    switch (hareket) {
    case StokHareketi::Giris:
        query.prepare("UPDATE stokkartlari SET miktar = ? WHERE id = ?");
        query.bindValue(0, mevcutStokMiktari + miktar);
        query.bindValue(1, kart.getId());
        query.exec();
        if(query.lastError().isValid()){
            qDebug() << "stok kartı miktar ekleme/çıkarma hatası:\n" << query.lastError().text();
        }
        stokHareketiEkle(kullanici, kart.getBarkod(), hareket, miktar);
        cevap = true;
        break;
    case StokHareketi::Cikis:
        if(mevcutStokMiktari >= miktar){
            query.prepare("UPDATE stokkartlari SET miktar = ? WHERE id = ?");
            query.bindValue(0, (mevcutStokMiktari - miktar));
            query.bindValue(1, kart.getId());
            query.exec();
            if(query.lastError().isValid()){
                qDebug() << "stok kartı miktar ekleme/çıkarma hatası:\n" << query.lastError().text();
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
    QSqlQuery query(db);
    query.prepare("INSERT INTO stokkartlari (id, barkod, kod, ad, birim, miktar, grup, afiyat, sfiyat, kdv, otv, kdvdahil, otvdahil, tarih, uretici, tedarikci, aciklama) "
                    "VALUES (nextval('stokkartlari_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bindValue(0, stokKarti.getBarkod());
    query.bindValue(1, stokKarti.getKod());
    query.bindValue(2, stokKarti.getAd());
    query.bindValue(3, stokKarti.getBirim());
    query.bindValue(4, stokKarti.getMiktar());
    query.bindValue(5, stokKarti.getGrup());
    query.bindValue(6, stokKarti.getAFiyat());
    query.bindValue(7, stokKarti.getSFiyat());
    query.bindValue(8, stokKarti.getKdv());
    query.bindValue(9, stokKarti.getOtv());
    query.bindValue(10, stokKarti.getKdvdahil());
    query.bindValue(11, stokKarti.getOtvdahil());
    query.bindValue(12, stokKarti.getTarih());
    query.bindValue(13, stokKarti.getUretici().toInt());
    query.bindValue(14, stokKarti.getTedarikci().toInt());
    query.bindValue(15, stokKarti.getAciklama() + " [" + kullanici->getUserName() + "]");
    query.exec();
    return query.lastError();
}

QSqlError StokYonetimi::stokKartiniGuncelle(StokKarti duzenlenecekStokKarti, User *kullanici, bool fiyatDegistimi)
{
    QSqlQuery query(db);
    query.prepare("UPDATE stokkartlari SET "
                  "barkod = ?, "
                  "kod = ?, "
                  "ad = ?, "
                  "birim = ?, "
                  "miktar = ?, "
                  "grup = ?, "
                  "afiyat = ?, "
                  "sfiyat = ?, "
                  "kdv = ?, "
                  "otv = ?, "
                  "kdvdahil = ?, "
                  "otvdahil = ?, "
                  "tarih = ?, "
                  "uretici = ?, "
                  "tedarikci = ?, "
                  "aciklama = ? "
                  "WHERE id = ?");
    query.bindValue(0, duzenlenecekStokKarti.getBarkod());
    query.bindValue(1, duzenlenecekStokKarti.getKod());
    query.bindValue(2, duzenlenecekStokKarti.getAd());
    query.bindValue(3, duzenlenecekStokKarti.getBirim());
    query.bindValue(4, duzenlenecekStokKarti.getMiktar());
    query.bindValue(5, duzenlenecekStokKarti.getGrup());
    query.bindValue(6, duzenlenecekStokKarti.getAFiyat());
    query.bindValue(7, duzenlenecekStokKarti.getSFiyat());
    query.bindValue(8, duzenlenecekStokKarti.getKdv());
    query.bindValue(9, duzenlenecekStokKarti.getOtv());
    query.bindValue(10, duzenlenecekStokKarti.getKdvdahil());
    query.bindValue(11, duzenlenecekStokKarti.getOtvdahil());
    query.bindValue(12, duzenlenecekStokKarti.getTarih());
    query.bindValue(13, duzenlenecekStokKarti.getUretici().toInt());
    query.bindValue(14, duzenlenecekStokKarti.getTedarikci().toInt());
    query.bindValue(15, duzenlenecekStokKarti.getAciklama() + " " + kullanici->getUserName());
    query.bindValue(16, duzenlenecekStokKarti.getId());
    query.exec();
    qDebug() << "Stok karti guncelleme sorgusu:" << query.lastQuery();
    qDebug() << "Bound values:" << query.boundValues();
    qDebug() << "Etkilenen satir sayisi:" << query.numRowsAffected();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }

    //fiyatı değişti ise carilere veresiye satışların tutarlarını düzeltme
    if(fiyatDegistimi){

        // cari hareketlerini güncel fiyatlardan gösterme için fatura tutarlarını düzenleme ( cari borç hesaplama güncel fiyattan ise)------------------------
        // güncel fiyattan hesaplanacak cariye yapılmış veresiye satışları alır
        query.exec("SELECT fatura_no, toplamtutar, kalantutar FROM faturalar WHERE kalantutar NOT IN ('0') AND tipi = 2");
        if(query.lastError().isValid()){
            qDebug() << qPrintable("güncellenen ürün veresiye satıldımı sorgu hatası: \n" + query.lastError().text());
        }
        QStringList veresiyeFaturaNolar;
        QList<double> eskiToplamTutar;
        QList<double> eskiKalanTutar;
        while(query.next()){
            veresiyeFaturaNolar.append(query.value(0).toString());
            eskiToplamTutar.append(query.value(1).toDouble());
            eskiKalanTutar.append(query.value(2).toDouble());
        }
        for (int index = 0; index < veresiyeFaturaNolar.length(); ++index) {
            Sepet sepet = getSatis(veresiyeFaturaNolar.at(index));
            if(sepet.urunler.contains(duzenlenecekStokKarti.getBarkod())){
                query.prepare("UPDATE faturalar SET toplamtutar = ?, kalantutar = ? WHERE fatura_no = ?");
                query.bindValue(0, sepet.sepetToplamTutari());
                query.bindValue(1, ((sepet.sepetToplamTutari() - eskiToplamTutar.at(index)) + eskiKalanTutar.at(index)));
                query.bindValue(2, veresiyeFaturaNolar.at(index));
                query.exec();
                if(query.lastError().isValid()){
                    qDebug() << qPrintable("veresiyeTutarlariGuncelle() hatasi: \n" + query.lastError().text());
                }
            }
        }
    }

    return query.lastError();
}

Sepet StokYonetimi::getSatis(QString _faturaNo)
{
    Sepet satilmisSepet;
    QSqlQuery satisSorgu(db);
    QString sql = QString("SELECT barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, CAST(birim_f AS decimal), toplam_f, aciklama FROM stokhareketleri WHERE islem_no = '%1'")
                      .arg(_faturaNo.replace("'", "''"));
    satisSorgu.exec(sql);
    // aynı db bağlantısı üzerinde iç içe sorgu yapmamak için sonuçları tampona alıyorum
    struct SatisRow {
        QString barkod;
        float miktar;
    };
    QList<SatisRow> rows;
    while (satisSorgu.next()) {
        rows.append({satisSorgu.value(0).toString(), satisSorgu.value(3).toFloat()});
    }
    for (const auto& row : rows) {
        StokKarti sk = getStokKarti(row.barkod);
        satilmisSepet.urunEkle(sk, row.miktar);
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
    QSqlQuery islemSorgu(db);
    QString sql = QString("SELECT id, fatura_no, kullanici, toplamtutar, odenentutar, kalantutar, tarih, tipi, cari FROM faturalar WHERE fatura_no = '%1'")
                      .arg(_faturaNo.replace("'", "''"));
    islemSorgu.exec(sql);
    islemSorgu.next();
    return islemSorgu;
}

Cari StokYonetimi::getCariKart(QString cariID)
{
    QSqlQuery query(db);
    Cari kart;
    query.prepare("SELECT * FROM carikartlar WHERE id = ?");
    query.bindValue(0, cariID);
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
    if(query.next()){
        kart.setId(query.value(0).toInt());
        kart.setAd(query.value(1).toString());
        kart.setTip(query.value(2).toInt());
        kart.setVerigino(query.value(3).toString());
        kart.setVergiDaire(query.value(4).toString());
        kart.setIl(query.value(5).toString());
        kart.setIlce(query.value(6).toString());
        kart.setAdres(query.value(7).toString());
        kart.setMail(query.value(8).toString());
        kart.setTelefon(query.value(9).toString());
        kart.setTarih(query.value(10).toDateTime());
        kart.setAciklama(query.value(11).toString());
        kart.setYetkili(query.value(12).toString());
        return kart;
    }
    else{
        return kart;
    }
}

bool StokYonetimi::stokKartiSil(QString stokKartiID)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM stokkartlari WHERE id = ?");
    query.bindValue(0, stokKartiID);
    query.exec();
    if(query.lastError().isValid()){
        return false;
    }
    return true;
}

void StokYonetimi::stokHareketiEkle(User kullanici, QString barkod, StokHareketi hareket, float miktar)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO stokhareketleri(barkod, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                    "VALUES (?, ?, ?, ?, ?, ?)");
    query.bindValue(0, barkod);
    query.bindValue(1, hareket);
    query.bindValue(2, miktar);
    query.bindValue(3, QDateTime::currentDateTime());
    query.bindValue(4, kullanici.getUserID());
    query.bindValue(5, "STOK GİRİŞİ");
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << query.lastError().text();
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
    stokKartlariModel->setQuery(std::move(ququ));

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

QSqlQueryModel *StokYonetimi::getStokKartlari(QSqlQuery &&query)
{
    stokKartlariModel->setQuery(std::move(query));
    stokKartlariModel->setHeaderData(0, Qt::Horizontal, "Kod");
    stokKartlariModel->setHeaderData(1, Qt::Horizontal, "Barkod");
    stokKartlariModel->setHeaderData(2, Qt::Horizontal, "Ürün Adı");
    stokKartlariModel->setHeaderData(3, Qt::Horizontal, "Satış Fiyatı");
    return stokKartlariModel;
}

QSqlQueryModel *StokYonetimi::getStokKartlariEtiket()
{
    QSqlQuery query(db);
    query.prepare("SELECT barkod, ad, sfiyat, tarih FROM stokkartlari ORDER BY tarih DESC");
    query.exec();
    stokKartlariModel->setQuery(std::move(query));
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
    QSqlQuery query(db);
    query.exec("SELECT grup FROM stokgruplari");
    QStringList liste;
    while (query.next()) {
        liste.append(query.value(0).toString());
    }
    return liste;
}

int StokYonetimi::getGrupID(QString pGrup)
{
    QSqlQuery query(db);
    query.prepare("SELECT id FROM stokgruplari WHERE grup = ?");
    query.bindValue(0, pGrup);
    query.exec();
    if(query.next()){
        return query.value(0).toInt();
    }
    return 0;
}

QSqlQueryModel *StokYonetimi::getStokHareketleri(QString barkod)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM stokhareketleri WHERE barkod = ?");
    query.bindValue(0, barkod);
    query.exec();
    stokHareketleriModel->setQuery(std::move(query));
    return stokHareketleriModel;
}

QSqlQueryModel *StokYonetimi::getStokHareketleri(QString barkod, QDateTime baslangicTarih, QDateTime bitisTarih)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM stokhareketleri WHERE barkod = ? AND tarih BETWEEN ? AND ? ORDER BY tarih DESC");
    query.bindValue(0, barkod);
    query.bindValue(1, baslangicTarih);
    query.bindValue(2, bitisTarih.addDays(1));
    query.exec();
    stokHareketleriModel->setQuery(std::move(query));
    return stokHareketleriModel;
}

void StokYonetimi::stokGrupEkle(QString eklenecekGrupAdi)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO stokgruplari(id, grup) "
                    "VALUES (nextval('stokgruplari_id_seq'), ?)");
    query.bindValue(0, eklenecekGrupAdi);
    query.exec();
    if(query.lastError().isValid()){
        qWarning() << "stok grup ekleme sorgu INSERT hatası: " << query.lastError().text();
    }
}

void StokYonetimi::stokGrupSil(QString silinecekGrupAdi)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM stokgruplari WHERE grup = ?");
    query.bindValue(0, silinecekGrupAdi);
    query.exec();
    if(query.lastError().isValid()){
        qWarning() << "stok grup silme sorgu DELETE hatası: " << query.lastError().text();
    }
}

void StokYonetimi::stokBirimEkle(QString birim)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO stokbirimleri(id, birim) VALUES(nextval('stokbirimleri_sequence'), ?)");
    query.bindValue(0, birim);
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
}

void StokYonetimi::stokBirimSil(QString birim)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM stokbirimleri WHERE birim = ?");
    query.bindValue(0, birim);
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << "stok birim silme hatası: \n" << query.lastError().text();
    }
}

QStringList StokYonetimi::getStokBirimleri()
{
    QSqlQuery query(db);
    QStringList stokbirimleri;
    stokbirimleri.clear();
    stokbirimleri.append("Birim Seçin...");
    query.exec("SELECT * FROM stokbirimleri");
    while (query.next()) {
        stokbirimleri.append(query.value(1).toString());
    }
    return stokbirimleri;
}

int StokYonetimi::getBirimID(QString pBirim)
{
    QSqlQuery query(db);
    query.prepare("SELECT id FROM stokbirimleri WHERE birim = ?");
    query.bindValue(0, pBirim);
    query.exec();
    if(query.next()){
        return query.value(0).toInt();
    }
    return 0;
}

QString StokYonetimi::getBirimAd(int birimID)
{
    QSqlQuery query(db);
    query.prepare("SELECT birim FROM stokbirimleri WHERE id = ?");
    query.bindValue(0, birimID);
    query.exec();
    if(query.next()){
        return query.value(0).toString();
    }
    else{
        qDebug() << qPrintable(query.lastError().text());
    }
    return QString();
}

QHash<QString, float> StokYonetimi::getgunlukAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart)
{
    QSqlQuery query(db);
    sonTarih = sonTarih.addDays(1);// bir gün eklemezsem saati 00:00:00 aldığı için 1 gün eksik hesaplıyor.

    query.prepare("SELECT SUM(islem_miktari), DATE_TRUNC('day', tarih::timestamp) FROM stokhareketleri "
                  "WHERE tarih BETWEEN ? AND ? AND barkod = ? "
                  "GROUP BY DATE_TRUNC('day', tarih::timestamp) ORDER BY DATE_TRUNC('day', tarih::timestamp)");
    query.bindValue(0, ilkTarih);
    query.bindValue(1, sonTarih);
    query.bindValue(2, kart.getBarkod());
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
    QHash<QString, float> adetler;
    while (query.next()) {
        adetler.insert(query.value(1).toDate().toString(), query.value(0).toFloat());
    }
    return adetler;
}

QHash<QString, float> StokYonetimi::getAylikAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart)
{
    QSqlQuery query(db);
    sonTarih = sonTarih.addMonths(1);// bir ay eklemezsem saati 00:00:00 aldığı için 1 ay eksik hesaplıyor.

    query.prepare("SELECT SUM(islem_miktari), DATE_TRUNC('month', tarih::timestamp) FROM stokhareketleri "
                  "WHERE tarih BETWEEN ? AND ? AND barkod = ? "
                  "GROUP BY DATE_TRUNC('month', tarih::timestamp) ORDER BY DATE_TRUNC('month', tarih::timestamp)");
    query.bindValue(0, ilkTarih);
    query.bindValue(1, sonTarih);
    query.bindValue(2, kart.getBarkod());
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
    QHash<QString, float> adetler;
    while (query.next()) {
        adetler.insert(query.value(1).toDate().toString("MM.yyyy MMMM"), query.value(0).toFloat());
    }
    return adetler;
}

QHash<QString, float> StokYonetimi::getYillikAdetler(QDate ilkTarih, QDate sonTarih, StokKarti kart)
{
    QSqlQuery query(db);
    sonTarih = sonTarih.addYears(1);// bir yıl eklemezsem saati 00:00:00 aldığı için 1 yıl eksik hesaplıyor.

    query.prepare("SELECT SUM(islem_miktari), DATE_TRUNC('year', tarih::timestamp) FROM stokhareketleri "
                  "WHERE tarih BETWEEN ? AND ? AND barkod = ? "
                  "GROUP BY DATE_TRUNC('year', tarih::timestamp) ORDER BY DATE_TRUNC('year', tarih::timestamp)");
    query.bindValue(0, ilkTarih);
    query.bindValue(1, sonTarih);
    query.bindValue(2, kart.getBarkod());
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
    QHash<QString, float> adetler;
    while (query.next()) {
        adetler.insert(query.value(1).toDate().toString("yyyy"), query.value(0).toFloat());
    }
    return adetler;
}

QStringList StokYonetimi::getUreticiler()
{
    QSqlQuery query(db);
    QStringList liste;
    liste.append("Üretici seçin...");
    query.exec("SELECT ad FROM ureticiler ORDER BY ad ASC");
    while (query.next()) {
        liste.append(query.value(0).toString());
    }
    return liste;
}

int StokYonetimi::getUreticiID(QString ureticiAd)
{
    QSqlQuery query(db);
    query.prepare("SELECT id FROM ureticiler WHERE ad = ?");
    query.bindValue(0, ureticiAd);
    query.exec();
    if(!query.next()){
        return 0;
    }
    return query.value(0).toInt();
}

QString StokYonetimi::getUreticiAD(int ID)
{
    QSqlQuery query(db);
    query.prepare("SELECT ad FROM ureticiler WHERE id = ?");
    query.bindValue(0, ID);
    query.exec();
    if(query.lastError().isValid() || !query.next()){
        return 0;
    }
    return query.value(0).toString();
}

QStringList StokYonetimi::getTedarikciler()
{
    QSqlQuery query(db);
    QStringList liste;
    liste.append("Tedarikçi seçin...");
    query.exec("SELECT ad FROM carikartlar WHERE tip = 2 ORDER BY ad ASC");
    while (query.next()) {
        liste.append(query.value(0).toString());
    }
    return liste;
}

int StokYonetimi::getTedarikciID(QString tedarikciAd)
{
    QSqlQuery query(db);
    query.prepare("SELECT id FROM carikartlar WHERE tip = 2 AND ad = ?");
    query.bindValue(0, tedarikciAd);
    query.exec();
    if(!query.next()){
        return 0;
    }
    return query.value(0).toInt();
}

bool StokYonetimi::csvAktar(QString dosyaYolu)
{
    QSqlQuery query(db);
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
