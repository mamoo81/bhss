/*######################################
*MIT LICENCE                          #
*######################################
*Copyright 2021 Mehmet AKDEMİR        #
*bilgi@basat.dev                      #
*######################################
*Permission is hereby granted, free of charge,
*to any person obtaining a copy of this software and associated documentation files (the "Software"),
*to deal in the Software without restriction, including without limitation the rights to use, copy,
*modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
*and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
*INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
*DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef KASAHAREKETEKLEDIALOG_H
#define KASAHAREKETEKLEDIALOG_H
#include "user.h"
#include "veritabani.h"
//*************************
#include <QDialog>

namespace Ui {
class KasaHareketEkleDialog;
}

class KasaHareketEkleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KasaHareketEkleDialog(QWidget *parent = nullptr);
    ~KasaHareketEkleDialog();

    Veritabani *vt = new Veritabani();

    void setKullanici(User newKullanici);

    void setHareket(int newHareket);

    void sethareketDuzenle(bool newhareketDuzenle);

    void setTarih(const QDateTime newTarih);

    void setAciklama(const QString &newAciklama);

    void setTutar(double newTutar);

    void setEvrakNo(const QString &newEvrakNo);

    void setHareketID(const QString &newHareketID);

private slots:
    void on_KaydetpushButton_clicked();

    void on_iptalpushButton_clicked();

    void on_tarihdateEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_KullaniciCikisipushButton_clicked();

    void on_HareketcomboBox_currentIndexChanged(int index);

private:
    Ui::KasaHareketEkleDialog *ui;

    int hareket;
    User kullanici;
    bool hareketDuzenle;
    QDateTime tarih;
    QString aciklama;
    QString hareketID;
    QString oncekiHareket;
    double tutar;
    QString evrakNo;
};

#endif // KASAHAREKETEKLEDIALOG_H
