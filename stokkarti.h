#ifndef STOKKARTI_H
#define STOKKARTI_H

#include <QString>
#include <QDateTime>

class StokKarti
{
public:
    StokKarti();

    void setID(QString p_ID);
    QString getID();
    void setBarkod(QString p_Barkod);
    QString getBarkod();
    void setAd(QString p_Ad);
    QString getAd();
    void setBirim(QString p_Birim);
    QString getBirim();
    void setMiktar(QString p_Miktar);
    QString getMiktar();
    void setGrup(QString p_Grup);
    QString getGrup();
    void setAFiyat(double p_AFiyat);
    double getAfiyat() const;
    void setSFiyat(double p_SFiyat);
    double getSFiyat() const;
    void setKDV1(int p_KDV1);
    int getKDV1();
    void setKDV2(int p_KDV2);
    int getKDV2();
    void setKDV3(int p_KDV3);
    int getKDV3();
    void setSonGunTarih(QDateTime p_SonGunTarih);
    QDateTime getSonGunTarih();
    void setAciklama(QString p_Aciklama);
    QString getAciklama();


    QString id;
    QString barkod;
    QString ad;
    QString birim;
    QString miktar;
    QString grup;
    double aFiyat;
    double sFiyat;
    int kdv1, kdv2, kdv3;
    QDateTime sonGunTarih;
    QString aciklama;
private:

};

#endif // STOKKARTI_H
