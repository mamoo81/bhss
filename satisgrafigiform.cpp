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
#include "satisgrafigiform.h"
#include "ui_satisgrafigiform.h"
//********************************
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QLegend>
#include <QBarCategoryAxis>
#include <QHorizontalBarSeries>
#include <QLineSeries>
#include <QCategoryAxis>
#include <QPieSeries>
#include <QPieSlice>

QT_CHARTS_USE_NAMESPACE

SatisGrafigiForm::SatisGrafigiForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SatisGrafigiForm)
{
    ui->setupUi(this);

    FormLoad();
}

void SatisGrafigiForm::FormLoad()
{
    QBarSet *barset = new QBarSet("EKMEK");

    *barset << 10 << 5 << 7 << 3 << 6 << 2 << 1 << 12 << 8 << 8 << 11 << 20;

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barset);

    QChart *chart = new QChart();
    chart->addSeries(barSeries);
    chart->setTitle("satış grafiği");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList aylar;
    aylar << "OCAK" << "ŞUBAT" << "MART" << "NİSAN" << "MAYIS" << "HAZİRAN" << "TEMMUZ" << "AĞUSTOS" << "EYLÜL" << "EKİM" << "KASIM" << "ARALIK";

    QBarCategoryAxis *categoryaxis = new QBarCategoryAxis();
    categoryaxis->append(aylar);

    chart->createDefaultAxes();
    chart->setAxisX(categoryaxis, barSeries);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);


    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);

    chartview->setParent(ui->horizontalFrame);
    chartview->setFixedSize(ui->horizontalFrame->size());
}

SatisGrafigiForm::~SatisGrafigiForm()
{
    delete ui;
}

void SatisGrafigiForm::AralıkBelirle(QDate baslangicTarih, QDate bitisTarih)
{

}
