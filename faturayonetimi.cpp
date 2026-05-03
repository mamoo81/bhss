#include "faturayonetimi.h"

#include <QList>

// SQL string'lerinde tek tırnak kaçışı için yardımcı
static QString esc(const QString &s) {
    QString r = s;
    r.replace("'", "''");
    return r;
}

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
    QString dtStr = QDateTime::currentDateTime().toString(Qt::ISODate);
    double odenen = satilacakSepet.getOdenenTutar();
    double toplam = satilacakSepet.sepetToplamTutari();
    //yeni fatura bilgisi girme başlangıcı
    {
        QSqlQuery q(db);
        QString sql = QString("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, odemetipi) "
                              "VALUES (nextval('faturalar_sequence'), '%1', %2, 2, '%3'::timestamp, %4, %5, %6, %7, 1)")
                          .arg(esc(FaturaNo))
                          .arg(satisYapilanCariID)
                          .arg(dtStr)
                          .arg(satisYapanKullanici.getUserID())
                          .arg(toplam)
                          .arg((odenen > toplam) ? toplam : odenen)
                          .arg(satilacakSepet.getKalanTutar());
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << q.lastError().text();
        }
    }

    //kasa hareketi girme ve kasaya ödenen para kadar giriş
    if(odenen > 0){
        if(odenen >= toplam){
            kasaYonetimi.KasaHareketiEkle(satisYapanKullanici,
                             KasaYonetimi::KasaHareketi::Giris,
                             toplam,
                             "SATIŞ FAT.NO:" + FaturaNo,
                             QDateTime::currentDateTime(),
                             satilacakSepet.getSepettekiKazanc());

            if(satilacakSepet.getFazlaTutarAlacaklandir()){
                double fazlaTutar = odenen - toplam;
                QString yeniFazlaFaturaNo = yeniFaturaNo();
                QSqlQuery q(db);
                QString sql = QString("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                                      "VALUES (nextval('faturalar_sequence'), '%1', %2, 1, '%3'::timestamp, %4, %5, 0, %6, '', '%7', 1)")
                                  .arg(esc(yeniFazlaFaturaNo))
                                  .arg(satisYapilanCariID)
                                  .arg(QDateTime::currentDateTime().toString(Qt::ISODate))
                                  .arg(satisYapanKullanici.getUserID())
                                  .arg(fazlaTutar)
                                  .arg(fazlaTutar)
                                  .arg(esc(FaturaNo + "'lu işlemde fazla ödenen tutar alacaklandirildi"));
                q.exec(sql);
                if(q.lastError().isValid()){
                    qDebug() << qPrintable(q.lastError().text());
                }
            }
        }
        if(odenen < toplam){
            kasaYonetimi.KasaHareketiEkle(satisYapanKullanici,
                             KasaYonetimi::KasaHareketi::Giris,
                             odenen,
                             "SATIŞ FAT.NO:" + FaturaNo,
                             QDateTime::currentDateTime(),
                             satilacakSepet.getSepettekiKazanc());
        }
    }

    // KDV değerlerini döngüden ÖNCE önbelleğe alıyorum
    QHash<QString, int> kdvMap;
    for (auto urun : satilacakSepet.urunler) {
        kdvMap[urun.barkod] = stokYonetimi.getStokKarti(urun.barkod).getKdv();
    }

    //sepetteki ürünlerin stok hareketlerine girişi
    for (auto urun : satilacakSepet.urunler) {
        QSqlQuery q(db);
        QString sql = QString("INSERT INTO stokhareketleri(barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama, birim_f, toplam_f, kdv) "
                              "VALUES ('%1', '%2', 'SATIŞ', %3, '%4'::timestamp, %5, 'SATIŞ', %6, %7, %8)")
                          .arg(esc(urun.barkod))
                          .arg(esc(FaturaNo))
                          .arg(urun.miktar)
                          .arg(dtStr)
                          .arg(satisYapanKullanici.getUserID())
                          .arg(urun.birimFiyat)
                          .arg(urun.birimFiyat * urun.miktar)
                          .arg(kdvMap[urun.barkod]);
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << q.lastError().text();
        }
    }
    //sepetteki ürünlerin stoklardan düşülmesi
    for (auto urun : satilacakSepet.urunler) {
        QSqlQuery q(db);
        QString sql = QString("UPDATE stokkartlari SET miktar = %1 WHERE barkod = '%2'")
                          .arg(urun.stokMiktari - urun.miktar)
                          .arg(esc(urun.barkod));
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << q.lastError().text();
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

Sepet FaturaYonetimi::getSatis(QString _faturaNo)
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
        double birimFiyat;
    };
    QList<SatisRow> rows;
    while (satisSorgu.next()) {
        rows.append({satisSorgu.value(0).toString(), satisSorgu.value(3).toFloat(), satisSorgu.value(6).toDouble()});
    }
    for (const auto& row : rows) {
        StokKarti sk = stokYonetimi.getStokKarti(row.barkod);
        satilmisSepet.urunEkle(sk, row.miktar, row.birimFiyat);
    }
    // faturanın ödenen ve kalan tutar bilgisini alma.
    QSqlQuery islem = getIslemInfo(_faturaNo);
    satilmisSepet.setOdenenTutar(islem.value(4).toDouble());
    satilmisSepet.setKalanTutar(islem.value(5).toDouble());

    return satilmisSepet;
}

QSqlQuery FaturaYonetimi::getIslemInfo(QString _faturaNo)
{
    QSqlQuery islemSorgu(db);
    QString sql = QString("SELECT id, fatura_no, kullanici, toplamtutar, odenentutar, kalantutar, tarih, tipi, cari FROM faturalar WHERE fatura_no = '%1'")
                      .arg(_faturaNo.replace("'", "''"));
    islemSorgu.exec(sql);
    islemSorgu.next();
    return islemSorgu;
}

void FaturaYonetimi::iadeAl(Sepet iadeSepet, User kullanici)
{
    QString iadeFaturaNo = yeniFaturaNo();
    QString dtStr = QDateTime::currentDateTime().toString(Qt::ISODate);
    // faturalar tablosuna iade fatura bilgisi girme başlangıcı
    {
        QSqlQuery q(db);
        QString sql = QString("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odemetipi) "
                              "VALUES (nextval('faturalar_sequence'), '%1', 1, 3, '%2'::timestamp, %3, %4, 1)")
                          .arg(esc(iadeFaturaNo))
                          .arg(dtStr)
                          .arg(kullanici.getUserID())
                          .arg(iadeSepet.sepetToplamTutari());
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << qPrintable(q.lastError().text());
        }
    }
    // kasaya eksi para girme
    double suankiPara = 0;
    {
        QSqlQuery q(db);
        q.exec("SELECT * FROM kasa");
        q.next();
        suankiPara = q.value(1).toDouble();
    }
    double guncelKasaPara = suankiPara - iadeSepet.sepetToplamTutari();
    {
        QSqlQuery q(db);
        QString sql = QString("UPDATE kasa SET para = %1 WHERE id = '1'")
                          .arg(guncelKasaPara);
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << qPrintable(q.lastError().text());
        }
    }
    kasaYonetimi.KasaHareketiEkle(kullanici, KasaYonetimi::KasaHareketi::Iade, iadeSepet.sepetToplamTutari(), ("İADE İŞLEMİ:" + iadeFaturaNo), QDateTime::currentDateTime(), -iadeSepet.getSepettekiKazanc());
    //sepetteki iade ürünlerin stok hareketlerine girişi
    for (auto urun : iadeSepet.urunler) {
        QSqlQuery q(db);
        QString sql = QString("INSERT INTO stokhareketleri(barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                              "VALUES ('%1', '%2', 3, %3, '%4'::timestamp, %5, 'İADE FATURA NO:%6')")
                          .arg(esc(urun.barkod))
                          .arg(esc(iadeFaturaNo))
                          .arg(urun.miktar)
                          .arg(dtStr)
                          .arg(kullanici.getUserID())
                          .arg(esc(iadeFaturaNo));
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << q.lastError().text();
        }
    }
    // sepetteki iade ürünlerin stoğa eklenmesi
    for (Urun urun : iadeSepet.urunler) {
        QSqlQuery q(db);
        QString sql = QString("UPDATE stokkartlari SET miktar = %1 WHERE barkod = '%2'")
                          .arg(urun.miktar + urun.stokMiktari)
                          .arg(esc(urun.barkod));
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << "iade ürün stoğa geri ekleme hatası:\n" << qPrintable(q.lastError().text());
        }
    }
}

void FaturaYonetimi::iadeAl(Sepet iadeSepet, User kullanici, Cari iadeCari, QString faturaNo)
{
    QString iadeFaturaNo = yeniFaturaNo();
    QString dtStr = QDateTime::currentDateTime().toString(Qt::ISODate);
    // iade fatura bilgisi girme başlangıcı
    {
        QSqlQuery q(db);
        QString sql = QString("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, evrakno) "
                              "VALUES (nextval('faturalar_sequence'), '%1', %2, 3, '%3'::timestamp, %4, %5, '%6')")
                          .arg(esc(iadeFaturaNo))
                          .arg(iadeCari.getId())
                          .arg(dtStr)
                          .arg(kullanici.getUserID())
                          .arg(iadeSepet.sepetToplamTutari())
                          .arg(esc(faturaNo));
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << qPrintable(q.lastError().text());
        }
    }
    // kasaya eksi para girme
    double suankiPara = 0;
    {
        QSqlQuery q(db);
        q.exec("SELECT * FROM kasa");
        q.next();
        suankiPara = q.value(1).toDouble();
    }
    double guncelKasaPara = suankiPara - iadeSepet.getOdenenTutar();
    {
        QSqlQuery q(db);
        QString sql = QString("UPDATE kasa SET para = %1 WHERE id = '1'")
                          .arg(guncelKasaPara);
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << qPrintable(q.lastError().text());
        }
    }
    //kasa hareketlerini girme
    {
        QSqlQuery q(db);
        QString sql = QString("INSERT INTO kasahareketleri (id, miktar, kullanici, islem, kar, tarih, aciklama) "
                              "VALUES (nextval('kasahareketleri_sequence'), %1, %2, 'İADE', %3, '%4'::timestamp, 'İADESİ YAPILAN FAT.NO:%5')")
                          .arg(iadeSepet.getOdenenTutar())
                          .arg(kullanici.getUserID())
                          .arg(iadeSepet.getSepettekiKazanc() * -1)
                          .arg(dtStr)
                          .arg(esc(faturaNo));
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << qPrintable(q.lastError().text());
        }
    }
    //sepetteki iade ürünlerin stok hareketlerine girişi
    for (auto urun : iadeSepet.urunler) {
        QSqlQuery q(db);
        QString sql = QString("INSERT INTO stokhareketleri(barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                              "VALUES ('%1', '%2', 'İADE', %3, '%4'::timestamp, %5, 'İADE')")
                          .arg(esc(urun.barkod))
                          .arg(esc(iadeFaturaNo))
                          .arg(urun.miktar)
                          .arg(dtStr)
                          .arg(kullanici.getUserID());
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << qPrintable(q.lastError().text());
        }
    }
    // sepetteki iade ürünlerin stoğa eklenmesi
    for (Urun urun : iadeSepet.urunler) {
        QSqlQuery q(db);
        QString sql = QString("UPDATE stokkartlari SET miktar = %1 WHERE barkod = '%2'")
                          .arg(urun.miktar + urun.stokMiktari)
                          .arg(esc(urun.barkod));
        q.exec(sql);
        if(q.lastError().isValid()){
            qWarning() << "iade ürün stoğa geri ekleme hatası:\n" << qPrintable(q.lastError().text());
        }
    }
}

bool FaturaYonetimi::iadeAlinmismi(QString FaturaNo)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM faturalar WHERE evrakno = ? AND tipi = 3");
    query.bindValue(0, FaturaNo);
    query.exec();
    // satışı yapılmış fatura no sunun iade faturası da varsa true döndürür
    if(query.next()){
        return true;
    }
    return false;
}

bool FaturaYonetimi::faturayiSil(QString faturaNo)
{
    QSqlQuery query(db);
    if(!iadeAlinmismi(faturaNo)){
        query.prepare("DELETE FROM faturalar WHERE fatura_no = ?");
        query.bindValue(0, faturaNo);
        if(query.exec()){
            return true;
        }
        else{
            return false;
            qDebug() << qPrintable(query.lastError().text());
        }
    }
    return false;
}

QString FaturaYonetimi::yeniFaturaNo()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM faturalar");
    // ilk fatura girişi ise faturalar_sequence değerini gönder.
    if(!query.next()){
        //yeni fatura numarası için faturalar_sequence'den son değeri alma
        query.exec("SELECT last_value FROM faturalar_sequence");
        query.next();
        if(query.lastError().isValid()){
            qDebug() << query.lastError().text();
        }
        return QDate::currentDate().toString("ddMMyy") + QString::number(query.value(0).toUInt());
    }
    //yeni fatura numarası için faturalar_sequence'den son değeri alma
    query.exec("SELECT last_value FROM faturalar_sequence");
    query.next();
    if(query.lastError().isValid()){
        qDebug() << query.lastError().text();
    }
    return QDate::currentDate().toString("ddMMyy") + QString::number(query.value(0).toUInt() + 1);
}

QString FaturaYonetimi::sonIslemNumarasi()
{
    QSqlQuery query(db);
    query.exec("SELECT fatura_no, tarih FROM faturalar ORDER BY tarih DESC LIMIT 1");
    query.next();
    return query.value(0).toString();
}

