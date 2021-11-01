#ifndef STOKKARTI_H
#define STOKKARTI_H

#include <QString>
#include <QDateTime>

class StokKarti
{
public:
    StokKarti();

    const QString &getId() const;
    void setId(const QString &newId);

    const QString &getBarkod() const;
    void setBarkod(const QString &newBarkod);

    const QString &getAd() const;
    void setAd(const QString &newAd);

    const QString &getBirim() const;
    void setBirim(const QString &newBirim);

    float getMiktar() const;
    void setMiktar(float newMiktar);

    const QString &getGrup() const;
    void setGrup(const QString &newGrup);

    double getAFiyat() const;
    void setAFiyat(double newAFiyat);

    double getSFiyat() const;
    void setSFiyat(double newSFiyat);

    int getKdv() const;
    void setKdv(int newKdv);

    const QDateTime &getTarih() const;
    void setTarih(const QDateTime &newTarih);

    const QString &getAciklama() const;
    void setAciklama(const QString &newAciklama);

private:
    QString id;
    QString barkod;
    QString ad;
    QString birim;
    float miktar;
    QString grup;
    double aFiyat;
    double sFiyat;
    int kdv;
    QDateTime tarih;
    QString aciklama;
};

#endif // STOKKARTI_H
