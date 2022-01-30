#ifndef STOKFROM_H
#define STOKFROM_H
#include "user.h"
#include "veritabani.h"
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

    int seciliIndex;
    bool yeniKayit = false;

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

    void on_bitisdateEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_baslangicdateEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_StokBirimBtn_clicked();

    void on_SatisGrafikBtn_clicked();

    void on_StokGirBtn_clicked();

    void on_StokCikBtn_clicked();

private:
    Ui::StokFrom *ui;

    User kullanici;
    Veritabani *vt = new Veritabani();
    QItemSelectionModel *selectionModel = new QItemSelectionModel();
};

#endif // STOKFROM_H
