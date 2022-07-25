#ifndef STOKKARTIFORM_H
#define STOKKARTIFORM_H
#include "stokkarti.h"
#include "veritabani.h"
#include "user.h"
//*************************
#include <QDialog>
#include <QPalette>
#include <QRegExp>
#include <QDoubleValidator>
#include <QSound>

namespace Ui {
class StokKartiForm;
}

class StokKartiForm : public QDialog
{
    Q_OBJECT

public:
    explicit StokKartiForm(QWidget *parent = nullptr);
    ~StokKartiForm();

    QSound *uyariSes = new QSound(":/sounds/sounds/warning-sound.wav", this);
    bool yeniKart = true;
    bool toplustokkarti = false;
    bool toplustokkartiIptal = false;
    bool kayitBasarilimi = false;
    bool fiyatGuncelle = false;
    bool fiyatGuncellendi = false;

    void setKart(StokKarti newYeniKart);

    void FormLoad();

    void setKullanici(const User &newKullanici);

private slots:
    void on_KaydetpushButton_clicked();

    void on_iptalpushButton_clicked();

    void on_temizlepushButton_clicked();

    void AlanlariTemizle();
    void on_BarkodlineEdit_textChanged(const QString &arg1);

    void on_StokKodlineEdit_textChanged(const QString &arg1);

    void on_AdlineEdit_textChanged(const QString &arg1);

    void on_AFiyatdoubleSpinBox_valueChanged(double arg1);

    void on_SFiyatdoubleSpinBox_valueChanged(double arg1);

    void on_MiktarlineEdit_textChanged(const QString &arg1);

    void on_BarkodOlusturpushButton_clicked();

    void on_stokHareketleripushButton_clicked();

    void on_cikispushButton_clicked();

private:
    Ui::StokKartiForm *ui;

    Veritabani vt = Veritabani();
    User kullanici;

    StokKarti kart;

    QPalette TextColorPaletteRed;
    QPalette TextColorPaletteDefault;

    QRegExp regEXPbarkod;
    QRegExp regEXPstokKod;
    QRegExp regEXPstokAd;
    QDoubleValidator *regEXPstokMiktar;
};

#endif // STOKKARTIFORM_H
