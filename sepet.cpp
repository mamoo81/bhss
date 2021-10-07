#include "sepet.h"
#include "stokkarti.h"



Sepet::Sepet()
{

}

void Sepet::ekle(StokKarti p_StokKarti)
{
    sepetUrunler.append(p_StokKarti);
}

double Sepet::toplamTutar()
{
    double genelToplam = 0;
    for(int i = 0; i < sepetUrunler.count(); i++){
        genelToplam = genelToplam + sepetUrunler.at(i).getSFiyat();
    }
    return genelToplam;
}
