#ifndef CARI_H
#define CARI_H

#include <QString>
#include <QDateTime>

class Cari
{
public:
    Cari();


    const QString &getAd() const;
    void setAd(const QString &newAd);

    int getId() const;
    void setId(int newId);

    const QString &getVerigino() const;
    void setVerigino(const QString &newVerigino);
    const QString &getVergiDaire() const;
    void setVergiDaire(const QString &newVergiDaire);
    const QString &getTcNo() const;
    void setTcNo(const QString &newTcNo);
    const QString &getAdres() const;
    void setAdres(const QString &newAdres);
    const QString &getIl() const;
    void setIl(const QString &newIl);
    const QString &getIlce() const;
    void setIlce(const QString &newIlce);
    const QString &getMail() const;
    void setMail(const QString &newMail);
    const QDateTime &getTarih() const;
    void setTarih(const QDateTime &newTarih);

    const int &getTip() const;
    void setTip(const int &newTip);

    const QString &getTelefon() const;
    void setTelefon(const QString &newTelefon);

    const QString &getAciklama() const;
    void setAciklama(const QString &newAciklama);

    const QString &getYetkili() const;
    void setYetkili(const QString &newYetkili);

private:
    int id;
    QString ad;
    QString yetkili;
    int tip;
    QString verigino;
    QString vergiDaire;
    QString tcNo;
    QString adres;
    QString il;
    QString ilce;
    QString mail;
    QString telefon;
    QDateTime tarih;
    QString aciklama;

};

#endif // CARI_H
