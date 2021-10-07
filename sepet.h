#ifndef SEPET_H
#define SEPET_H

#include <stokkarti.h>
#include <user.h>

#include <QString>
#include <QDateTime>
#include <QVector>

class Sepet
{
public:
    Sepet();

    void sat(User satisYapan, QString satisKanali);
    void ekle(StokKarti p_StokKarti);
    double toplamTutar();
    void adetArtir(QString artirilacakBarkod);
    void adetAzalt(QString azaltilacakBarkod);
    void satirSil(QString silinecekBarkod);

private:
    QVector<StokKarti> sepetUrunler;
};

#endif // SEPET_H
