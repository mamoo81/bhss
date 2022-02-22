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
#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>

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
    sorgu.prepare("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, yeniFaturaNo);
    sorgu.bindValue(1, _satisYapilanCariID);
    sorgu.bindValue(2, 2);// 2 = satış faturası (veritabanında faturatipleri.tip)
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, _satisYapanKullanici.getUserID());
    sorgu.bindValue(5, _satilacakSepet.sepetToplamTutari());
    sorgu.bindValue(6, _satilacakSepet.getOdenenTutar());
    sorgu.bindValue(7, _satilacakSepet.getKalanTutar());
    sorgu.bindValue(8, 1);//nakit ödeme tipinde satış
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    //kasa hareketi girme ve kasaya para giriş/çıkış
    KasaHareketiEkle(_satisYapanKullanici,
                     "GİRİŞ",
                     _satilacakSepet.sepetToplamTutari(),
                     "SATIŞ FAT.NO:" + yeniFaturaNo,
                     QDateTime::currentDateTime(),
                     _satilacakSepet.getSepettekiKazanc());
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
    sorgu.exec("SELECT fatura_no, tarih FROM faturalar WHERE tarih::date = now()::date AND tipi = 2 ORDER BY tarih DESC");
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

double Veritabani::getNetKar(QDateTime _baslangicTarih, QDateTime _bitisTarih)
{
    sorgu.prepare("SELECT SUM(CAST(kar AS DECIMAL)) FROM kasahareketleri WHERE tarih BETWEEN ? AND ?");
    sorgu.bindValue(0, _baslangicTarih);
    sorgu.bindValue(1, _bitisTarih);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
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

double Veritabani::getKasaToplamGiren(QDateTime _baslangicTarih, QDateTime _bitisTarih)
{
    sorgu.prepare("SELECT SUM(CAST(miktar AS DECIMAL)) FROM kasahareketleri WHERE islem = 'GİRİŞ' AND tarih BETWEEN ? AND ?");
    sorgu.bindValue(0, _baslangicTarih);
    sorgu.bindValue(1, _bitisTarih);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
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

double Veritabani::getKasaToplamCikan(QDateTime _baslangicTarih, QDateTime _bitisTarih)
{
    sorgu.prepare("SELECT SUM(CAST(miktar AS DECIMAL)) FROM kasahareketleri WHERE islem IN ('ÇIKIŞ','İADE') AND tarih BETWEEN ? AND ?");
    sorgu.bindValue(0, _baslangicTarih);
    sorgu.bindValue(1, _bitisTarih);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
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

QSqlQueryModel *Veritabani::getKasaHareketleri(QDateTime _baslangicTarih, QDateTime _bitisTarih)
{
    sorgu.prepare("SELECT kasahareketleri.id, islem, CAST(miktar AS DECIMAL), kasahareketleri.tarih, kullanicilar.username, evrakno, kasahareketleri.aciklama FROM kasahareketleri "
                    "INNER JOIN kullanicilar ON kasahareketleri.kullanici = kullanicilar.id "
                    "WHERE kasahareketleri.tarih BETWEEN ? AND ? "
                    "ORDER BY kasahareketleri.id DESC");
    sorgu.bindValue(0, _baslangicTarih);
    sorgu.bindValue(1, _bitisTarih);
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
        qFatal(sorgu.lastError().text().toStdString().c_str());
        return NULL;
    }
    else{
        return kasaHareketlerimodel;
    }
}

int Veritabani::kasaHareketiDuzenle(User _user, QString _hareketID, QString _hareket, double _tutar, QString _aciklama, QDateTime _tarih, QString _evrakNo)
{
    //KASA HAREKETİNİ DÜZELTMEDEN ÖNCE, düzeltilecek işlemin şuan ki HAREKET TUTARINI AL. metodun EN SON da +/- YÖNDE kasadaki paraya ekle
    sorgu.prepare("SELECT islem, CAST(miktar AS DECIMAL) FROM kasahareketleri WHERE id = ?");
    sorgu.bindValue(0, _hareketID);
    sorgu.exec();
    double oncekiTutar = 0;
    QString oncekiHareket;
    if(sorgu.next()){
        oncekiTutar = sorgu.value(1).toDouble();
        oncekiHareket = sorgu.value(0).toString();
    }
    //----------------------------------------
    //KASA HAREKETLERİ TABLOSUNU DÜZELTME
    sorgu.prepare("UPDATE kasahareketleri "
                    "SET miktar = ?, kullanici = ?, islem = ?, tarih = ?, evrakno = ?, aciklama = ?");
    sorgu.bindValue(0, _tutar);
    sorgu.bindValue(1, _user.getUserID());
    sorgu.bindValue(2, _hareket);
    sorgu.bindValue(3, _tarih);
    sorgu.bindValue(4, _evrakNo);
    sorgu.bindValue(5, "İŞLEM DÜZELTME:" + _aciklama);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
    }
    // DÜZELTME YAPILACAK HAREKET DEĞİŞTİ İSE TUTARI KASAda ki paraya EKLE/ÇIKAR
    if(_hareket != oncekiHareket){
        if(oncekiHareket == "GİRİŞ"){
            double suankiPara = getKasadakiPara();
            double guncelPara = suankiPara - oncekiTutar;
            sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
            sorgu.bindValue(0, guncelPara);
            sorgu.exec();
            guncelPara = getKasadakiPara();
            guncelPara -= _tutar;
            sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
            sorgu.bindValue(0, guncelPara);
            sorgu.exec();
        }
        else if(oncekiHareket == "ÇIKIŞ"){
            double suankiPara = getKasadakiPara();
            double guncelPara = suankiPara + oncekiTutar;
            sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
            sorgu.bindValue(0, guncelPara);
            sorgu.exec();
            guncelPara = getKasadakiPara();
            guncelPara += _tutar;
            sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
            sorgu.bindValue(0, guncelPara);
            sorgu.exec();
        }
    }
    return 1;
}

bool Veritabani::kasaHareketiSil(QString _hareketID, QString _hareket, double _tutar)
{
    // kasahareketleri tablosundan kayıdı silme
    sorgu.prepare("DELETE FROM kasahareketleri WHERE id = ?");
    sorgu.bindValue(0, _hareketID);
    sorgu.exec();
    //kasadan silinen işlemin parasını düşme.
    if(_hareket == "GİRİŞ"){
        double suankiPara = getKasadakiPara();
        double guncelPara = suankiPara - _tutar;
        sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
        sorgu.bindValue(0, guncelPara);
        sorgu.exec();
    }
    else if(_hareket == "ÇIKIŞ"){
        double suankiPara = getKasadakiPara();
        double guncelPara = suankiPara + _tutar;
        sorgu.prepare("UPDATE kasa SET para = ? WHERE id = 1");
        sorgu.bindValue(0, guncelPara);
        sorgu.exec();
    }
    return true;
}

bool Veritabani::veritabaniYedekle(QString _dirNameAndFileName)
{
    QString yedeklemeKomutu = "pg_dump -Fc -U postgres mhss_data > ";
    yedeklemeKomutu += _dirNameAndFileName + ".dump";
    int exitCode = system(qPrintable(yedeklemeKomutu));
    if(exitCode == QProcess::NormalExit && exitCode == QProcess::NormalExit){// system() ile gönderdiğim komut normal olarak bittiyse
        // komut başarılı
        return true;
    } else {
        // komut başarısız.
        return false;
    }
}

bool Veritabani::veritabaniYedektenGeriYukle(QString _dosyaYolu)
{
    db.close();
    db.setDatabaseName("postgres");
    //veritabanını silme
    db.open();
    QSqlQuery yedekSorgu = QSqlQuery(db);
    yedekSorgu.exec("DROP DATABASE mhss_data");
    if(!QString(yedekSorgu.lastError().text()).isEmpty()){
        qDebug() << yedekSorgu.lastError().text();
    }
    //veritabanını oluşturma
    yedekSorgu.exec("CREATE DATABASE mhss_data OWNER postgres");
    if(!QString(yedekSorgu.lastError().text()).isEmpty()){
        qDebug() << yedekSorgu.lastError().text();
    }
    db.close();
    db.setDatabaseName("mhss_data");
    db.open();
    QString geriYuklemeKomutu = "pg_restore -U postgres -d mhss_data " + _dosyaYolu;
    int exitCode = system(qPrintable(geriYuklemeKomutu));
    if(exitCode == QProcess::NormalExit){// system() ile gönderdiğim komut süreci normal olarak bittiyse
        // komut başarılı
        return true;
    } else {
        // komut başarısız.
        return false;
    }
}

bool Veritabani::veritabaniSifirla()
{
    // veritabani sıfırlandığı için hizli butonları da sıfırla
    QSettings hizlibutonlariINI(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/hizlibutonlar.ini", QSettings::IniFormat);
    hizlibutonlariINI.clear();
    QFile mhss_data_sifir(":/dosyalar/dosyalar/mhss_data_sifir.dump");
    if(!QFileInfo().exists("/tmp/mhss_data_sifir.dump")){
        mhss_data_sifir.copy("/tmp/mhss_data_sifir.dump");
    }
    QFile dosya("/tmp/mhss_data_sifir.dump");
    qDebug() << QFileInfo(dosya).absoluteFilePath();
    if(veritabaniYedektenGeriYukle(QFileInfo(dosya).absoluteFilePath())){
        return true;
    }
    else{
        return false;
    }
}

void Veritabani::setHizliButon(StokKarti _stokKarti)
{
    QSettings hizliButonBarkodlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/hizlibutonlar.ini", QSettings::IniFormat);
    foreach (QString buttonName, hizliButonBarkodlar.childGroups()) {
        hizliButonBarkodlar.beginGroup(buttonName);
        QString duzenlenecekStokID = hizliButonBarkodlar.value("stokid").toString();
        if(duzenlenecekStokID == _stokKarti.getId()){
            hizliButonBarkodlar.setValue("barkod", _stokKarti.getBarkod());
            hizliButonBarkodlar.setValue("ad", _stokKarti.getAd());
        }
        hizliButonBarkodlar.endGroup();
    }
}

void Veritabani::setOturum(User _user)
{
    sorgu.exec("SELECT * FROM oturum");
    if(sorgu.next()){
        if(sorgu.value(1).toString() != _user.getUserName()){
            sorgu.prepare("UPDATE oturum SET username = ?, giristarihi = ? WHERE id = 1");
            sorgu.bindValue(0, _user.getUserName());
            sorgu.bindValue(1, QDateTime::currentDateTime());
            sorgu.exec();
            if(sorgu.lastError().isValid()){
                qCritical(qPrintable(sorgu.lastError().text()));
            }
        }
    }
    else{
        sorgu.prepare("INSERT INTO oturum(id, username, giristarihi) VALUES('1', ?,?)");
        sorgu.bindValue(0, _user.getUserName());
        sorgu.bindValue(1, QDateTime::currentDateTime());
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qCritical(qPrintable(sorgu.lastError().text()));
        }
    }
}

QSqlQuery Veritabani::getOturum()
{
    sorgu.exec("SELECT * FROM oturum");
    if(!sorgu.next()){
        return QSqlQuery();
    }
    return sorgu;
}

void Veritabani::oturumSonlandir()
{
    sorgu.exec("DELETE FROM oturum WHERE id = 1");
    if(sorgu.lastError().isValid()){
        qCritical(qPrintable(sorgu.lastError().text()));
    }
}

bool Veritabani::veritabaniVarmi()
{
    //mhss_data veritabanı varmı kontrol
    sorgu.exec("SELECT datname FROM pg_database WHERE datname = 'mhss_data'");
    if(sorgu.next()){
        db.setDatabaseName("mhss_data");
        db.open();
        return true;
    }
    else{
        return false;
    }
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
        u.setPassWord(sorgu.value(2).toString());
        u.setAd(sorgu.value(3).toString());
        u.setSoyad(sorgu.value(4).toString());
        u.setCepNo(sorgu.value(5).toString());
        u.setTarih(sorgu.value(6).toDateTime());
        u.setKasaYetki(sorgu.value(7).toBool());
        u.setIadeYetki(sorgu.value(8).toBool());
        u.setStokYetki(sorgu.value(9).toBool());
        u.setAyaryetki(sorgu.value(10).toBool());
        u.setCariyetki(sorgu.value(11).toBool());
    }
    return u;
}

void Veritabani::updateUser(User _NewUserInfos)
{
    sorgu.prepare("UPDATE kullanicilar SET password = ?, ad = ?, soyad = ?, cepno = ?, tarih = ?, kasayetki = ?, iadeyetki = ?, stokyetki = ?, ayaryetki = ?, cariyetki = ? "
                    "WHERE id = ?");
    sorgu.bindValue(0, _NewUserInfos.getPassWord());
    sorgu.bindValue(1, _NewUserInfos.getAd());
    sorgu.bindValue(2, _NewUserInfos.getSoyad());
    sorgu.bindValue(3, _NewUserInfos.getCepNo());
    sorgu.bindValue(4, _NewUserInfos.getTarih());
    sorgu.bindValue(5, _NewUserInfos.getKasaYetki());
    sorgu.bindValue(6, _NewUserInfos.getIadeYetki());
    sorgu.bindValue(7, _NewUserInfos.getStokYetki());
    sorgu.bindValue(8, _NewUserInfos.getAyaryetki());
    sorgu.bindValue(9, _NewUserInfos.getCariyetki());
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
    sorgu.prepare("INSERT INTO kullanicilar(id, username, password, ad, soyad, cepno, tarih, kasayetki, iadeyetki, stokyetki, ayaryetki, cariyetki)"
                    " VALUES(nextval('kullanicilar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, _NewUser.getUserName());
    sorgu.bindValue(1, _NewUser.getPassWord());
    sorgu.bindValue(2, _NewUser.getAd());
    sorgu.bindValue(3, _NewUser.getSoyad());
    sorgu.bindValue(4, _NewUser.getCepNo());
    sorgu.bindValue(5, _NewUser.getTarih());
    sorgu.bindValue(6, _NewUser.getKasaYetki());
    sorgu.bindValue(7, _NewUser.getIadeYetki());
    sorgu.bindValue(8, _NewUser.getStokYetki());
    sorgu.bindValue(9, _NewUser.getAyaryetki());
    sorgu.bindValue(10, _NewUser.getCariyetki());
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

QString Veritabani::sonIslemNumarasi()
{
    sorgu.exec("SELECT fatura_no, tarih FROM faturalar ORDER BY tarih DESC LIMIT 1");
    sorgu.next();
    return sorgu.value(0).toString();
}

QList<Cari> Veritabani::getCariKartlar()
{
    QList<Cari> kartlar;
    sorgu.exec("SELECT * FROM carikartlar");
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

QStringList Veritabani::getOdemeTipleri()
{
    QStringList odemeTipleri;
    sorgu.exec("SELECT * FROM odemetipleri");
    while (sorgu.next()) {
        odemeTipleri.append(sorgu.value(1).toString());
    }
    return odemeTipleri;
}

void Veritabani::caridenTahsilatYap(QString _cariID,
                                    double _tutar,
                                    QDateTime _tarih,
                                    int _faturaTipi,
                                    int _odemeTipi,
                                    User _islemYapanKullanici,
                                    QString _evrakNo,
                                    QString _aciklama)
{
    //yeni fatura numarası için faturalar_sequence'den son değeri alma
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.next();
    QString yeniFaturaNo = QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt() + 1);
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, yeniFaturaNo);
    sorgu.bindValue(1, _cariID);
    sorgu.bindValue(2, _faturaTipi);
    _tarih.setTime(QTime::currentTime());
    sorgu.bindValue(3, _tarih);
    sorgu.bindValue(4, _islemYapanKullanici.getUserID());
    sorgu.bindValue(5, 0);
    sorgu.bindValue(6, _tutar);
    sorgu.bindValue(7, 0);
    sorgu.bindValue(8, _evrakNo);
    sorgu.bindValue(9, _aciklama);
    sorgu.bindValue(10, _odemeTipi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
    }
}

void Veritabani::cariyeOdemeYap(QString _cariID,
                                    double _tutar,
                                    QDateTime _tarih,
                                    int _faturaTipi,
                                    int _odemeTipi,
                                    User _islemYapanKullanici,
                                    QString _evrakNo,
                                    QString _aciklama)
{
    //yeni fatura numarası için faturalar_sequence'den son değeri alma
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.next();
    QString yeniFaturaNo = QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt() + 1);
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, yeniFaturaNo);
    sorgu.bindValue(1, _cariID);
    sorgu.bindValue(2, _faturaTipi);
    _tarih.setTime(QTime::currentTime());
    sorgu.bindValue(3, _tarih);
    sorgu.bindValue(4, _islemYapanKullanici.getUserID());
    sorgu.bindValue(5, 0);
    sorgu.bindValue(6, _tutar);
    sorgu.bindValue(7, 0);
    sorgu.bindValue(8, _evrakNo);
    sorgu.bindValue(9, _aciklama);
    sorgu.bindValue(10, _odemeTipi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
    }
}

void Veritabani::cariyiAlacaklandır(QString _cariID, double _tutar, QDateTime _tarih, int _faturaTipi, int _odemeTipi, User _islemYapanKullanici, QString _evrakNo, QString _aciklama)
{
    //yeni fatura numarası için faturalar_sequence'den son değeri alma
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.next();
    QString yeniFaturaNo = QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt() + 1);
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, yeniFaturaNo);
    sorgu.bindValue(1, _cariID);
    sorgu.bindValue(2, _faturaTipi);
    _tarih.setTime(QTime::currentTime());
    sorgu.bindValue(3, _tarih);
    sorgu.bindValue(4, _islemYapanKullanici.getUserID());
    sorgu.bindValue(5, _tutar);
    sorgu.bindValue(6, 0);
    sorgu.bindValue(7, _tutar);
    sorgu.bindValue(8, _evrakNo);
    sorgu.bindValue(9, _aciklama);
    sorgu.bindValue(10, _odemeTipi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
    }
}

void Veritabani::cariyiBorclandir(QString _cariID, double _tutar, QDateTime _tarih, int _faturaTipi, int _odemeTipi, User _islemYapanKullanici, QString _evrakNo, QString _aciklama)
{
    //yeni fatura numarası için faturalar_sequence'den son değeri alma
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    sorgu.next();
    QString yeniFaturaNo = QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt() + 1);
    //yeni fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar(id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odenentutar, kalantutar, evrakno, aciklama, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, yeniFaturaNo);
    sorgu.bindValue(1, _cariID);
    sorgu.bindValue(2, _faturaTipi);
    _tarih.setTime(QTime::currentTime());
    sorgu.bindValue(3, _tarih);
    sorgu.bindValue(4, _islemYapanKullanici.getUserID());
    sorgu.bindValue(5, _tutar);
    sorgu.bindValue(6, 0);
    sorgu.bindValue(7, _tutar);
    sorgu.bindValue(8, _evrakNo);
    sorgu.bindValue(9, _aciklama);
    sorgu.bindValue(10, _odemeTipi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
    }
}

QSqlQueryModel* Veritabani::getCariKartIsimleri()
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

QSqlQueryModel *Veritabani::getCariHareketleri(QString _cariID)
{
    sorgu.prepare("SELECT fatura_no, faturalar.tarih, faturatipleri.tip, toplamtutar, odenentutar, kalantutar, odemetipleri.tip, kullanicilar.username, faturalar.aciklama FROM faturalar "
                    "INNER JOIN faturatipleri ON faturalar.tipi = faturatipleri.id "
                    "INNER JOIN odemetipleri ON faturalar.odemetipi = odemetipleri.id "
                    "INNER JOIN kullanicilar ON faturalar.kullanici = kullanicilar.id WHERE cari = ? ORDER BY faturalar.tarih DESC");
    sorgu.bindValue(0, _cariID);
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

Cari Veritabani::getCariKart(QString _cariID)
{
    Cari kart;
    sorgu.prepare("SELECT * FROM carikartlar WHERE id = ?");
    sorgu.bindValue(0, _cariID);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
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

QStringList Veritabani::getCariTipleri()
{
    QStringList cariTipList;
    sorgu.exec("SELECT * FROM caritipleri");
    while (sorgu.next()) {
        cariTipList.append(sorgu.value(1).toString());
    }
    return cariTipList;
}

double Veritabani::getCariToplamAlacak(QString _cariID)
{
    if(_cariID != "1000"){// DİREKT CARİSİ İSE ES GEÇSİN
        double kalanTutar, odenenTutar;
        sorgu.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari = ? AND tipi = 1");
        sorgu.bindValue(0, _cariID);
        sorgu.exec();
        if(sorgu.next()){
            kalanTutar = sorgu.value(0).toDouble();
        }
        else{
            kalanTutar = 0;
        }
        sorgu.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari = ? AND tipi = 4");
        sorgu.bindValue(0, _cariID);
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

double Veritabani::getCariToplamBorc(QString _cariID)
{
    if(_cariID != "1000"){// DİREKT CARİSİ İSE ES GEÇSİN
        double kalanTutar, odenenTutar;
        sorgu.prepare("SELECT SUM(kalantutar) FROM faturalar WHERE cari = ? AND tipi = 2");
        sorgu.bindValue(0, _cariID);
        sorgu.exec();
        if(sorgu.next()){
            kalanTutar = sorgu.value(0).toDouble();
        }
        else{
            kalanTutar = 0;
        }
        sorgu.prepare("SELECT SUM(odenentutar) FROM faturalar WHERE cari = ? AND tipi = 5");
        sorgu.bindValue(0, _cariID);
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

void Veritabani::yeniCariKart(Cari _cariKart)
{
    sorgu.prepare("INSERT INTO carikartlar(id, ad, tip, vergi_no, vergi_daire, il, ilce, adres, mail, telefon, tarih, aciklama, yetkili)"
                    "VALUES (nextval('carikartlar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, _cariKart.getAd());
    sorgu.bindValue(1, _cariKart.getTip());
    sorgu.bindValue(2, _cariKart.getVerigino());
    sorgu.bindValue(3, _cariKart.getVergiDaire());
    sorgu.bindValue(4, _cariKart.getIl());
    sorgu.bindValue(5, _cariKart.getIlce());
    sorgu.bindValue(6, _cariKart.getAdres());
    sorgu.bindValue(7, _cariKart.getMail());
    sorgu.bindValue(8, _cariKart.getTelefon());
    sorgu.bindValue(9, _cariKart.getTarih());
    sorgu.bindValue(10, _cariKart.getAciklama());
    sorgu.bindValue(11, _cariKart.getYetkili());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
    }
    else{
        QMessageBox msg;
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Yeni cari kart oluşturuldu.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

bool Veritabani::cariKartDuzenle(Cari _cariKart)
{
    sorgu.prepare("UPDATE carikartlar "
                    "SET ad = ?, tip = ?, vergi_no = ?, vergi_daire = ?, il = ?, ilce = ?, adres = ?, mail = ?, telefon = ?, aciklama = ?, yetkili = ? "
                    "WHERE id = ?") ;
    sorgu.bindValue(0, _cariKart.getAd());
    sorgu.bindValue(1, _cariKart.getTip());
    sorgu.bindValue(2, _cariKart.getVerigino());
    sorgu.bindValue(3, _cariKart.getVergiDaire());
    sorgu.bindValue(4, _cariKart.getIl());
    sorgu.bindValue(5, _cariKart.getIlce());
    sorgu.bindValue(6, _cariKart.getAdres());
    sorgu.bindValue(7, _cariKart.getMail());
    sorgu.bindValue(8, _cariKart.getTelefon());
    sorgu.bindValue(9, QDateTime::currentDateTime().toString() + " <- tarihinde güncellendi.");
    sorgu.bindValue(10, _cariKart.getYetkili());
    sorgu.bindValue(11, _cariKart.getId());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
        return false;
    }
    return true;
}

void Veritabani::cariKartSil(QString _cariID)
{
    sorgu.prepare("DELETE FROM carikartlar WHERE id = ?");
    sorgu.bindValue(0, _cariID);
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
        qFatal(sorgu.lastError().text().toStdString().c_str());
    }
}

QStringList Veritabani::getIller()
{
    QStringList ilList;
    ilList.append("İl seçiniz.");
    sorgu.exec("SELECT il FROM iller");
    while (sorgu.next()) {
        ilList.append(sorgu.value(0).toString());
    }
    return ilList;
}

QStringList Veritabani::getIlceler(int _plaka)
{
    QStringList ilcelerList;
    ilcelerList.append("İlçe seçiniz.");
    sorgu.prepare("SELECT ilce FROM ilceler WHERE ilid = ?");
    sorgu.bindValue(0, _plaka);
    sorgu.exec();
    while (sorgu.next()) {
        ilcelerList.append(sorgu.value(0).toString());
    }
    return ilcelerList;
}

QStringList Veritabani::GetUsers()
{
    QStringList users;
    sorgu.exec("SELECT username FROM kullanicilar");
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
    while (sorgu.next()) {
        users << sorgu.value(0).toString();
    }
    return users;
}

StokKarti Veritabani::getStokKarti(QString _Barkod)
{
    StokKarti kart = StokKarti();
    sorgu.prepare("SELECT id, barkod, kod, ad, birim, miktar, grup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv, otv, kdvdahil, otvdahil, tarih, aciklama FROM stokkartlari WHERE barkod = ?");
    sorgu.bindValue(0, _Barkod);
    sorgu.exec();
    if(sorgu.next()){
        kart.setId(sorgu.value(0).toString());
        kart.setBarkod(sorgu.value(1).toString());
        kart.setKod(sorgu.value(2).toString());
        kart.setAd(sorgu.value(3).toString());
        kart.setBirim(sorgu.value(4).toString());
        kart.setMiktar(sorgu.value(5).toFloat());
        kart.setGrup(sorgu.value(6).toString());
        kart.setAFiyat(sorgu.value(7).toDouble());
        kart.setSFiyat(sorgu.value(8).toDouble());
        kart.setKdv(sorgu.value(9).toInt());
        kart.setOtv(sorgu.value(10).toInt());
        kart.setKdvdahil(sorgu.value(11).toBool());
        kart.setOtvdahil(sorgu.value(12).toBool());
        kart.setTarih(sorgu.value(13).toDateTime());
        kart.setAciklama(sorgu.value(14).toString());
    }
    return kart;
}

bool Veritabani::setStokMiktari(User _kullanici, QString _stokKartiID, QString _islem, float _Miktar)
{
    float mevcutStokMiktari = 0;
    sorgu.prepare("SELECT miktar FROM stokkartlari WHERE id = ?");
    sorgu.bindValue(0, _stokKartiID);
    sorgu.exec();
    if(sorgu.next()){
        mevcutStokMiktari = sorgu.value(0).toFloat();
    }
    QString barkod;
    sorgu.prepare("SELECT barkod FROM stokkartlari WHERE id = ?");
    sorgu.bindValue(0, _stokKartiID);
    sorgu.exec();
    if(sorgu.next()){
        barkod = sorgu.value(0).toString();
    }
    if(_islem == "GİRİŞ"){
        sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE id = ?");
        sorgu.bindValue(0, mevcutStokMiktari + _Miktar);
        sorgu.bindValue(1, _stokKartiID);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << "stok kartı miktar ekleme/çıkarma hatası:\n" << sorgu.lastError().text();
        }
        stokHareketiEkle(_kullanici, barkod, _islem, _Miktar);
        return true;
    }
    else if(_islem == "ÇIKIŞ"){
        if(mevcutStokMiktari >= _Miktar){
            sorgu.prepare("UPDATE stokkartlari SET miktar = ? WHERE id = ?");
            sorgu.bindValue(0, (mevcutStokMiktari - _Miktar));
            sorgu.bindValue(1, _stokKartiID);
            sorgu.exec();
            if(sorgu.lastError().isValid()){
                qDebug() << "stok kartı miktar ekleme/çıkarma hatası:\n" << sorgu.lastError().text();
            }
            stokHareketiEkle(_kullanici, barkod, _islem, _Miktar);
            return true;
        }
        else{
            QMessageBox msg(0);
            msg.setWindowTitle("Uyari");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Stoktan düşülecek miktar stok miktarından fazla olamaz.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            return false;
        }
    }
}

void Veritabani::yeniStokKartiOlustur(StokKarti _StokKarti, User *_Kullanici)
{
    sorgu.prepare("INSERT INTO stokkartlari (id, barkod, kod, ad, birim, miktar, grup, afiyat, sfiyat, kdv, otv, kdvdahil, otvdahil, tarih, aciklama) "
                    "VALUES (nextval('stokkartlari_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, _StokKarti.getBarkod());
    sorgu.bindValue(1, _StokKarti.getKod());
    sorgu.bindValue(2, _StokKarti.getAd());
    sorgu.bindValue(3, _StokKarti.getBirim());
    sorgu.bindValue(4, _StokKarti.getMiktar());
    sorgu.bindValue(5, _StokKarti.getGrup());
    sorgu.bindValue(6, _StokKarti.getAFiyat());
    sorgu.bindValue(7, _StokKarti.getSFiyat());
    sorgu.bindValue(8, _StokKarti.getKdv());
    sorgu.bindValue(9, _StokKarti.getOtv());
    sorgu.bindValue(10, _StokKarti.getKdvdahil());
    sorgu.bindValue(11, _StokKarti.getOtvdahil());
    sorgu.bindValue(12, _StokKarti.getTarih());
    sorgu.bindValue(13, _StokKarti.getAciklama() + " [" + _Kullanici->getUserName() + "]");
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
    sorgu.prepare("UPDATE stokkartlari SET barkod = ?, kod = ?, ad = ?, birim = ?, miktar = ?, grup = ?, afiyat = ?, sfiyat = ?, kdv = ?, otv = ?, kdvdahil = ?, otvdahil = ?, tarih = ? , aciklama = ? "
                        "WHERE id = ?");
    sorgu.bindValue(0, _YeniStokKarti.getBarkod());
    sorgu.bindValue(1, _YeniStokKarti.getKod());
    sorgu.bindValue(2, _YeniStokKarti.getAd());
    sorgu.bindValue(3, _YeniStokKarti.getBirim());
    sorgu.bindValue(4, _YeniStokKarti.getMiktar());
    sorgu.bindValue(5, _YeniStokKarti.getGrup());
    sorgu.bindValue(6, _YeniStokKarti.getAFiyat());
    sorgu.bindValue(7, _YeniStokKarti.getSFiyat());
    sorgu.bindValue(8, _YeniStokKarti.getKdv());
    sorgu.bindValue(9, _YeniStokKarti.getOtv());
    sorgu.bindValue(10, _YeniStokKarti.getKdvdahil());
    sorgu.bindValue(11, _YeniStokKarti.getOtvdahil());
    sorgu.bindValue(12, _YeniStokKarti.getTarih());
    sorgu.bindValue(13, _YeniStokKarti.getAciklama() + " " + _Kullanici->getUserName());
    sorgu.bindValue(14, _EskiStokKartiID);
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
    stokKartlariModel->setQuery("SELECT id, barkod, kod, ad, birim, miktar, grup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv, otv, kdvdahil, otvdahil, tarih, aciklama FROM stokkartlari ORDER BY ad ASC", db);
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
    stokKartlariModel->setHeaderData(14, Qt::Horizontal, "Açıklama");
    return stokKartlariModel;
}

QSqlQueryModel *Veritabani::getStokKartlari(QString query)
{
    stokKartlariModel->setQuery(query, db);
    stokKartlariModel->setHeaderData(0, Qt::Horizontal, "Stok ID");
    stokKartlariModel->setHeaderData(1, Qt::Horizontal, "Barkod");
    stokKartlariModel->setHeaderData(2, Qt::Horizontal, "Ürün Adı");
    return stokKartlariModel;
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

void Veritabani::stokGrupEkle(QString _eklenecekGrupAdi)
{
    sorgu.prepare("INSERT INTO stokgruplari(id, grup) "
                    "VALUES (nextval('stokgruplari_sequence'), ?)");
    sorgu.bindValue(0, _eklenecekGrupAdi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qWarning() << "stok grup ekleme sorgu INSERT hatası: " << sorgu.lastError().text();
    }
}

void Veritabani::stokGrupSil(QString _silinecekGrupAdi)
{
    sorgu.prepare("DELETE FROM stokgruplari WHERE grup = ?");
    sorgu.bindValue(0, _silinecekGrupAdi);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qWarning() << "stok grup silme sorgu DELETE hatası: " << sorgu.lastError().text();
    }
}

void Veritabani::stokBirimEkle(QString _birim)
{
    sorgu.prepare("INSERT INTO stokbirimleri(id, birim) VALUES(nextval('stokbirimleri_sequence'), ?)");
    sorgu.bindValue(0, _birim);
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qFatal(sorgu.lastError().text().toStdString().c_str());
    }
}

void Veritabani::stokBirimSil(QString _birim)
{
    sorgu.prepare("DELETE FROM stokbirimleri WHERE birim = ?");
    sorgu.bindValue(0, _birim);
    sorgu.exec();
}

QStringList Veritabani::getVergiDaireleri()
{
    QStringList liste;
    sorgu.exec("SELECT ad FROM vergidaireleri");
    while (sorgu.next()) {
        liste.append(sorgu.value(0).toString());
    }
    return liste;
}

QSqlQueryModel *Veritabani::getStokHareketleri(QString _barkod)
{
    sorgu.prepare("SELECT * FROM stokhareketleri WHERE barkod = ?");
    sorgu.bindValue(0, _barkod);
    sorgu.exec();
    stokHareketleriModel->setQuery(sorgu);
    return stokHareketleriModel;
}

QSqlQueryModel *Veritabani::getStokHareketleri(QString _barkod, QDateTime _baslangicTarih, QDateTime _bitisTarih)
{
    sorgu.prepare("SELECT * FROM stokhareketleri WHERE barkod = ? AND tarih BETWEEN ? AND ? ORDER BY tarih DESC");
    sorgu.bindValue(0, _barkod);
    sorgu.bindValue(1, _baslangicTarih);
    sorgu.bindValue(2, _bitisTarih.addDays(1));
    sorgu.exec();
    stokHareketleriModel->setQuery(sorgu);
    return stokHareketleriModel;
}

void Veritabani::stokHareketiEkle(User _kullanici, QString _barkod, QString _islem, float _miktar)
{
    sorgu.prepare("INSERT INTO stokhareketleri(barkod, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                    "VALUES (?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, _barkod);
    sorgu.bindValue(1, _islem);
    sorgu.bindValue(2, _miktar);
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, _kullanici.getUserID());
    sorgu.bindValue(5, "STOK GİRİŞİ");
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << sorgu.lastError().text();
    }
}

QStringList Veritabani::getStokBirimleri()
{
    stokbirimleri.clear();
    sorgu.exec("SELECT * FROM stokbirimleri");
    while (sorgu.next()) {
        stokbirimleri.append(sorgu.value(1).toString());
    }
    return stokbirimleri;
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
    sorgu.bindValue(2, "ÇIKIŞ");
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << "kasadanParaCek kasa hareketleri hatası: " << sorgu.lastError().text();
    }
}

double Veritabani::getGunlukCiro()
{
    sorgu.prepare("SELECT SUM(miktar) FROM kasahareketleri WHERE tarih > ? AND islem IN(?,?)");
    sorgu.bindValue(0, QDateTime::currentDateTime().date());
    sorgu.bindValue(1, "GİRİŞ");
    sorgu.bindValue(2, "İADE");
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qWarning() << sorgu.lastError().text();
    }
    sorgu.next();
    return sorgu.value(0).toDouble();
}

void Veritabani::iadeAl(Sepet _iadeSepet, User _kullanici)
{
    // iade faturano alımı ve yeni oluşturma.
    sorgu.exec("SELECT last_value FROM faturalar_sequence");
    if(sorgu.lastError().isValid()){
        qFatal(qPrintable(sorgu.lastError().text()));
    }
    sorgu.next();
    QString iadeFaturaNo = QDate::currentDate().toString("ddMMyy") + QString::number(sorgu.value(0).toUInt() + 1);;
    // faturalar tablosuna iade fatura bilgisi girme başlangıcı
    sorgu.prepare("INSERT INTO faturalar (id, fatura_no, cari, tipi, tarih, kullanici, toplamtutar, odemetipi) "
                    "VALUES (nextval('faturalar_sequence'), ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, iadeFaturaNo);
    sorgu.bindValue(1, 1000);// DİREKT cari id
    sorgu.bindValue(2, 3);// 3 = iade
    sorgu.bindValue(3, QDateTime::currentDateTime());
    sorgu.bindValue(4, _kullanici.getUserID());
    sorgu.bindValue(5, _iadeSepet.sepetToplamTutari());
    sorgu.bindValue(6, 1);//1 = nakit ödeme tipi
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
    KasaHareketiEkle(_kullanici, "İADE", _iadeSepet.sepetToplamTutari(), ("İADE İŞLEMİ:" + iadeFaturaNo), QDateTime::currentDateTime(), -_iadeSepet.getSepettekiKazanc());
    //sepetteki iade ürünlerin stok hareketlerine girişi
    foreach (auto urun, _iadeSepet.urunler) {
        sorgu.prepare("INSERT INTO stokhareketleri(barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, urun.barkod);
        sorgu.bindValue(1, iadeFaturaNo);
        sorgu.bindValue(2, 3);// 3 = iade
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
    sorgu.prepare("INSERT INTO kasahareketleri(id, miktar, kullanici, islem, tarih) VALUES (nextval('kasahareketleri_sequence'), ?, ?, ?, ?)");
    sorgu.bindValue(0, _iadeSepet.sepetToplamTutari());
    sorgu.bindValue(1, _iadeCari.getId());
    sorgu.bindValue(2, "İADE");
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

int Veritabani::KasaHareketiEkle(User _user, QString _hareket, double _tutar, QString _aciklama, QDateTime _tarih, QString _evrakno, double _netKar)
{
    // KASAYA PARA GİRİŞ/ÇIKIŞ İŞLEMİ BAŞLANGIÇ
    sorgu.exec("select para FROM kasa");
    double mevcutPara = 0;
    double sonPara = 0;
    if(sorgu.next()){
        mevcutPara = sorgu.value(0).toDouble();
        if(_hareket == "GİRİŞ"){
            sonPara = mevcutPara + _tutar;
            sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
            sorgu.bindValue(0, sonPara);
            sorgu.exec();
            if(sorgu.lastError().isValid()){
                qDebug() << "kasahareketiekle() hata:\n" << sorgu.lastError().text();
            }
        }
        else if(_hareket == "ÇIKIŞ"){
            sonPara = mevcutPara - _tutar;
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
                    qDebug() << "kasahareketiekle() hata:\n" << sorgu.lastError().text();
                }
            }
        }
        //HAREKET EKLEME BAŞLANGICI
        sorgu.prepare("INSERT INTO kasahareketleri (id, miktar, kullanici, islem, tarih, kar, evrakno, aciklama) "
                        "VALUES (nextval('kasahareketleri_sequence'), ?, ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, _tutar);
        sorgu.bindValue(1, _user.getUserID().toInt());
        sorgu.bindValue(2, _hareket);
        sorgu.bindValue(3, _tarih);
        sorgu.bindValue(4, _netKar);
        sorgu.bindValue(5, _evrakno);
        sorgu.bindValue(6, _aciklama);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << sorgu.lastError().text().toStdString().c_str();
        }
        QMessageBox msg(0);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        if(_hareket == "GİRİŞ"){// hareket giriş ise
            msg.setText("Kasaya " + QString::number(_tutar, 'f', 2) + " TL giriş yapıldı.");
        }
        else if(_hareket == "ÇIKIŞ"){// hareket çıkış ise
            msg.setText("Kasadan " + QString::number(_tutar, 'f', 2) + " TL çıkış yapıldı.");
        }
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        return 1;
    }
}

//
int Veritabani::KasaHareketiEkle(User _user, QString _hareket, double _tutar, QString _aciklama, QDateTime _tarih, double _netKar)
{
    // KASAYA PARA GİRİŞ/ÇIKIŞ İŞLEMİ BAŞLANGIÇ
    sorgu.exec("select para FROM kasa");
    double mevcutPara = 0;
    double sonPara = 0;
    if(sorgu.next()){
        mevcutPara = sorgu.value(0).toDouble();
        if(_hareket == "GİRİŞ"){
            sonPara = mevcutPara + _tutar;
            sorgu.prepare("UPDATE kasa SET para = ? WHERE id = '1'");
            sorgu.bindValue(0, sonPara);
            sorgu.exec();
            if(sorgu.lastError().isValid()){
                qDebug() << "kasahareketiekle() hata:\n" << sorgu.lastError().text();
            }
        }
        //HAREKET EKLEME BAŞLANGICI
        sorgu.prepare("INSERT INTO kasahareketleri (id, miktar, kullanici, islem, tarih, kar, aciklama) "
                        "VALUES (nextval('kasahareketleri_sequence'), ?, ?, ?, ?, ?, ?)");
        sorgu.bindValue(0, _tutar);
        sorgu.bindValue(1, _user.getUserID().toInt());
        sorgu.bindValue(2, _hareket);
        sorgu.bindValue(3, _tarih);
        sorgu.bindValue(4, _netKar);
        sorgu.bindValue(5, _aciklama);
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << sorgu.lastError().text().toStdString().c_str();
        }
    }
    return 1;
}

