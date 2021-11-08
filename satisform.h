#ifndef SATISFORM_H
#define SATISFORM_H

#include "user.h"
#include "stokkarti.h"
#include "veritabani.h"

#include <QWidget>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QList>
#include <QToolButton>

namespace Ui {
class SatisForm;
}

class SatisForm : public QWidget
{
    Q_OBJECT

public:
    explicit SatisForm(QWidget *parent = nullptr);
    ~SatisForm();

    QToolButton *hizliEklenecekButon;

    void setUser(User user);
    void formLoad();
    bool eventFilter(QObject *watched, QEvent *event);
    void getSonSatislar();

public slots:
    void hizliButonConnects();
    void ekleAction();
    void silAction();
public Q_SLOTS:
    void slotCustomContextMenuRequested(QPoint position);

    void hizliUrunButonlariAyarla();
private slots:
    void on_StokKartlariBtn_clicked();
    void initTableWidgets();
    void sepeteEkle();
    void on_SepetlertabWidget_currentChanged(int index);
    void sepetToplaminiYaz();
    void tableWidgetEkle(StokKarti p_StokKarti, float _miktar);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool urunSepetteVarmi(QString _Barkod);
    int getSeciliSatirIndexi();
    void on_artirBtn_clicked();

    void on_azaltBtn_clicked();

    void on_satirSilBtn_clicked();

    void on_sepetSilBtn_clicked();
    void butonDurumlariniAyarla();

    void on_satisYapBtn_clicked();

    double paraYuvarla(double _toplam);
    void on_SatisiGosterpushButton_clicked();

    void on_hizlitoolButton1_clicked();

    void on_hizlitoolButton2_clicked();

    void on_hizlitoolButton3_clicked();

    void on_hizlitoolButton4_clicked();

    void on_hizlitoolButton5_clicked();

    void on_hizlitoolButton6_clicked();

    void on_hizlitoolButton7_clicked();

    void on_hizlitoolButton8_clicked();

    void on_hizlitoolButton9_clicked();

    void on_hizlitoolButton10_clicked();

    void on_hizlitoolButton11_clicked();

    void on_hizlitoolButton12_clicked();

    void on_hizlitoolButton13_clicked();

    void on_hizlitoolButton14_clicked();

    void on_hizlitoolButton15_clicked();

    void on_hizlitoolButton16_clicked();

    void on_hizlitoolButton17_clicked();

    void on_hizlitoolButton18_clicked();

    void on_hizlitoolButton19_clicked();

    void on_hizlitoolButton20_clicked();

    void on_hizlitoolButton21_clicked();

    void on_hizlitoolButton22_clicked();

    void on_hizlitoolButton23_clicked();

    void on_hizlitoolButton24_clicked();

    void on_hizlitoolButton25_clicked();

    void on_hizlitoolButton26_clicked();

    void on_hizlitoolButton27_clicked();

    void on_hizlitoolButton28_clicked();

    void on_hizlitoolButton29_clicked();

    void on_hizlitoolButton30_clicked();

    void on_hizlitoolButton31_clicked();

    void on_hizlitoolButton32_clicked();

    void on_hizlitoolButton33_clicked();

    void on_hizlitoolButton34_clicked();

    void on_hizlitoolButton35_clicked();

    void on_hizlitoolButton36_clicked();

    void on_hizlitoolButton37_clicked();

    void on_hizlitoolButton38_clicked();

    void on_hizlitoolButton39_clicked();

    void on_hizlitoolButton40_clicked();

    void on_hizlitoolButton41_clicked();

    void on_hizlitoolButton42_clicked();

    void on_hizlitoolButton43_clicked();

    void on_hizlitoolButton44_clicked();

    void on_hizlitoolButton45_clicked();

    void on_hizlitoolButton46_clicked();

    void on_hizlitoolButton47_clicked();

    void on_hizlitoolButton48_clicked();

    void on_hizlitoolButton49_clicked();

    void on_hizlitoolButton50_clicked();

    void on_hizlitoolButton51_clicked();

    void on_hizlitoolButton52_clicked();

    void on_hizlitoolButton53_clicked();

    void on_hizlitoolButton54_clicked();

    void on_hizlitoolButton55_clicked();

    void on_hizlitoolButton56_clicked();

    void on_hizlitoolButton57_clicked();

    void on_hizlitoolButton58_clicked();

    void on_hizlitoolButton59_clicked();

    void on_hizlitoolButton60_clicked();

private:
    Ui::SatisForm *ui;
};

#endif // SATISFORM_H
