#include "sepet.h"
#include "stokkarti.h"

#include <QDebug>

Sepet::Sepet()
{

}

void Sepet::ekle(StokKarti p_StokKarti)
{
    if(!sepetUrunler.contains(p_StokKarti.getBarkod())){
        sepetUrunler.insert(p_StokKarti.getBarkod(), p_StokKarti);
        StokKarti gecicistokkarti = sepetUrunler.value(p_StokKarti.getBarkod());
        qDebug() << gecicistokkarti.getAd();
    }

}

QString Sepet::satirToplami()
{
    QString genelToplam = 0;
    return genelToplam;
}

