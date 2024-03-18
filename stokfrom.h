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
#include "stokyonetimi.h"
#include "stokkartiform.h"
//************************
#include <QDialog>
#include <QPixmap>
#include <QShortcut>
#include <QSoundEffect>
#include <QItemSelectionModel>
#include <QRegExp>
#include <QDoubleValidator>

namespace Ui {
class StokFrom;
}

class StokFrom : public QDialog
{
    Q_OBJECT

public:
    explicit StokFrom(QWidget *parent = nullptr);
    ~StokFrom();

    QSoundEffect uyariSesi;
    int seciliIndex;
    bool yeniKayit = false;

    StokKartlariModel *_stokKartlariTableModel;

public slots:
    void setUser(User user);

    void customMenuRequested(QPoint pos);

    void hizliRafEtiketiYazdir();

    void stokGirSlot();

    void stokCikSlot();

private slots:

    void formLoad();

    void filtreGrupComboboxDoldur();

    void stokKartlariniListele();

    void on_YeniBtn_clicked();

    void on_DuzenleBtn_clicked();

    void closeEvent(QCloseEvent*);

    void keyPressEvent(QKeyEvent *event);

    void on_SilBtn_clicked();

    void stokKartiAra(QString aranacakMetin);

    void on_araBtn_clicked();

    void on_dosyadanToolButton_clicked();

    void on_StokKartlaritableView_doubleClicked(const QModelIndex &index);

    void on_AraLineEdit_textChanged(const QString &arg1);

    void CTRL_F_Slot();

    void key_Down_Slot();

    void key_UP_Slot();

    void key_F1_Slot();

    void key_F2_Slot();

    void key_F3_Slot();

    void key_F4_Slot();

    void key_F5_Slot();

    void key_F6_Slot();

    void key_F7_Slot();

    void fiyatGuncelle_Slot();

    void on_barkodRadioButton_clicked();

    void on_StokKartlaritableView_clicked(const QModelIndex &index);

    void on_adRadioButton_clicked();

    void on_kodRadioButton_clicked();

    void on_TopluEtikettoolButton_clicked();

    void on_grupFiltrecomboBox_currentIndexChanged(int index);

private:
    Ui::StokFrom *ui;

    User kullanici;
    Veritabani *vt = new Veritabani();
    StokYonetimi stokYonetimi = StokYonetimi();
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
    QShortcut *key_F7;
    QShortcut *key_Down;
    QShortcut *key_DownArrow;
    QShortcut *key_Up;
    QShortcut *key_UpArrow;

    QList<int> gosterilenSatirlar;
    int sonSecilenGosterilenSatirIndexi = 0;

    QPalette TextColorPaletteRed;
    QPalette TextColorPaletteDefault;

    QRegExp regEXPbarkod;
    QRegExp regEXPstokKod;
    QRegExp regEXPstokAd;
    QDoubleValidator *regEXPstokMiktar;
};

#endif // STOKFROM_H
