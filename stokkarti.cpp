/*####################################
MIT LICENCE                          #
######################################
Copyright 2021 Mehmet AKDEMİR        #
bilgi@basat.dev                      #
######################################
Permission is hereby granted, free of charge,
to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stokkarti.h"
//****************************
#include <QProcess>
#include <QDebug>
StokKarti::StokKarti()
{

}

const QString &StokKarti::getId() const
{
    return id;
}

void StokKarti::setId(const QString &newId)
{
    id = newId;
}

const QString &StokKarti::getBarkod() const
{
    return barkod;
}

void StokKarti::setBarkod(const QString &newBarkod)
{
    barkod = newBarkod;
    QFile resimDosya(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/urunler-image/" + barkod + ".png");
    if(resimDosya.exists()){
        QImage img(QFileInfo(resimDosya).absoluteFilePath());
        setResim(QPixmap::fromImage(img));
    }
    else{
        QImage img(":/images/ui/box.png");
        setResim(QPixmap::fromImage(img));
    }
}

const QString &StokKarti::getAd() const
{
    return ad;
}

void StokKarti::setAd(const QString &newAd)
{
    ad = newAd;
}

const int &StokKarti::getBirim() const
{
    return birim;
}

void StokKarti::setBirim(const int &newBirim)
{
    birim = newBirim;
}

float StokKarti::getMiktar() const
{
    return miktar;
}

void StokKarti::setMiktar(float newMiktar)
{
    miktar = newMiktar;
}

const int &StokKarti::getGrup() const
{
    return grup;
}

void StokKarti::setGrup(const int &newGrup)
{
    grup = newGrup;
}

double StokKarti::getAFiyat() const
{
    return aFiyat;
}

void StokKarti::setAFiyat(double newAFiyat)
{
    aFiyat = newAFiyat;
}

double StokKarti::getSFiyat() const
{
    return sFiyat;
}

void StokKarti::setSFiyat(double newSFiyat)
{
    sFiyat = newSFiyat;
}

int StokKarti::getKdv() const
{
    return kdv;
}

void StokKarti::setKdv(int newKdv)
{
    kdv = newKdv;
}

const QDateTime &StokKarti::getTarih() const
{
    return tarih;
}

void StokKarti::setTarih(const QDateTime &newTarih)
{
    tarih = newTarih;
}

const QString &StokKarti::getAciklama() const
{
    return aciklama;
}

void StokKarti::setAciklama(const QString &newAciklama)
{
    aciklama = newAciklama;
}

const QString &StokKarti::getKod() const
{
    return kod;
}

void StokKarti::setKod(const QString &newKod)
{
    kod = newKod;
}

int StokKarti::getOtv() const
{
    return otv;
}

void StokKarti::setOtv(int newOtv)
{
    otv = newOtv;
}

bool StokKarti::getKdvdahil() const
{
    return kdvdahil;
}

void StokKarti::setKdvdahil(bool newKdvdahil)
{
    kdvdahil = newKdvdahil;
}

bool StokKarti::getOtvdahil() const
{
    return otvdahil;
}

void StokKarti::setOtvdahil(bool newOtvdahil)
{
    otvdahil = newOtvdahil;
}

QString StokKarti::getUretici() const
{
    return uretici;
}

void StokKarti::setUretici(QString newUretici)
{
    uretici = newUretici;
}

QString StokKarti::getTedarikci() const
{
    return tedarikci;
}

void StokKarti::setTedarikci(QString newTedarikci)
{
    tedarikci = newTedarikci;
}

const QPixmap &StokKarti::getResim() const
{
    return resim;
}

void StokKarti::setResim(const QPixmap &newResim)
{
    resim = newResim;
}

QImage StokKarti::getBarkodImg() const
{
    QFile barkodImgDosya(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/barkodlar/" + getBarkod() + ".svg");
    if(!QFileInfo(barkodImgDosya).exists()){
        //barkod image oluşturma
        QString kirpilmisBarkod = QString(getBarkod().left(getBarkod().count() - 1));// barkodun son hanesi doğrulama hanesi olduğu için zint kendi veriyor. kırpıyorum.
        // zint ile EAN-8 veya EAN-13(TR DE KULLANILAN BARKOD TİPİ) barkod resmini oluşturma
        QString zintKomut("zint -b 13 -o " + QFileInfo(barkodImgDosya).absoluteFilePath() + " -d \"" + kirpilmisBarkod + "\"");
        int exitCode = system(qPrintable(zintKomut));
        if(!exitCode == QProcess::NormalExit){
            qDebug() << "barkodIMG oluşturulamadı";
        }
    }
    return QImage(QFileInfo(barkodImgDosya).absoluteFilePath());
}
