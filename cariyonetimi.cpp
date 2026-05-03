#include "cariyonetimi.h"

CariYonetimi::CariYonetimi()
{

}

Cari CariYonetimi::getCariKart(QString cariID)
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

QList<Cari> CariYonetimi::getCariKartlar()
{
    QSqlQuery query(db);
    QList<Cari> kartlar;
    query.exec("SELECT * FROM carikartlar WHERE id NOT IN('1') ORDER BY ad ASC");
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
    while (query.next()) {
        Cari kart;
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
        kartlar.append(kart);
    }
    return kartlar;
}


QSqlQueryModel *CariYonetimi::getCariKartIsimleri()
{
    cariKartIsımleriModel->setQuery("SELECT carikartlar.id, carikartlar.ad, yetkili, caritipleri.tip, il, ilce, vergi_no, vergi_daire FROM carikartlar "
                                    "INNER JOIN caritipleri ON carikartlar.tip = caritipleri.id "
                                    "WHERE carikartlar.id NOT IN(1)", db);
    cariKartIsımleriModel->setHeaderData(0, Qt::Horizontal, "ID");
    cariKartIsımleriModel->setHeaderData(1, Qt::Horizontal, "Cari ismi");
    cariKartIsımleriModel->setHeaderData(2, Qt::Horizontal, "Yetkili");
    cariKartIsımleriModel->setHeaderData(3, Qt::Horizontal, "Cari Tipi");
    cariKartIsımleriModel->setHeaderData(4, Qt::Horizontal, "İl");
    cariKartIsımleriModel->setHeaderData(5, Qt::Horizontal, "İlçe");
    cariKartIsımleriModel->setHeaderData(6, Qt::Horizontal, "Vergi No");
    cariKartIsımleriModel->setHeaderData(7, Qt::Horizontal, "Vergi Dairesi");
    return cariKartIsımleriModel;
}

bool CariYonetimi::yeniCariKart(Cari cariKart)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO carikartlar(id, ad, tip, vergi_no, vergi_daire, il, ilce, adres, mail, telefon, tarih, aciklama, yetkili) "
                  "VALUES(nextval('carikartlar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bindValue(0, cariKart.getAd());
    query.bindValue(1, cariKart.getTip());
    query.bindValue(2, cariKart.getVerigino());
    query.bindValue(3, cariKart.getVergiDaire());
    query.bindValue(4, cariKart.getIl());
    query.bindValue(5, cariKart.getIlce());
    query.bindValue(6, cariKart.getAdres());
    query.bindValue(7, cariKart.getMail());
    query.bindValue(8, cariKart.getTelefon());
    query.bindValue(9, cariKart.getTarih());
    query.bindValue(10, cariKart.getAciklama());
    query.bindValue(11, cariKart.getYetkili());
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
        return false;
    }
    return true;
}

bool CariYonetimi::cariKartDuzenle(Cari cariKart)
{
    QSqlQuery query(db);
    query.prepare("UPDATE carikartlar "
                    "SET ad = ?, tip = ?, vergi_no = ?, vergi_daire = ?, il = ?, ilce = ?, adres = ?, mail = ?, telefon = ?, aciklama = ?, yetkili = ? "
                    "WHERE id = ?") ;
    query.bindValue(0, cariKart.getAd());
    query.bindValue(1, cariKart.getTip());
    query.bindValue(2, cariKart.getVerigino());
    query.bindValue(3, cariKart.getVergiDaire());
    query.bindValue(4, cariKart.getIl());
    query.bindValue(5, cariKart.getIlce());
    query.bindValue(6, cariKart.getAdres());
    query.bindValue(7, cariKart.getMail());
    query.bindValue(8, cariKart.getTelefon());
    query.bindValue(9, QDateTime::currentDateTime().toString() + " <- tarihinde güncellendi.");
    query.bindValue(10, cariKart.getYetkili());
    query.bindValue(11, cariKart.getId());
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
        return false;
    }
    return true;
}

QSqlQueryModel *CariYonetimi::getCariHareketleri(QString cariID)
{
    QSqlQuery query(db);
    query.prepare("SELECT fatura_no, faturalar.tarih, faturatipleri.tip, toplamtutar, odenentutar, kalantutar, odemetipleri.tip, kullanicilar.username, faturalar.aciklama FROM faturalar "
                    "INNER JOIN faturatipleri ON faturalar.tipi = faturatipleri.id "
                    "INNER JOIN odemetipleri ON faturalar.odemetipi = odemetipleri.id "
                    "INNER JOIN kullanicilar ON faturalar.kullanici = kullanicilar.id "
                    "WHERE cari = ? ORDER BY faturalar.tarih DESC");
    query.bindValue(0, cariID);
    query.exec();
    cariHareketleriModel->setQuery(std::move(query));
    cariHareketleriModel->setHeaderData(0, Qt::Horizontal, "Fatura No");
    cariHareketleriModel->setHeaderData(1, Qt::Horizontal, "Fatura Tarihi");
    cariHareketleriModel->setHeaderData(2, Qt::Horizontal, "Fatura Tip");
    cariHareketleriModel->setHeaderData(3, Qt::Horizontal, "T. Tutar");
    cariHareketleriModel->setHeaderData(4, Qt::Horizontal, "Ö. Tutar");
    cariHareketleriModel->setHeaderData(5, Qt::Horizontal, "K. Tutar");
    cariHareketleriModel->setHeaderData(6, Qt::Horizontal, "Ödeme");
    cariHareketleriModel->setHeaderData(7, Qt::Horizontal, "İşlemi Yapan");
    cariHareketleriModel->setHeaderData(8, Qt::Horizontal, "Açıklama");
    return cariHareketleriModel;
}

QStringList CariYonetimi::getCariTipleri()
{
    QSqlQuery query(db);
    QStringList cariTipList;
    query.exec("SELECT * FROM caritipleri");
    while (query.next()) {
        cariTipList.append(query.value(1).toString());
    }
    return cariTipList;
}

QStringList CariYonetimi::getVergiDaireleri()
{
    QSqlQuery query(db);
    QStringList liste;
    query.exec("SELECT ad FROM vergidaireleri");
    while (query.next()) {
        liste.append(query.value(0).toString());
    }
    return liste;
}

double CariYonetimi::getCariToplamAlacak(QString cariID)
{
    QSqlQuery query(db);
    if(cariID != "1"){// DİREKT CARİSİ İSE ES GEÇSİN
        double kalanTutar, odenenTutar;
        query.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari = ? AND tipi = 1");
        query.bindValue(0, cariID);
        query.exec();
        if(query.next()){
            kalanTutar = query.value(0).toDouble();
        }
        else{
            kalanTutar = 0;
        }
        query.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari = ? AND tipi = 4");
        query.bindValue(0, cariID);
        query.exec();
        if(query.next()){
            odenenTutar = query.value(0).toDouble();
        }
        else{
            odenenTutar = 0;
        }
        return kalanTutar - odenenTutar;
    }
    else{
        return 0;
    }
}

double CariYonetimi::getCariToplamBorc(QString cariID)
{
    QSqlQuery query(db);
    double odenenTutar = 0, toplamTutar = 0;

//    Cari cariKart = getCariKart(cariID);

    QSqlQuery sorguSatislar(db);
    sorguSatislar.prepare("select sum(cast(toplamtutar as decimal)), sum(cast(odenentutar as decimal)), sum(cast(kalantutar as decimal)) from faturalar where cari = ? and tipi in(2,5)");
    sorguSatislar.bindValue(0, cariID);
    sorguSatislar.exec();
    if(sorguSatislar.next()){
        odenenTutar = sorguSatislar.value(1).toDouble();
        toplamTutar = sorguSatislar.value(0).toDouble();
    }
    return toplamTutar - odenenTutar;

//    if(cariKart.getGuncelBorcHesaplama()){
//        // cariye yapılan tüm satışların yapıldığı an ki tutarlarının alınması.
//        QSqlQuery sorguSatislar(db);
//        sorguSatislar.prepare("select sum(cast(toplamtutar as decimal)), sum(cast(odenentutar as decimal)), sum(cast(kalantutar as decimal)) from faturalar where cari = ? and tipi in(2,5)");
//        sorguSatislar.bindValue(0, cariID);
//        sorguSatislar.exec();
//        if(sorguSatislar.next()){
////            kalanTutar = sorguSatislar.value(2).toDouble();
//            odenenTutar = sorguSatislar.value(1).toDouble();
//            toplamTutar = sorguSatislar.value(0).toDouble();
//        }
//        // veresiye/kısmi ödenmiş/satılmış sepetlerin fatura numaralarını getirme
//        QSqlQuery veresiyeSatilansepetler(db);
//        veresiyeSatilansepetler.prepare("select fatura_no from faturalar where cari = ? and kalantutar not in ('0') and tipi = 2");
//        veresiyeSatilansepetler.bindValue(0, cariID);
//        veresiyeSatilansepetler.exec();
//        QStringList faturaNumaralari;
//        while (veresiyeSatilansepetler.next()) {
//            faturaNumaralari.append(veresiyeSatilansepetler.value(0).toString());
//        }

//        for (QString faturaNo : faturaNumaralari) {
//            zamToplamlari += faturaYonetimi.getSatis(faturaNo/*, cariKart*/).getFiyatFarki();
//        }
//        toplamTutar += zamToplamlari;
////        kalanTutar += zamToplamlari;

//        return toplamTutar - odenenTutar;
//    }
//    else{
//        query.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari = ? AND tipi = 2");
//        query.bindValue(0, cariID);
//        query.exec();
//        if(query.next()){
//            kalanTutar = query.value(0).toDouble();
//        }
//        else{
//            kalanTutar = 0;
//        }
//        query.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari = ? AND tipi = 5");
//        query.bindValue(0, cariID);
//        query.exec();
//        if(query.next()){
//            odenenTutar = query.value(0).toDouble();
//        }
//        else{
//            odenenTutar = 0;
//        }
//        return kalanTutar - odenenTutar;
//    }
}

double CariYonetimi::getCarilerToplamAlacak()
{
    QSqlQuery query(db);
    double kalanTutar, odenenTutar;
    query.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 1");
    query.exec();
    if(query.next()){
        kalanTutar = query.value(0).toDouble();
    }
    else{
        kalanTutar = 0;
    }
    query.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 4");
    query.exec();
    if(query.next()){
        odenenTutar = query.value(0).toDouble();
    }
    else{
        odenenTutar = 0;
    }
    return kalanTutar - odenenTutar;
}

double CariYonetimi::getcarilerToplamBorc(QDateTime startDate, QDateTime endDate)
{
    QSqlQuery query(db);
    double kalanTutar = 0, odenenTutar = 0;
//    if(!guncel){
        query.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 2 AND tarih BETWEEN ? AND ?"); // cari 1 'e eşit olmayanları select ediyorum ki direkt carisini dahil etmesin.
        query.bindValue(0, startDate);
        query.bindValue(1, endDate);
        query.exec();
        if(query.next()){
            kalanTutar = query.value(0).toDouble();
        }
        else{
            kalanTutar = 0;
        }
        query.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 5 AND tarih BETWEEN ? AND ?"); // cari 1 'e eşit olmayanları select ediyorum ki direkt carisini dahil etmesin.
        query.bindValue(0, startDate);
        query.bindValue(1, endDate);
        query.exec();
        if(query.next()){
            odenenTutar = query.value(0).toDouble();
        }
        else{
            odenenTutar = 0;
        }
        return kalanTutar - odenenTutar;
//    }
//    else{               //********* sepetlerin güncel tutarlarının alınması. *************//
//        double guncelSepetToplamlari = 0;

//        QSqlQuery sorguGuncelFiyat(db);
//        sorguGuncelFiyat.prepare("SELECT fatura_no FROM faturalar WHERE cari NOT IN('1') AND tipi = 2 AND kalantutar > 0 AND tarih BETWEEN ? AND ?");
//        sorguGuncelFiyat.bindValue(0, startDate);
//        sorguGuncelFiyat.bindValue(1, endDate);
//        sorguGuncelFiyat.exec();
//        if(sorguGuncelFiyat.isSelect()){

//            QStringList faturalar;
//            while (sorguGuncelFiyat.next()) {
//                faturalar.append(sorguGuncelFiyat.value(0).toString());
//            }

//            for (auto fatura : faturalar) {
//                guncelSepetToplamlari += faturaYonetimi.getSatis(fatura).sepetToplamTutari();
//            }

//            sorguGuncelFiyat.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 2 AND kalantutar > 0 AND tarih BETWEEN ? AND ?");
//            sorguGuncelFiyat.bindValue(0, startDate);
//            sorguGuncelFiyat.bindValue(1, endDate);
//            sorguGuncelFiyat.exec();
//            if(sorguGuncelFiyat.next()){
//                kalanTutar = sorguGuncelFiyat.value(0).toDouble() + (guncelSepetToplamlari - sorguGuncelFiyat.value(0).toDouble());
//            }
//            else{
//                kalanTutar = 0;
//            }

//            sorguGuncelFiyat.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 5 AND kalantutar > 0 AND tarih BETWEEN ? AND ?");
//            sorguGuncelFiyat.bindValue(0, startDate);
//            sorguGuncelFiyat.bindValue(1, endDate);
//            sorguGuncelFiyat.exec();
//            if(sorguGuncelFiyat.next()){
//                odenenTutar = sorguGuncelFiyat.value(0).toDouble();
//            }
//            else{
//                odenenTutar = 0;
//            }
//        }
//    }
//    return kalanTutar - odenenTutar;
}

bool CariYonetimi::cariHareketiSil(QString faturaNo, User kullanici, Cari cari)
{
    QSqlQuery query(db);
    //    3 ana adım
    //    faturanın silinmesi
    //    sepetteki ürünlerin stoğa geri eklenmesi
    //    kasa hareketlerine girilmesi

    //satış faturası iade alınmamışsa
    if(!faturaYonetimi.iadeAlinmismi(faturaNo)){
        // faturadaki ürünlerin stoğa geri eklenmesi
        Sepet sepet = faturaYonetimi.getSatis(faturaNo);
        //satılan sepetteki ürünleri stoğa geri ekle
        for (auto urun : sepet.urunler) {
            stokYonetimi.setStokMiktari(kullanici, stokYonetimi.getStokKarti(urun.barkod), StokYonetimi::StokHareketi::Giris, urun.miktar);// stok hareketlerine de ekliyor.
        }
        // kasahareketine - para girişi
        kasaYonetimi.KasaHareketiEkle(kullanici, KasaYonetimi::KasaHareketi::Cikis, sepet.getOdenenTutar(), QString("cari hareket silme:%1").arg(cari.getId()), QDateTime::currentDateTime(), sepet.getSepettekiKazanc());// kasaya para giriş/çıkış da yapar.
        //faturanın silinmesi
        faturaYonetimi.faturayiSil(faturaNo);
        return true;
    }
    return false;
}

bool CariYonetimi::carininTumHareketleriniSil(Cari cari)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM faturalar WHERE cari = ?");
    query.bindValue(0, cari.getId());
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
        return false;
    }
    return true;
}

void CariYonetimi::caridenTahsilatYap(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama)
{
    QSqlQuery query(db);
    double guncelBorc = getCariToplamBorc(cariID);
    double odenenTutar = tutar;
    if(guncelBorc < odenenTutar){// tahsilat _tutar'ı büyükse toplam borçtan. falza tutarı carinin alacağına ekle.
        cariyiAlacaklandır(cariID, (odenenTutar - guncelBorc), tarih, 1, odemeTipi, islemYapanKullanici, evrakNo, aciklama);
        tutar = guncelBorc;
    }
    QString FaturaNo = faturaYonetimi.yeniFaturaNo();
    //yeni fatura bilgisi girme başlangıcı
    query.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bindValue(0, FaturaNo);
    query.bindValue(1, cariID);
    query.bindValue(2, faturaTipi);
    tarih.setTime(QTime::currentTime());
    query.bindValue(3, tarih);
    query.bindValue(4, islemYapanKullanici.getUserID());
    query.bindValue(5, 0);
    query.bindValue(6, tutar);
    query.bindValue(7, 0);
    query.bindValue(8, evrakNo);
    query.bindValue(9, aciklama);
    query.bindValue(10, odemeTipi);
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }

    // tahsilatı kasaya girme
    kasaYonetimi.KasaHareketiEkle(islemYapanKullanici, KasaYonetimi::KasaHareketi::Giris, tutar, "tahsilat yapıldı yapan:" + islemYapanKullanici.getAd(), tarih, 0);
}

void CariYonetimi::cariKartSil(QString cariID)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM carikartlar WHERE id = ?");
    query.bindValue(0, cariID);
    if(query.exec()){
        QMessageBox msg(0);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Cari kart silindi!");
        msg.setStandardButtons(QMessageBox::Ok);
        // msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
}

void CariYonetimi::cariyiAlacaklandır(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama)
{
    QSqlQuery query(db);
    QString FaturaNo = faturaYonetimi.yeniFaturaNo();
    //yeni fatura bilgisi girme başlangıcı
    query.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bindValue(0, FaturaNo);
    query.bindValue(1, cariID);
    query.bindValue(2, faturaTipi);
    tarih.setTime(QTime::currentTime());
    query.bindValue(3, tarih);
    query.bindValue(4, islemYapanKullanici.getUserID());
    query.bindValue(5, tutar);
    query.bindValue(6, 0);
    query.bindValue(7, tutar);
    query.bindValue(8, evrakNo);
    query.bindValue(9, aciklama);
    query.bindValue(10, odemeTipi);
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
}

void CariYonetimi::cariyeOdemeYap(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama)
{
    QSqlQuery query(db);
    QString FaturaNo = faturaYonetimi.yeniFaturaNo();
    //yeni fatura bilgisi girme başlangıcı
    query.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bindValue(0, FaturaNo);
    query.bindValue(1, cariID);
    query.bindValue(2, faturaTipi);
    tarih.setTime(QTime::currentTime());
    query.bindValue(3, tarih);
    query.bindValue(4, islemYapanKullanici.getUserID());
    query.bindValue(5, 0);
    query.bindValue(6, tutar);
    query.bindValue(7, 0);
    query.bindValue(8, evrakNo);
    query.bindValue(9, aciklama);
    query.bindValue(10, odemeTipi);
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
}

void CariYonetimi::cariyiBorclandir(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama)
{
    QSqlQuery query(db);
    QString FaturaNo = faturaYonetimi.yeniFaturaNo();
    //yeni fatura bilgisi girme başlangıcı
    query.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bindValue(0, FaturaNo);
    query.bindValue(1, cariID);
    query.bindValue(2, faturaTipi);
    tarih.setTime(QTime::currentTime());
    query.bindValue(3, tarih);
    query.bindValue(4, islemYapanKullanici.getUserID());
    query.bindValue(5, tutar);
    query.bindValue(6, 0);
    query.bindValue(7, tutar);
    query.bindValue(8, evrakNo);
    query.bindValue(9, aciklama);
    query.bindValue(10, odemeTipi);
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
}
