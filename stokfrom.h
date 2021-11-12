#ifndef STOKFROM_H
#define STOKFROM_H
#include "user.h"
//************************
#include <QDialog>



namespace Ui {
class StokFrom;
}

class StokFrom : public QDialog
{
    Q_OBJECT

public:
    explicit StokFrom(QWidget *parent = nullptr);
    ~StokFrom();



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

    void on_BckBtn_clicked();

    void on_Fwbtn_clicked();

    void stokKartiAra(QString aranacakMetin);

    void on_araBtn_clicked();
    void on_dosyadanToolButton_clicked();

private:
    Ui::StokFrom *ui;

    User kullanici;
};

#endif // STOKFROM_H
