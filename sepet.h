#ifndef SEPET_H
#define SEPET_H

#include <stokkarti.h>
#include <user.h>

#include <QString>
#include <QDateTime>
#include <QHash>

class Sepet
{
public:
    Sepet();

    void sat(User satisYapan, QString satisKanali);
    void ekle(StokKarti p_StokKarti);
    QString satirToplami();
    QString genelToplam();
    void adetArtir(QString artirilacakBarkod);
    void adetAzalt(QString azaltilacakBarkod);
    void satirSil(QString silinecekBarkod);

private:
    QHash<QString, StokKarti> sepetUrunler;
};


#endif // SEPET_H
