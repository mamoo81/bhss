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
#ifndef SATISGRAFIGIFORM_H
#define SATISGRAFIGIFORM_H
#include "stokkarti.h"
#include "veritabani.h"
#include "stokyonetimi.h"
//***********************
#include <QDialog>
#include <QDate>
// chart ile ilgili
#include <QBarSet>
#include <QChart>
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QChartView>
#include <QCategoryAxis>
#include <QHash>

#include <QtCharts>
// veya alttaki flagı kullanabilirsin.
//QT_CHARTS_USE_NAMESPACE

namespace Ui {
class SatisGrafigiForm;
}

class SatisGrafigiForm : public QDialog
{
    Q_OBJECT

    enum aralik{
        Gunluk = 0,
        Haftalik = 1,
        Aylik = 2,
        Yillik = 3
    };

public:
    explicit SatisGrafigiForm(QWidget *parent = nullptr);
    ~SatisGrafigiForm();

    QBarSet *barset;
    QBarSeries *barSeries;
    QBarCategoryAxis *categoryaxis;

    QStringList gunler;
    QStringList gunlerTamFormat;
    QStringList aylar;
    QStringList aylarTamFormat;
    QStringList yillar;
    QStringList yillarTamFormat;

    Veritabani vt = Veritabani();
    StokYonetimi stokYonetimi = StokYonetimi();

    QChart *chart = new QChart();
    QChartView *chartview = new QChartView(chart);

    void setStokKarti(StokKarti gosterilecekKart);

    void zamanAraligiAyarla();

private slots:
    void FormLoad();

    void on_gunlukradioButton_clicked();

    void on_aylikradioButton_clicked();

    void on_bitisdateEdit_dateChanged(const QDate &date);

    void on_yillikradioButton_clicked();

    void on_gosterpushButton_clicked();

    void on_EklepushButton_clicked();

private:
    Ui::SatisGrafigiForm *ui;

    StokKarti kart;
};

#endif // SATISGRAFIGIFORM_H
