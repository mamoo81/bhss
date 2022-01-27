#include "sepet.h"
#include "urun.h"
//**************************
#include <QHash>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

Sepet::Sepet()
{

}

double Sepet::getOdenenTutar() const
{
    return odenenTutar;
}

void Sepet::setOdenenTutar(double newOdenenTutar)
{
    double kalan = sepetToplamTutari() - newOdenenTutar;
    if(kalan <= 0){
        setKalanTutar(0);
    }
    else{
        setKalanTutar(kalan);
    }
    odenenTutar = newOdenenTutar;
}

double Sepet::getKalanTutar() const
{
    return kalanTutar;
}

void Sepet::setKalanTutar(double newKalanTutar)
{
    kalanTutar = newKalanTutar;
}

double Sepet::sepetToplamTutari()
{
    double sepetToplam = 0;
    foreach (auto item, urunler) {
        sepetToplam += item.toplam;
    }
    return sepetToplam;
}

bool Sepet::sepetBosmu()
{
    if(urunler.isEmpty()){
        return true;
    }
    else{
        return false;
    }
}

/**
 * @brief Sepet::urunEkle
 * @param _StokKarti Sepete eklenecek stok kartı
 * @param _miktar Sepete eklenecek miktar -> stok kartı
 */
void Sepet::urunEkle(StokKarti _StokKarti, float _miktar)
{
    if(!urunler.contains(_StokKarti.getBarkod())){ //arrayda yok ise
        if(_StokKarti.getBirim() == "ADET"){
            Urun urun;
            urun.barkod = _StokKarti.getBarkod();
            urun.ad = _StokKarti.getAd();
            urun.alisFiyat = _StokKarti.getAFiyat();
            urun.birimFiyat = _StokKarti.getSFiyat();
            urun.birim = _StokKarti.getBirim();
            urun.miktar = urun.miktar + _miktar;
            urun.stokMiktari = _StokKarti.getMiktar();
            urun.toplam = urun.miktar * _StokKarti.getSFiyat();
            urunler.insert(_StokKarti.getBarkod(), urun);
        }
        else if(_StokKarti.getBirim() == "KİLOGRAM"){
            Urun urun;
            urun.barkod = _StokKarti.getBarkod();
            urun.ad = _StokKarti.getAd();
            urun.alisFiyat = _StokKarti.getAFiyat();
            urun.birimFiyat = _StokKarti.getSFiyat();
            urun.birim = _StokKarti.getBirim();
            urun.miktar += _miktar;
            urun.stokMiktari = _StokKarti.getMiktar();
            urun.toplam = urun.miktar * _StokKarti.getSFiyat();
            urunler.insert(_StokKarti.getBarkod(), urun);
        }
    }
    else{// arrayde var ise
        if(_StokKarti.getBirim() == "ADET"){
            ++urunler[_StokKarti.getBarkod()].miktar;
            urunler[_StokKarti.getBarkod()].toplam = urunler[_StokKarti.getBarkod()].miktar * _StokKarti.getSFiyat();
        }
        else if(_StokKarti.getBirim() == "KİLOGRAM"){
            urunler[_StokKarti.getBarkod()].miktar += _miktar;
            urunler[_StokKarti.getBarkod()].toplam = urunler[_StokKarti.getBarkod()].miktar * _StokKarti.getSFiyat();
        }
    }
}
/**
 * @brief Sepet::urunArtir Gönderilen stok kartini gönderilen miktar kadar arttırır.
 * @param _Barkod arttirilacak ürünün barkodu
 * @param _miktar arttirilacak miktar
 */
void Sepet::urunArtir(QString _Barkod, float _miktar)
{
    float yeniMiktar = urunler[_Barkod].miktar + _miktar;
    if(urunler[_Barkod].stokMiktari  >= yeniMiktar){
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

void Sepet::urunAdetCarp(QString _Barkod, float _miktar)
{
    urunler[_Barkod].miktar = _miktar;
    urunler[_Barkod].toplam = _miktar * urunler[_Barkod].birimFiyat;
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

double Sepet::getSepettekiKazanc()
{
    double toplam;
    foreach (auto urun, urunler) {
        toplam += urun.birimFiyat - urun.alisFiyat;
    }
    return toplam;
}

