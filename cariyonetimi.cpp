#include "cariyonetimi.h"

CariYonetimi::CariYonetimi()
{

}

Cari CariYonetimi::getCariKart(QString cariID)
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
        return kart;
    }
    else{
        return kart;
    }
}

QList<Cari> CariYonetimi::getCariKartlar()
{
    QList<Cari> kartlar;
    sorgu.exec("SELECT * FROM carikartlar WHERE id NOT IN('1') ORDER BY ad ASC");
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    while (sorgu.next()) {
        Cari kart;
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
        kartlar.append(kart);
    }
    return kartlar;
}


QSqlQueryModel *CariYonetimi::getCariKartIsimleri()
{
    cariKartIsımleriModel->setQuery("SELECT carikartlar.id, ad, yetkili, caritipleri.tip, il, ilce, vergi_no, vergi_daire FROM carikartlar "
                                    "INNER JOIN caritipleri ON carikartlar.tip = caritipleri.id", db);
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
    sorgu.prepare("INSERT INTO carikartlar(id, ad, tip, vergi_no, vergi_daire, il, ilce, adres, mail, telefon, tarih, aciklama, yetkili) "
                  "VALUES(nextval('carikartlar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, cariKart.getAd());
    sorgu.bindValue(1, cariKart.getTip());
    sorgu.bindValue(2, cariKart.getVerigino());
    sorgu.bindValue(3, cariKart.getVergiDaire());
    sorgu.bindValue(4, cariKart.getIl());
    sorgu.bindValue(5, cariKart.getIlce());
    sorgu.bindValue(6, cariKart.getAdres());
    sorgu.bindValue(7, cariKart.getMail());
    sorgu.bindValue(8, cariKart.getTelefon());
    sorgu.bindValue(9, cariKart.getTarih());
    sorgu.bindValue(10, cariKart.getAciklama());
    sorgu.bindValue(11, cariKart.getYetkili());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
        return false;
    }
    return true;
}

bool CariYonetimi::cariKartDuzenle(Cari cariKart)
{
    sorgu.prepare("UPDATE carikartlar "
                    "SET ad = ?, tip = ?, vergi_no = ?, vergi_daire = ?, il = ?, ilce = ?, adres = ?, mail = ?, telefon = ?, aciklama = ?, yetkili = ? "
                    "WHERE id = ?") ;
    sorgu.bindValue(0, cariKart.getAd());
    sorgu.bindValue(1, cariKart.getTip());
    sorgu.bindValue(2, cariKart.getVerigino());
    sorgu.bindValue(3, cariKart.getVergiDaire());
    sorgu.bindValue(4, cariKart.getIl());
    sorgu.bindValue(5, cariKart.getIlce());
    sorgu.bindValue(6, cariKart.getAdres());
    sorgu.bindValue(7, cariKart.getMail());
    sorgu.bindValue(8, cariKart.getTelefon());
    sorgu.bindValue(9, QDateTime::currentDateTime().toString() + " <- tarihinde güncellendi.");
    sorgu.bindValue(10, cariKart.getYetkili());
    sorgu.bindValue(11, cariKart.getId());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
        return false;
    }
    return true;
}

QSqlQueryModel *CariYonetimi::getCariHareketleri(QString cariID)
{
    sorgu.prepare("SELECT fatura_no, faturalar.tarih, faturatipleri.tip, toplamtutar, odenentutar, kalantutar, odemetipleri.tip, kullanicilar.username, faturalar.aciklama FROM faturalar "
                    "INNER JOIN faturatipleri ON faturalar.tipi = faturatipleri.id "
                    "INNER JOIN odemetipleri ON faturalar.odemetipi = odemetipleri.id "
                    "INNER JOIN kullanicilar ON faturalar.kullanici = kullanicilar.id WHERE cari = ? ORDER BY faturalar.tarih DESC");
    sorgu.bindValue(0, cariID);
    sorgu.exec();
    cariHareketleriModel->setQuery(sorgu);
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
    QStringList cariTipList;
    sorgu.exec("SELECT * FROM caritipleri");
    while (sorgu.next()) {
        cariTipList.append(sorgu.value(1).toString());
    }
    return cariTipList;
}

QStringList CariYonetimi::getVergiDaireleri()
{
    QStringList liste;
    sorgu.exec("SELECT ad FROM vergidaireleri");
    while (sorgu.next()) {
        liste.append(sorgu.value(0).toString());
    }
    return liste;
}

double CariYonetimi::getCariToplamAlacak(QString cariID)
{
    if(cariID != "1"){// DİREKT CARİSİ İSE ES GEÇSİN
        double kalanTutar, odenenTutar;
        sorgu.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari = ? AND tipi = 1");
        sorgu.bindValue(0, cariID);
        sorgu.exec();
        if(sorgu.next()){
            kalanTutar = sorgu.value(0).toDouble();
        }
        else{
            kalanTutar = 0;
        }
        sorgu.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari = ? AND tipi = 4");
        sorgu.bindValue(0, cariID);
        sorgu.exec();
        if(sorgu.next()){
            odenenTutar = sorgu.value(0).toDouble();
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

double CariYonetimi::getCariToplamBorc(QString cariID, bool guncel)
{
    if(cariID != "1"){// DİREKT CARİSİ İSE ES GEÇSİN

        double kalanTutar, odenenTutar;

        if(!guncel){
            sorgu.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari = ? AND tipi = 2");
            sorgu.bindValue(0, cariID);
            sorgu.exec();
            if(sorgu.next()){
                kalanTutar = sorgu.value(0).toDouble();
            }
            else{
                kalanTutar = 0;
            }
            sorgu.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari = ? AND tipi = 5");
            sorgu.bindValue(0, cariID);
            sorgu.exec();
            if(sorgu.next()){
                odenenTutar = sorgu.value(0).toDouble();
            }
            else{
                odenenTutar = 0;
            }
            return kalanTutar - odenenTutar;
        }
        else{
            double guncelSepetToplamlari = 0;
            // sepetlerin güncel tutarlarının alınması.
            QSqlQuery sorguGuncelFiyat(db);
            sorguGuncelFiyat.prepare("SELECT fatura_no FROM faturalar WHERE cari = ? AND tipi = 2");
            sorguGuncelFiyat.bindValue(0, cariID);
            sorguGuncelFiyat.exec();
            if(sorguGuncelFiyat.isSelect()){
                while (sorguGuncelFiyat.next()) {
                    guncelSepetToplamlari += faturaYonetimi.getSatis(sorguGuncelFiyat.value(0).toString()).sepetToplamTutari();
                }
            }
            else{
                guncelSepetToplamlari = 0;
            }

            sorguGuncelFiyat.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari = ? AND tipi = 2");
            sorguGuncelFiyat.bindValue(0, cariID);
            sorguGuncelFiyat.exec();
            if(sorguGuncelFiyat.next()){
                kalanTutar = sorguGuncelFiyat.value(0).toDouble() + (guncelSepetToplamlari - sorguGuncelFiyat.value(0).toDouble());
            }
            else{
                kalanTutar = 0;
            }

            sorguGuncelFiyat.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari = ? AND tipi = 5");
            sorguGuncelFiyat.bindValue(0, cariID);
            sorguGuncelFiyat.exec();
            if(sorguGuncelFiyat.next()){
                odenenTutar = sorguGuncelFiyat.value(0).toDouble();
            }
            else{
                odenenTutar = 0;
            }
            return kalanTutar - odenenTutar;
        }
    }
    else{
        return 0;
    }
}

double CariYonetimi::getCarilerToplamAlacak()
{
    double kalanTutar, odenenTutar;
    sorgu.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 1");
    sorgu.exec();
    if(sorgu.next()){
        kalanTutar = sorgu.value(0).toDouble();
    }
    else{
        kalanTutar = 0;
    }
    sorgu.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 4");
    sorgu.exec();
    if(sorgu.next()){
        odenenTutar = sorgu.value(0).toDouble();
    }
    else{
        odenenTutar = 0;
    }
    return kalanTutar - odenenTutar;
}

double CariYonetimi::getcarilerToplamBorc(bool guncel, QDateTime startDate, QDateTime endDate)
{
    double kalanTutar = 0, odenenTutar = 0;
    if(!guncel){
        sorgu.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari NOT IN('1')  AND tipi = 2 AND tarih BETWEEN ? AND ?"); // cari 1 'e eşit olmayanları select ediyorum ki direkt carisini dahil etmesin.
        sorgu.bindValue(0, startDate);
        sorgu.bindValue(1, endDate);
        sorgu.exec();
        if(sorgu.next()){
            kalanTutar = sorgu.value(0).toDouble();
        }
        else{
            kalanTutar = 0;
        }
        sorgu.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 5 AND tarih BETWEEN ? AND ?"); // cari 1 'e eşit olmayanları select ediyorum ki direkt carisini dahil etmesin.
        sorgu.bindValue(0, startDate);
        sorgu.bindValue(1, endDate);
        sorgu.exec();
        if(sorgu.next()){
            odenenTutar = sorgu.value(0).toDouble();
        }
        else{
            odenenTutar = 0;
        }
        return kalanTutar - odenenTutar;
    }
    else{               //********* sepetlerin güncel tutarlarının alınması. *************//
        double guncelSepetToplamlari = 0;

        QSqlQuery sorguGuncelFiyat(db);
        sorguGuncelFiyat.prepare("SELECT fatura_no FROM faturalar WHERE cari NOT IN('1') AND tipi = 2 AND kalantutar > 0 AND tarih BETWEEN ? AND ?");
        sorguGuncelFiyat.bindValue(0, startDate);
        sorguGuncelFiyat.bindValue(1, endDate);
        sorguGuncelFiyat.exec();
        if(sorguGuncelFiyat.isSelect()){

            QStringList faturalar;
            while (sorguGuncelFiyat.next()) {
                faturalar.append(sorguGuncelFiyat.value(0).toString());
            }

            foreach (auto fatura, faturalar) {
                guncelSepetToplamlari += faturaYonetimi.getSatis(fatura).sepetToplamTutari();
            }

            sorguGuncelFiyat.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 2 AND kalantutar > 0 AND tarih BETWEEN ? AND ?");
            sorguGuncelFiyat.bindValue(0, startDate);
            sorguGuncelFiyat.bindValue(1, endDate);
            sorguGuncelFiyat.exec();
            if(sorguGuncelFiyat.next()){
                kalanTutar = sorguGuncelFiyat.value(0).toDouble() + (guncelSepetToplamlari - sorguGuncelFiyat.value(0).toDouble());
            }
            else{
                kalanTutar = 0;
            }

            sorguGuncelFiyat.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari NOT IN('1') AND tipi = 5 AND kalantutar > 0 AND tarih BETWEEN ? AND ?");
            sorguGuncelFiyat.bindValue(0, startDate);
            sorguGuncelFiyat.bindValue(1, endDate);
            sorguGuncelFiyat.exec();
            if(sorguGuncelFiyat.next()){
                odenenTutar = sorguGuncelFiyat.value(0).toDouble();
            }
            else{
                odenenTutar = 0;
            }
        }
    }
    return kalanTutar - odenenTutar;
}

bool CariYonetimi::cariHareketiSil(QString faturaNo, User kullanici, Cari cari)
{
    //    3 ana adım
    //    faturanın silinmesi
    //    sepetteki ürünlerin stoğa geri eklenmesi
    //    kasa hareketlerine girilmesi

        //satış faturası iade alınmamışsa
        if(!faturaYonetimi.iadeAlinmismi(faturaNo)){
            // faturadaki ürünlerin stoğa geri eklenmesi
            Sepet sepet = faturaYonetimi.getSatis(faturaNo);
            //satılan sepetteki ürünleri stoğa geri ekle
            foreach (auto urun, sepet.urunler) {
                stokYonetimi.setStokMiktari(kullanici, urun.ID, "GİRİŞ", urun.miktar);// stok hareketlerine de ekliyor.
            }
            // kasahareketine - para girişi
            kasaYonetimi.KasaHareketiEkle(kullanici, "ÇIKIŞ", sepet.getOdenenTutar(), QString("cari hareket silme:%1").arg(cari.getId()), QDateTime::currentDateTime(), sepet.getSepettekiKazanc());// kasaya para giriş/çıkış da yapar.
            //faturanın silinmesi
            faturaYonetimi.faturayiSil(faturaNo);
            return true;
        }
        return false;
}

void CariYonetimi::caridenTahsilatYap(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama)
{
    double guncelBorc = getCariToplamBorc(cariID, false);
    double odenenTutar = tutar;
    if(guncelBorc < odenenTutar){// tahsilat _tutar'ı büyükse toplam borçtan. falza tutarı carinin alacağına ekle.
        cariyiAlacaklandır(cariID, (odenenTutar - guncelBorc), tarih, 1, odemeTipi, islemYapanKullanici, evrakNo, aciklama);
        tutar = guncelBorc;
    }
    QString FaturaNo = faturaYonetimi.yeniFaturaNo();
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, FaturaNo);
    sorgu.bindValue(1, cariID);
    sorgu.bindValue(2, faturaTipi);
    tarih.setTime(QTime::currentTime());
    sorgu.bindValue(3, tarih);
    sorgu.bindValue(4, islemYapanKullanici.getUserID());
    sorgu.bindValue(5, 0);
    sorgu.bindValue(6, tutar);
    sorgu.bindValue(7, 0);
    sorgu.bindValue(8, evrakNo);
    sorgu.bindValue(9, aciklama);
    sorgu.bindValue(10, odemeTipi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
}

void CariYonetimi::cariKartSil(QString cariID)
{
    sorgu.prepare("DELETE FROM carikartlar WHERE id = ?");
    sorgu.bindValue(0, cariID);
    if(sorgu.exec()){
        QMessageBox msg(0);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Cari kart silindi!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
}

void CariYonetimi::cariyiAlacaklandır(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama)
{
    QString FaturaNo = faturaYonetimi.yeniFaturaNo();
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, FaturaNo);
    sorgu.bindValue(1, cariID);
    sorgu.bindValue(2, faturaTipi);
    tarih.setTime(QTime::currentTime());
    sorgu.bindValue(3, tarih);
    sorgu.bindValue(4, islemYapanKullanici.getUserID());
    sorgu.bindValue(5, tutar);
    sorgu.bindValue(6, 0);
    sorgu.bindValue(7, tutar);
    sorgu.bindValue(8, evrakNo);
    sorgu.bindValue(9, aciklama);
    sorgu.bindValue(10, odemeTipi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
}

void CariYonetimi::cariyeOdemeYap(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama)
{
    QString FaturaNo = faturaYonetimi.yeniFaturaNo();
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, FaturaNo);
    sorgu.bindValue(1, cariID);
    sorgu.bindValue(2, faturaTipi);
    tarih.setTime(QTime::currentTime());
    sorgu.bindValue(3, tarih);
    sorgu.bindValue(4, islemYapanKullanici.getUserID());
    sorgu.bindValue(5, 0);
    sorgu.bindValue(6, tutar);
    sorgu.bindValue(7, 0);
    sorgu.bindValue(8, evrakNo);
    sorgu.bindValue(9, aciklama);
    sorgu.bindValue(10, odemeTipi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
}

void CariYonetimi::cariyiBorclandir(QString cariID, double tutar, QDateTime tarih, int faturaTipi, int odemeTipi, User islemYapanKullanici, QString evrakNo, QString aciklama)
{
    QString FaturaNo = faturaYonetimi.yeniFaturaNo();
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, FaturaNo);
    sorgu.bindValue(1, cariID);
    sorgu.bindValue(2, faturaTipi);
    tarih.setTime(QTime::currentTime());
    sorgu.bindValue(3, tarih);
    sorgu.bindValue(4, islemYapanKullanici.getUserID());
    sorgu.bindValue(5, tutar);
    sorgu.bindValue(6, 0);
    sorgu.bindValue(7, tutar);
    sorgu.bindValue(8, evrakNo);
    sorgu.bindValue(9, aciklama);
    sorgu.bindValue(10, odemeTipi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
}
