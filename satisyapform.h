#ifndef SATISYAPFORM_H
#define SATISYAPFORM_H
#include "veritabani.h"
#include "sepet.h"
#include "user.h"
#include "cari.h"
//******************
#include <QDialog>

namespace Ui {
class SatisYapForm;
}

class SatisYapForm : public QDialog
{
    Q_OBJECT

public:
    explicit SatisYapForm(QWidget *parent = nullptr);
    ~SatisYapForm();

    Veritabani vt_satisFormu;
    QList<QString> cariAdlari;

    void setSatilacakSepet(const Sepet &newSatilacakSepet);

    void setKullanici(const User &newKullanici);

public slots:
    void formLoad();

private slots:


    void on_satBtn_clicked();

    void on_iptalBtn_clicked();

    void on_OdenendoubleSpinBox_valueChanged(double arg1);

private:
    Ui::SatisYapForm *ui;

    Sepet satilacakSepet;
    User kullanici;
};

#endif // SATISYAPFORM_H
