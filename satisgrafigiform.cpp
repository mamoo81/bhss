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

    QPalette qpalette = qApp->palette();
    qpalette.setColor(QPalette::Window, QRgb(0xffffff));
    qpalette.setColor(QPalette::WindowText, QRgb(0x404040));
    qApp->setPalette(qpalette);

    chartview->setParent(ui->horizontalFrame);
}

SatisGrafigiForm::~SatisGrafigiForm()
{
    delete ui;
}
