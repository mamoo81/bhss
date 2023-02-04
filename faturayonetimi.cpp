#include "faturayonetimi.h"

FaturaYonetimi::FaturaYonetimi()
{

}

FaturaYonetimi::~FaturaYonetimi()
{

}
/**
 * @brief FaturaYonetimi::satisYap sepet satış işlemlerini yapar db kaydeder.
 * @param _SatilacakSepet Satılacak sepet parametresi
 * @param _Kullanici Satışı yapan kullanıcı parametresi
 * @param _SatisYapilacakCari Satış yapılacak cari id parametresi
 */
void FaturaYonetimi::satisYap(Sepet satilacakSepet, User satisYapanKullanici, int satisYapilanCariID)
{
    QString FaturaNo = yeniFaturaNo();
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, FaturaNo);
    sorgu.bindValue(1, satisYapilanCariID);
    sorgu.bindValue(2, 2);// 2 = satış faturası (veritabanında faturatipleri.tip)
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, satisYapanKullanici.getUserID());
    sorgu.bindValue(5, satilacakSepet.sepetToplamTutari());
    if(satilacakSepet.getOdenenTutar() > satilacakSepet.sepetToplamTutari()){// ödenen tutar sepet toplamından büyükse
        sorgu.bindValue(6, satilacakSepet.sepetToplamTutari());
    }
    else if(satilacakSepet.getOdenenTutar() <= satilacakSepet.sepetToplamTutari()){// ödenen tutar sepet toplam tutarından küçükse
        sorgu.bindValue(6, satilacakSepet.getOdenenTutar());
    }
    sorgu.bindValue(7, satilacakSepet.getKalanTutar());
    sorgu.bindValue(8, 1);//nakit ödeme tipinde satış
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    //kasa hareketi girme ve kasaya ödenen para kadar giriş
    if(satilacakSepet.getOdenenTutar() > 0){// kasanın tutması için. cariye veresiye kaydedecekse odenen tutar 0 dan büyükse
        // odenen para büyük veya eşitse sepettoplamtutarina sepetin tamamı ödendi demek.
        if(satilacakSepet.getOdenenTutar() >= satilacakSepet.sepetToplamTutari()){
            kasaYonetimi.KasaHareketiEkle(satisYapanKullanici,
                             KasaYonetimi::KasaHareketi::Giris,
                             satilacakSepet.sepetToplamTutari(),
                             "SATIŞ FAT.NO:" + FaturaNo,
                             QDateTime::currentDateTime(),
                             satilacakSepet.getSepettekiKazanc());

            if(satilacakSepet.getFazlaTutarAlacaklandir()){
                //yeni alış fatura bilgisi girme başlangıcı cariYonetimi classını bu sayfaya ekleyemediğim için böyle geçici çözdüm.

                double fazlaTutar = satilacakSepet.getOdenenTutar() - satilacakSepet.sepetToplamTutari();
                sorgu.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                                "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                sorgu.bindValue(0, yeniFaturaNo());
                sorgu.bindValue(1, satisYapilanCariID);
                sorgu.bindValue(2, 1);
                sorgu.bindValue(3, QDateTime::currentDateTime());
                sorgu.bindValue(4, satisYapanKullanici.getUserID());
                sorgu.bindValue(5, fazlaTutar);
                sorgu.bindValue(6, 0);
                sorgu.bindValue(7, fazlaTutar);
                sorgu.bindValue(8, "");
                sorgu.bindValue(9, FaturaNo + "'lu işlemde fazla ödenen tutar alacaklandirildi");
                sorgu.bindValue(10, 1);
                sorgu.exec();
                if(sorgu.lastError().isValid()){
                    qDebug() << qPrintable(sorgu.lastError().text());
                }
            }

            //********************************************************
        }
        // odenen para küçükse sepettoplamtutarindan sepet eksik veya veresiye ödendi.
        if(satilacakSepet.getOdenenTutar() < satilacakSepet.sepetToplamTutari()){
            kasaYonetimi.KasaHareketiEkle(satisYapanKullanici,
                             KasaYonetimi::KasaHareketi::Giris,
                             satilacakSepet.getOdenenTutar(),
                             "SATIŞ FAT.NO:" + FaturaNo,
                             QDateTime::currentDateTime(),
                             satilacakSepet.getSepettekiKazanc());// veresiye veya eksik ödediği için kar olarak eklemiyorum.
        }
    }

    //sepetteki ürünlerin stok hareketlerine girişi
    foreach (auto urun, satilacakSepet.urunler) {
        sorgu.prepare("INSERT INTO stokhareketleri(barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama, birim_f, toplam_f, kdv) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, urun.barkod);
        sorgu.bindValue(1, FaturaNo);
        sorgu.bindValue(2, "SATIŞ");
        sorgu.bindValue(3, urun.miktar);
        sorgu.bindValue(4, QDateTime::currentDateTime());
        sorgu.bindValue(5, satisYapanKullanici.getUserID());
        sorgu.bindValue(6, "SATIŞ");
        sorgu.bindValue(7, urun.birimFiyat);
        sorgu.bindValue(8, urun.birimFiyat * urun.miktar);
        sorgu.bindValue(9, stokYonetimi.getStokKarti(urun.barkod).getKdv());
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << sorgu.lastError().text();
        }
    }
    //sepetteki ürünlerin stoklardan düşülmesi
    foreach (auto urun, satilacakSepet.urunler) {
        sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE barkod = ?");
        sorgu.bindValue(0, urun.stokMiktari - urun.miktar);
        sorgu.bindValue(1, urun.barkod);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << sorgu.lastError().text();
        }
    }
}

//Sepet FaturaYonetimi::getSatis(QString _faturaNo, Cari cari)
//{
//    Sepet satilmisSepet;
//    QSqlQuery satisSorgu = QSqlQuery(db);// aşağıda while içinde ki satis.urunEkle() metodunda çağrılacak sorgu nesnesi ile karışmasın diye yeni query nesnesi oluşturdum.
//    satisSorgu.prepare("SELECT barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, CAST(birim_f AS decimal), toplam_f, aciklama FROM stokhareketleri WHERE islem_no = ?");
//    satisSorgu.bindValue(0, _faturaNo);
//    satisSorgu.exec();
//    if(cari.getGuncelBorcHesaplama()){
//        while (satisSorgu.next()) {
//            StokKarti sk = stokYonetimi.getStokKarti(satisSorgu.value(0).toString());
//            satilmisSepet.urunEkle(sk, satisSorgu.value(3).toFloat());
//            satilmisSepet.urunler[sk.getBarkod()].birimFiyat = satisSorgu.value(6).toDouble();
//        }
//    }
//    else{
//        while (satisSorgu.next()) {
//            StokKarti sk = stokYonetimi.getStokKarti(satisSorgu.value(0).toString());
//            satilmisSepet.urunEkle(sk, satisSorgu.value(3).toFloat(), satisSorgu.value(6).toDouble());
//        }
//    }
//    // faturanın ödenen ve kalan tutar bilgisini alma.
//    QSqlQuery islem = getIslemInfo(_faturaNo);
//    satilmisSepet.setOdenenTutar(islem.value(4).toDouble());
//    satilmisSepet.setKalanTutar(islem.value(5).toDouble());

//    return satilmisSepet;
//}

Sepet FaturaYonetimi::getSatis(QString _faturaNo, Cari::BorcHesaplama borcHesaplama)
{
    Sepet satilmisSepet;
    QSqlQuery satisSorgu = QSqlQuery(db);// aşağıda while içinde ki satis.urunEkle() metodunda çağrılacak sorgu nesnesi ile karışmasın diye yeni query nesnesi oluşturdum.
    satisSorgu.prepare("SELECT barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, CAST(birim_f AS decimal), toplam_f, aciklama FROM stokhareketleri WHERE islem_no = ?");
    satisSorgu.bindValue(0, _faturaNo);
    satisSorgu.exec();
    switch (borcHesaplama) {
    case Cari::BorcHesaplama::GuncelFiyattan:
        while (satisSorgu.next()) {
            StokKarti sk = stokYonetimi.getStokKarti(satisSorgu.value(0).toString());
            satilmisSepet.urunEkle(sk, satisSorgu.value(3).toFloat());
            satilmisSepet.urunler[sk.getBarkod()].birimFiyat = satisSorgu.value(6).toDouble();
        }
        break;
    case Cari::BorcHesaplama::SatildigiFiyattan:
        while (satisSorgu.next()) {
            StokKarti sk = stokYonetimi.getStokKarti(satisSorgu.value(0).toString());
            satilmisSepet.urunEkle(sk, satisSorgu.value(3).toFloat(), satisSorgu.value(6).toDouble());
        }
        break;
    }
    // faturanın ödenen ve kalan tutar bilgisini alma.
    QSqlQuery islem = getIslemInfo(_faturaNo);
    satilmisSepet.setOdenenTutar(islem.value(4).toDouble());
    satilmisSepet.setKalanTutar(islem.value(5).toDouble());

    return satilmisSepet;
}

QSqlQuery FaturaYonetimi::getIslemInfo(QString _faturaNo)
{
    QSqlQuery islemSorgu = QSqlQuery(db);
    islemSorgu.prepare("SELECT id, fatura_no, kullanici, toplamtutar, odenentutar, kalantutar, tarih, tipi, cari FROM faturalar WHERE fatura_no = ?");
    islemSorgu.bindValue(0, _faturaNo);
    islemSorgu.exec();
    islemSorgu.next();
    return islemSorgu;
}

void FaturaYonetimi::iadeAl(Sepet iadeSepet, User kullanici)
{
    QString iadeFaturaNo = yeniFaturaNo();
    // faturalar tablosuna iade fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, iadeFaturaNo);
    sorgu.bindValue(1, 1);// DİREKT cari id
    sorgu.bindValue(2, 3);// 3 = iade
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, kullanici.getUserID());
    sorgu.bindValue(5, iadeSepet.sepetToplamTutari());
    sorgu.bindValue(6, 1);//1 = nakit ödeme tipi
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    // kasaya eksi para girme
    sorgu.exec("SELECT * FROM kasa");
    sorgu.next();
    double suankiPara = sorgu.value(1).toDouble();
    double guncelKasaPara = suankiPara - iadeSepet.sepetToplamTutari();
    sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
    sorgu.bindValue(0, guncelKasaPara);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    kasaYonetimi.KasaHareketiEkle(kullanici, KasaYonetimi::KasaHareketi::Iade, iadeSepet.sepetToplamTutari(), ("İADE İŞLEMİ:" + iadeFaturaNo), QDateTime::currentDateTime(), -iadeSepet.getSepettekiKazanc());
    //sepetteki iade ürünlerin stok hareketlerine girişi
    foreach (auto urun, iadeSepet.urunler) {
        sorgu.prepare("INSERT INTO stokhareketleri(barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, urun.barkod);
        sorgu.bindValue(1, iadeFaturaNo);
        sorgu.bindValue(2, 3);// 3 = iade
        sorgu.bindValue(3, urun.miktar);
        sorgu.bindValue(4, QDateTime::currentDateTime());
        sorgu.bindValue(5, kullanici.getUserID());
        sorgu.bindValue(6, "İADE FATURA NO:" + iadeFaturaNo);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << sorgu.lastError().text();
        }
    }
    // sepetteki iade ürünlerin stoğa eklenmesi
    foreach (Urun urun, iadeSepet.urunler) {
        sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE barkod = ?");
        sorgu.bindValue(0, urun.miktar + urun.stokMiktari);
        sorgu.bindValue(1, urun.barkod);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << "iade ürün stoğa geri ekleme hatası:\n" << qPrintable(sorgu.lastError().text());
        }
    }
}

void FaturaYonetimi::iadeAl(Sepet iadeSepet, User kullanici, Cari iadeCari, QString faturaNo)
{
    QString iadeFaturaNo = yeniFaturaNo();
    // iade fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, evrakno) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, iadeFaturaNo);
    sorgu.bindValue(1, iadeCari.getId());
    sorgu.bindValue(2, 3);// iade fatura tipi
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, kullanici.getUserID());
    sorgu.bindValue(5, iadeSepet.sepetToplamTutari());
    sorgu.bindValue(6, faturaNo);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    // kasaya eksi para girme
    sorgu.exec("SELECT * FROM kasa");
    sorgu.next();
    double suankiPara = sorgu.value(1).toDouble();
    double guncelKasaPara = suankiPara - iadeSepet.getOdenenTutar();
    sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
    sorgu.bindValue(0, guncelKasaPara);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    //kasa hareketlerini girme
    sorgu.clear();
    sorgu.prepare("INSERT INTO kasahareketleri (id, miktar, kullanici, islem, kar, tarih, aciklama) "
                  "VALUES (nextval('kasahareketleri_sequence'), ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, iadeSepet.getOdenenTutar());
    sorgu.bindValue(1, kullanici.getUserID());
    sorgu.bindValue(2, "İADE");
    sorgu.bindValue(3, (iadeSepet.getSepettekiKazanc() * -1));
    sorgu.bindValue(4, QDateTime::currentDateTime());
    sorgu.bindValue(5, "İADESİ YAPILAN FAT.NO:" + faturaNo);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    //sepetteki iade ürünlerin stok hareketlerine girişi
    foreach (auto urun, iadeSepet.urunler) {
        sorgu.prepare("INSERT INTO stokhareketleri(barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, urun.barkod);
        sorgu.bindValue(1, iadeFaturaNo);
        sorgu.bindValue(2, "İADE");
        sorgu.bindValue(3, urun.miktar);
        sorgu.bindValue(4, QDateTime::currentDateTime());
        sorgu.bindValue(5, kullanici.getUserID());
        sorgu.bindValue(6, "İADE");
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << qPrintable(sorgu.lastError().text());
        }
    }
    // sepetteki iade ürünlerin stoğa eklenmesi
    foreach (Urun urun, iadeSepet.urunler) {
        sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE barkod = ?");
        sorgu.bindValue(0, urun.miktar + urun.stokMiktari);
        sorgu.bindValue(1, urun.barkod);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qWarning() << "iade ürün stoğa geri ekleme hatası:\n" << qPrintable(sorgu.lastError().text());
        }
    }
}

bool FaturaYonetimi::iadeAlinmismi(QString FaturaNo)
{
    sorgu.prepare("SELECT * FROM faturalar WHERE evrakno = ? AND tipi = 3");
    sorgu.bindValue(0, FaturaNo);
    sorgu.exec();
    // satışı yapılmış fatura no sunun iade faturası da varsa true döndürür
    if(sorgu.next()){
        return true;
    }
    return false;
}

bool FaturaYonetimi::faturayiSil(QString faturaNo)
{
    if(!iadeAlinmismi(faturaNo)){
        sorgu.prepare("DELETE FROM faturalar WHERE fatura_no = ?");
        sorgu.bindValue(0, faturaNo);
        if(sorgu.exec()){
            return true;
        }
        else{
            return false;
            qDebug() << qPrintable(sorgu.lastError().text());
        }
    }
    return false;
}

QString FaturaYonetimi::yeniFaturaNo()
{
    sorgu.exec("SELECT * FROM faturalar");
    // ilk fatura girişi ise faturalar_sequence değerini gönder.
    if(!sorgu.next()){
        //yeni fatura numarası için faturalar_sequence'den son değeri alma
        sorgu.exec("SELECT last_value FROM faturalar_sequence");
        sorgu.next();
        if(sorgu.lastError().isValid()){
            qDebug() << sorgu.lastError().text();
        }
        return QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt());
    }
    //yeni fatura numarası için faturalar_sequence'den son değeri alma
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    sorgu.next();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    return QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt() + 1);
}

QString FaturaYonetimi::sonIslemNumarasi()
{
    sorgu.exec("SELECT fatura_no, tarih FROM faturalar ORDER BY tarih DESC LIMIT 1");
    sorgu.next();
    return sorgu.value(0).toString();
}

