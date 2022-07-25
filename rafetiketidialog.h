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
#ifndef RAFETIKETIDIALOG_H
#define RAFETIKETIDIALOG_H
#include "veritabani.h"
#include "yazici.h"
#include "etiketthread.h"

#include <QWidget>
#include <QSound>
#include <QKeyEvent>
#include <QShortcut>

namespace Ui {
class RafEtiketiDialog;
}

class RafEtiketiDialog : public QWidget
{
    Q_OBJECT

public:
    explicit RafEtiketiDialog(QWidget *parent = nullptr);
    ~RafEtiketiDialog();

    QSound *uyariSes = new QSound(":/sounds/sounds/warning-sound.wav", this);

public slots:

    void stokKartlaricustomContextMenuRequested(QPoint pos);

    void yazdirilacaklarcustomContextMenuRequested(QPoint pos);

    void stokKartlariniListele();

    void ListeyeEkle();

    void ListedenCikar();

    void ButonDurumlariniAyarla();

    void keyPressEvent(QKeyEvent *event);

    void stokKartiAra(QString aranacakMetin);
private slots:
    void on_eklepushButton_clicked();

    void on_cikarpushButton_clicked();

    void on_yazdirtoolButton_clicked();

    void on_hepsiStokkartlaricheckBox_clicked();

    void on_HepsiYazdirilacakcheckBox_clicked();

    void on_DurdurtoolButton_clicked();

    void on_arapushButton_clicked();

    void on_aralineEdit_textChanged(const QString &arg1);

    void on_adindaradioButton_clicked();

    void on_barkodradioButton_clicked();

    void on_TemizletoolButton_clicked();

    void yazdirmaSinyaliAlininca(int value, QString barkod);
    void yazdirmaBittiSinyaliAlininca();

private:
    Ui::RafEtiketiDialog *ui;

    Veritabani *vtEtiket = new Veritabani();
    Yazici *yaziciEtiket = new Yazici();
    EtiketThread *etiketTHRD;

    QList<StokKarti> kartlar;

    QShortcut *key_left;
    QShortcut *key_right;

};

#endif // RAFETIKETIDIALOG_H
