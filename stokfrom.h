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
#ifndef STOKFROM_H
#define STOKFROM_H
#include "user.h"
#include "veritabani.h"
#include "QSortFilterProxyModel"
#include "stokkartlarimodel.h"
//************************
#include <QDialog>
#include <QPixmap>
#include <QShortcut>
#include <QSound>
#include <QItemSelectionModel>

namespace Ui {
class StokFrom;
}

class StokFrom : public QDialog
{
    Q_OBJECT

public:
    explicit StokFrom(QWidget *parent = nullptr);
    ~StokFrom();

    QSound *uyariSes = new QSound(":/sounds/sounds/warning-sound.wav", this);
    int seciliIndex;
    bool yeniKayit = false;

    StokKartlariModel *_stokKartlariTableModel;

public slots:
    void setUser(User user);

    void customMenuRequested(QPoint pos);

    void hizliRafEtiketiYazdir();

private slots:
    void on_StokGrupBtn_clicked();

    void formLoad();

    void grupComboboxDoldur();

    void stokKartlariniListele();

    void on_YeniBtn_clicked();

    void on_DuzenleBtn_clicked();

    void on_IptalBtn_clicked();

    void alanlariTemizle();

    void alanlariDoldur();

    void closeEvent(QCloseEvent*);
    void keyPressEvent(QKeyEvent *event);

    void on_KaydetBtn_clicked();

    void on_SilBtn_clicked();

    void stokKartiAra(QString aranacakMetin);

    void on_araBtn_clicked();
    void on_dosyadanToolButton_clicked();

    void on_StokBirimBtn_clicked();

    void on_StokGirBtn_clicked();

    void on_StokCikBtn_clicked();

    void on_StokKartlaritableView_doubleClicked(const QModelIndex &index);

    void on_BarkodOlusturBtn_clicked();

    void on_AraLineEdit_textChanged(const QString &arg1);

    void on_ResimEkleBtn_clicked();

    void on_ResimSilBtn_clicked();

    void urunResmiKaydet(QPixmap urunResmi, QString urunBarkod);

    void CTRL_F_Slot();

    void key_Down_Slot();

    void key_UP_Slot();

    void key_F1_Slot();

    void key_F2_Slot();

    void key_F3_Slot();

    void key_F4_Slot();

    void key_F5_Slot();

    void key_F6_Slot();

    void on_barkodRadioButton_clicked();

private:
    Ui::StokFrom *ui;

    User kullanici;
    Veritabani *vt = new Veritabani();
    QItemSelectionModel *selectionModel = new QItemSelectionModel();
    QSortFilterProxyModel *sortModel = new QSortFilterProxyModel();

    int seciliSatirIndex;
    QItemSelectionModel *seciliSatirModel;

    QShortcut *CTRL_F;
    QShortcut *key_F1;
    QShortcut *key_F2;
    QShortcut *key_F3;
    QShortcut *key_F4;
    QShortcut *key_F5;
    QShortcut *key_F6;
    QShortcut *key_Down;
    QShortcut *key_DownArrow;
    QShortcut *key_Up;
    QShortcut *key_UpArrow;

    QList<int> gosterilenSatirlar;
    int sonSecilenGosterilenSatirIndexi = 0;
};

#endif // STOKFROM_H
