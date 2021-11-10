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
    void getKasaPara();
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

    void on_hizlitoolButton61_clicked();

    void on_hizlitoolButton62_clicked();

    void on_hizlitoolButton63_clicked();

    void on_hizlitoolButton64_clicked();

    void on_hizlitoolButton65_clicked();

    void on_hizlitoolButton66_clicked();

    void on_hizlitoolButton67_clicked();

    void on_hizlitoolButton68_clicked();

    void on_hizlitoolButton69_clicked();

    void on_hizlitoolButton70_clicked();

    void on_hizlitoolButton71_clicked();

    void on_hizlitoolButton72_clicked();

    void on_hizlitoolButton73_clicked();

    void on_hizlitoolButton74_clicked();

    void on_hizlitoolButton75_clicked();

    void on_hizlitoolButton76_clicked();

    void on_hizlitoolButton77_clicked();

    void on_hizlitoolButton78_clicked();

    void on_hizlitoolButton79_clicked();

    void on_hizlitoolButton80_clicked();

    void on_hizlitoolButton81_clicked();

    void on_hizlitoolButton82_clicked();

    void on_hizlitoolButton83_clicked();

    void on_hizlitoolButton84_clicked();

    void on_hizlitoolButton85_clicked();

    void on_hizlitoolButton86_clicked();

    void on_hizlitoolButton87_clicked();

    void on_hizlitoolButton88_clicked();

    void on_hizlitoolButton89_clicked();

    void on_hizlitoolButton90_clicked();

    void on_hizlitoolButton91_clicked();

    void on_hizlitoolButton92_clicked();

    void on_hizlitoolButton93_clicked();

    void on_hizlitoolButton94_clicked();

    void on_hizlitoolButton95_clicked();

    void on_hizlitoolButton96_clicked();

    void on_hizlitoolButton97_clicked();

    void on_hizlitoolButton98_clicked();

    void on_hizlitoolButton99_clicked();

    void on_hizlitoolButton100_clicked();

    void on_hizlitoolButton101_clicked();

    void on_hizlitoolButton102_clicked();

    void on_hizlitoolButton103_clicked();

    void on_hizlitoolButton104_clicked();

    void on_hizlitoolButton105_clicked();

    void on_hizlitoolButton106_clicked();

    void on_hizlitoolButton107_clicked();

    void on_hizlitoolButton108_clicked();

    void on_hizlitoolButton109_clicked();

    void on_hizlitoolButton110_clicked();

    void on_hizlitoolButton111_clicked();

    void on_hizlitoolButton112_clicked();

    void on_hizlitoolButton113_clicked();

    void on_hizlitoolButton114_clicked();

    void on_hizlitoolButton115_clicked();

    void on_hizlitoolButton116_clicked();

    void on_hizlitoolButton117_clicked();

    void on_hizlitoolButton118_clicked();

    void on_hizlitoolButton119_clicked();

    void on_hizlitoolButton120_clicked();

    void on_hizlitoolButton121_clicked();

    void on_hizlitoolButton122_clicked();

    void on_hizlitoolButton123_clicked();

    void on_hizlitoolButton124_clicked();

    void on_hizlitoolButton125_clicked();

    void on_hizlitoolButton126_clicked();

    void on_hizlitoolButton127_clicked();

    void on_hizlitoolButton128_clicked();

    void on_hizlitoolButton129_clicked();

    void on_hizlitoolButton130_clicked();

    void on_hizlitoolButton131_clicked();

    void on_hizlitoolButton132_clicked();

    void on_hizlitoolButton133_clicked();

    void on_hizlitoolButton134_clicked();

    void on_hizlitoolButton135_clicked();

    void on_hizlitoolButton136_clicked();

    void on_hizlitoolButton137_clicked();

    void on_hizlitoolButton138_clicked();

    void on_hizlitoolButton139_clicked();

    void on_hizlitoolButton140_clicked();

    void on_hizlitoolButton141_clicked();

    void on_hizlitoolButton142_clicked();

    void on_hizlitoolButton143_clicked();

    void on_hizlitoolButton144_clicked();

    void on_hizlitoolButton145_clicked();

    void on_hizlitoolButton146_clicked();

    void on_hizlitoolButton147_clicked();

    void on_hizlitoolButton148_clicked();

    void on_hizlitoolButton149_clicked();

    void on_hizlitoolButton150_clicked();

private:
    Ui::SatisForm *ui;
};

#endif // SATISFORM_H
