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

private:
    Ui::StokFrom *ui;

    User kullanici;
    Veritabani *vt = new Veritabani();
    QItemSelectionModel *selectionModel = new QItemSelectionModel();
    QSortFilterProxyModel *sortModel = new QSortFilterProxyModel();

    QShortcut *CTRL_F;
    QShortcut *key_F1;
    QShortcut *key_F2;
    QShortcut *key_F3;
    QShortcut *key_Down;
    QShortcut *key_DownArrow;
    QShortcut *key_Up;
    QShortcut *key_UpArrow;

    QList<int> gosterilenSatirlar;
    int sonSecilenGosterilenSatirIndexi = 0;
};

#endif // STOKFROM_H
