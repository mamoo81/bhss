#ifndef CARIHAREKETIEKLEFORM_H
#define CARIHAREKETIEKLEFORM_H
#include "veritabani.h"
//****************************
#include <QDialog>

namespace Ui {
class CariHareketiEkleForm;
}

class CariHareketiEkleForm : public QDialog
{
    Q_OBJECT

public:
    explicit CariHareketiEkleForm(QWidget *parent = nullptr);
    ~CariHareketiEkleForm();

    enum FaturaTipi{
        alis = 1,
        satis = 2,
        iade = 3,
        odeme = 4,
        tahsilat = 5
    };
    Q_ENUM(FaturaTipi)
    enum OdemeTipi{
        nakit = 1,
        kredikarti = 2,
        havale = 3,
        cek = 4,
        senet = 5
    };
    Q_ENUM(OdemeTipi)
    Veritabani *vt = new Veritabani();
    QString cariID;
    Cari cariKart;
    FaturaTipi faturaTip;
    OdemeTipi odemeTip;


    void setCariID(const QString &newCariID);

    void setFaturaTip(FaturaTipi newFaturaTip);

    void setKullanici(const User &newKullanici);

private slots:
    void FormLoad();
    void on_KaydetpushButton_clicked();

    void on_OdemeTipcomboBox_currentIndexChanged(const QString &arg1);

    void on_YetkililineEdit_textChanged(const QString &arg1);

    void on_iptalpushButton_clicked();

private:
    User kullanici;

    Ui::CariHareketiEkleForm *ui;
};

#endif // CARIHAREKETIEKLEFORM_H
