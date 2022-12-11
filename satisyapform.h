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
#ifndef SATISYAPFORM_H
#define SATISYAPFORM_H
#include "sepet.h"
#include "user.h"
#include "cari.h"
#include "yazici.h"
#include "faturayonetimi.h"
#include "cariyonetimi.h"
//******************
#include <QDialog>
#include <QSound>

namespace Ui {
class SatisYapForm;
}

class SatisYapForm : public QDialog
{
    Q_OBJECT

public:
    explicit SatisYapForm(QWidget *parent = nullptr);
    ~SatisYapForm();

    QSound *odemeAlindi = new QSound(":/sounds/sounds/odeme-alindi.wav");
//    Veritabani vt_satisFormu;
    FaturaYonetimi faturaYonetimi = FaturaYonetimi();
    CariYonetimi cariYonetimi = CariYonetimi();
    QList<Cari> cariKartlar;
    Yazici fisYazici;
    bool satisYapildimi;

    void setSatilacakSepet(const Sepet &newSatilacakSepet);

    void setKullanici(const User &newKullanici);

public slots:
    void formLoad();

private slots:

    void on_satBtn_clicked();

    void on_iptalBtn_clicked();

    void on_OdenendoubleSpinBox_valueChanged(double arg1);

    void keyPressEvent(QKeyEvent *e);
    bool eventFilter(QObject *filtrelenecekObje, QEvent *event);
    void on_toolButton200TL_clicked();

    void on_toolButton100TL_clicked();

    void on_toolButton50TL_clicked();

    void on_toolButton20TL_clicked();

    void on_toolButton10TL_clicked();

    void on_toolButton5TL_clicked();

    void on_toolButton1TL_clicked();

    void on_toolButton50krs_clicked();

    void on_toolButton25krs_clicked();

    void on_toolButton10krs_clicked();

    void on_toolButton5krs_clicked();

    void on_toolButton1krs_clicked();

    void on_caricomboBox_currentIndexChanged(int index);

private:
    Ui::SatisYapForm *ui;

    Sepet satilacakSepet;
    User kullanici;
    bool butonilkBasma = true;
};

#endif // SATISYAPFORM_H
