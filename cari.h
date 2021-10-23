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
    const QString &getAdresNo() const;
    void setAdresNo(const QString &newAdresNo);
    const QString &getAdres() const;
    void setAdres(const QString &newAdres);
    const QString &getIl() const;
    void setIl(const QString &newIl);
    const QString &getIlce() const;
    void setIlce(const QString &newIlce);
    const QString &getMail() const;
    void setMail(const QString &newMail);
    const QString &getCep() const;
    void setCep(const QString &newCep);
    const QDateTime &getTarih() const;
    void setTarih(const QDateTime &newTarih);

private:
    int id;
    QString ad;
    QString verigino;
    QString vergiDaire;
    QString tcNo;
    QString adresNo;
    QString adres;
    QString il;
    QString ilce;
    QString mail;
    QString cep;
    QDateTime tarih;

};

#endif // CARI_H
