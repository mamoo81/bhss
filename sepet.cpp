#include "sepet.h"
#include "urun.h"

#include <QHash>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>


Sepet::Sepet()
{
    db_sepet = QSqlDatabase::addDatabase("QPSQL","db_sepet");
    db_sepet.setDatabaseName("mhss_data");
    db_sepet.setHostName("localhost");
    db_sepet.setUserName("admin");
    db_sepet.setPassword("admin");
    db_sepet.open();
}


double Sepet::sepetToplamTutari()
{
    double sepetToplam = 0;
    foreach (auto item, urunler) {
        sepetToplam += item.toplam;
    }
    return sepetToplam;
}

void Sepet::sepetiSat(QString _Kullanici)
{
    // satış kayıtları ve veritabani işlemleri...
}
/**
 * @brief Sepet::urunEkle
 * @param _StokKarti
 * @param _miktar
 */
void Sepet::urunEkle(StokKarti _StokKarti, float _miktar)
{
    if(!urunler.contains(_StokKarti.getBarkod())){ //arrayda yok ise
        if(_StokKarti.getBirim() == "ADET"){
            Urun urun;
            urun.barkod = _StokKarti.getBarkod();
            urun.ad = _StokKarti.getAd();
            urun.birimFiyat = _StokKarti.getSFiyat().toDouble();
            urun.birim = _StokKarti.getBirim();
            urun.miktar = urun.miktar + _miktar;
            urun.toplam = urun.miktar * _StokKarti.getSFiyat().toDouble();
            urunler.insert(_StokKarti.getBarkod(), urun);
        }
        else if(_StokKarti.getBirim() == "KİLOGRAM"){
            Urun urun;
            urun.barkod = _StokKarti.getBarkod();
            urun.ad = _StokKarti.getAd();
            urun.birimFiyat = _StokKarti.getSFiyat().toDouble();
            urun.birim = _StokKarti.getBirim();
            urun.miktar += _miktar;
            urun.toplam = urun.miktar * _StokKarti.getSFiyat().toDouble();
            urunler.insert(_StokKarti.getBarkod(), urun);
        }
    }
    else{// arrayde var ise
        if(_StokKarti.getBirim() == "ADET"){
            ++urunler[_StokKarti.getBarkod()].miktar;
            urunler[_StokKarti.getBarkod()].toplam = urunler[_StokKarti.getBarkod()].miktar * _StokKarti.getSFiyat().toDouble();
        }
        else if(_StokKarti.getBirim() == "KİLOGRAM"){
            urunler[_StokKarti.getBarkod()].miktar += _miktar;
            urunler[_StokKarti.getBarkod()].toplam = urunler[_StokKarti.getBarkod()].miktar * _StokKarti.getSFiyat().toDouble();
        }
    }
}

void Sepet::urunArtir(QString _Barkod, float _miktar)
{
    float yeniMiktar = urunler[_Barkod].miktar + _miktar;
    QSqlQuery sorgu(db_sepet);
    sorgu.prepare("SELECT miktar FROM stokkartlari WHERE barkod = ?");
    sorgu.bindValue(0, _Barkod);
    sorgu.exec();
    if(sorgu.next()){
        if(sorgu.value(0).toFloat() >= yeniMiktar){
            urunler[_Barkod].miktar += _miktar;
            urunler[_Barkod].toplam = urunler[_Barkod].miktar * urunler[_Barkod].birimFiyat;
        }
        else{
            QMessageBox msg(0);
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle("UYARI");
            msg.setText("STOK MİKTARINDAN FAZLA OLAMAZ!");
            msg.setModal(true);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
}

void Sepet::urunAzalt(QString _Barkod, float _miktar)
{
    float yeniMiktar = urunler[_Barkod].miktar - _miktar;
    if(yeniMiktar > 0){
        urunler[_Barkod].miktar = yeniMiktar;
        urunler[_Barkod].toplam = yeniMiktar * urunler[_Barkod].birimFiyat;
    }
}

void Sepet::urunSil(QString _Barkod)
{
    urunler.remove(_Barkod);
}

void Sepet::sepetiSil()
{
    urunler.clear();
}

bool Sepet::urunSepetteVarmi(QString _Barkod)
{
    if(urunler.contains(_Barkod)){
        return true;
    }
    else{
        return false;
    }
}

QHash<QString, Urun> Sepet::sepetiGetir()
{
    return urunler;
}

Urun Sepet::urunBilgileriniGetir(QString _Barkod)
{
    return urunler.value(_Barkod);
}



