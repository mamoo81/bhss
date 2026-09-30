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
#include <QHash>

// QT_CHARTS_USE_NAMESPACE

SatisGrafigiForm::SatisGrafigiForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SatisGrafigiForm)
{
    ui->setupUi(this);

    FormLoad();
}

void SatisGrafigiForm::FormLoad()
{
    ui->bitisdateEdit->setDate(QDate::currentDate());

    // chartview in sayfaya yerleştirilmesi
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setParent(ui->horizontalFrame);
    chartview->setFixedSize(ui->horizontalFrame->size());

    // form açıldığında günlük seçili olacağı için
    // chart başlığını ayarlama
    chart->setTitleFont(QFont("Monospace", 14, QFont::Bold));
    chart->setTitle("Günlük satış grafiği");


}

SatisGrafigiForm::~SatisGrafigiForm()
{
    delete ui;
}

void SatisGrafigiForm::setStokKarti(StokKarti gosterilecekKart)
{
    kart = gosterilecekKart;
}

void SatisGrafigiForm::on_gunlukradioButton_clicked()
{
    ui->baslangicdateEdit->setCurrentSection(QDateEdit::DaySection);
    ui->baslangicdateEdit->setDisplayFormat("dd.MM.yyyy");
    ui->baslangicdateEdit->setCalendarPopup(true);
    ui->bitisdateEdit->setCurrentSection(QDateEdit::DaySection);
    ui->bitisdateEdit->setDisplayFormat("dd.MM.yyyy");
    ui->bitisdateEdit->setCalendarPopup(true);

    ui->baslangicdateEdit->setDate(ui->bitisdateEdit->date().addDays(-60));

    // chart başlığını ayarlama
    chart->setTitleFont(QFont("Monospace", 14, QFont::Bold));
    chart->setTitle("Günlük satış grafiği");
}

void SatisGrafigiForm::on_aylikradioButton_clicked()
{
    ui->baslangicdateEdit->setCurrentSection(QDateEdit::MonthSection);
    ui->baslangicdateEdit->setDisplayFormat("MM.yyyy");
    ui->baslangicdateEdit->setCalendarPopup(true);
    ui->bitisdateEdit->setCurrentSection(QDateEdit::MonthSection);
    ui->bitisdateEdit->setDisplayFormat("MM.yyyy");
    ui->bitisdateEdit->setCalendarPopup(true);

    ui->baslangicdateEdit->setDate(ui->bitisdateEdit->date().addDays(-730));

    // chart başlığını ayarlama
    chart->setTitleFont(QFont("Monospace", 14, QFont::Bold));
    chart->setTitle("Aylık satış grafiği");
}

void SatisGrafigiForm::on_bitisdateEdit_dateChanged(const QDate &date)
{
    if(ui->gunlukradioButton->isChecked()){
        ui->baslangicdateEdit->setDate(date.addMonths(-1));
    }
    if(ui->aylikradioButton->isChecked()){
        ui->baslangicdateEdit->setDate(date.addYears(-2));
    }
    if(ui->yillikradioButton->isChecked()){
        ui->baslangicdateEdit->setDate(date.addYears(-24));
    }
}

void SatisGrafigiForm::on_yillikradioButton_clicked()
{
    ui->baslangicdateEdit->setCurrentSection(QDateEdit::YearSection);
    ui->baslangicdateEdit->setDisplayFormat("yyyy");
    ui->baslangicdateEdit->setCalendarPopup(true);
    ui->bitisdateEdit->setCurrentSection(QDateEdit::YearSection);
    ui->bitisdateEdit->setDisplayFormat("yyyy");
    ui->bitisdateEdit->setCalendarPopup(true);

    ui->baslangicdateEdit->setDate(ui->bitisdateEdit->date().addDays(-7300));

    // chart başlığını ayarlama
    chart->setTitleFont(QFont("Monospace", 14, QFont::Bold));
    chart->setTitle("Yıllık satış grafiği");
}

void SatisGrafigiForm::on_gosterpushButton_clicked()
{
    barset = new QBarSet(kart.getAd());
    barSeries = new QBarSeries();
    categoryaxis = new QBarCategoryAxis();

    QStringList etiketler;
    QStringList anahtarlar;
    QHash<QString, float> adetlerList;
    QDate ilk = ui->baslangicdateEdit->date();
    QDate son = ui->bitisdateEdit->date();

    if(ui->gunlukradioButton->isChecked()){
        for (QDate d = ilk; d <= son; d = d.addDays(1)) {
            etiketler.append(d.toString("dd.MM.yyyy dddd"));
            anahtarlar.append(d.toString("dd.MM.yyyy"));
        }
        adetlerList = stokYonetimi.getgunlukAdetler(ilk, son, kart);
    }
    else if(ui->aylikradioButton->isChecked()){
        QDate d(ilk.year(), ilk.month(), 1);
        QDate sonAy(son.year(), son.month(), 1);
        for (; d <= sonAy; d = d.addMonths(1)) {
            etiketler.append(d.toString("MM.yyyy MMMM"));
            anahtarlar.append(d.toString("MM.yyyy MMMM"));
        }
        adetlerList = stokYonetimi.getAylikAdetler(ilk, son, kart);
    }
    else if(ui->yillikradioButton->isChecked()){
        for (int y = ilk.year(); y <= son.year(); ++y) {
            etiketler.append(QString::number(y));
            anahtarlar.append(QString::number(y));
        }
        adetlerList = stokYonetimi.getYillikAdetler(ilk, son, kart);
    }
    categoryaxis->append(etiketler);

    float enAz = 0, enCok = 0, toplam = 0;
    for (int i = 0; i < anahtarlar.count(); ++i) {
        float v = adetlerList.value(anahtarlar.at(i), 0);
        barset->append(v);
        toplam += v;
        if(i == 0 || v < enAz){
            enAz = v;
        }
        if(i == 0 || v > enCok){
            enCok = v;
        }
    }
    ui->EnAzlabel->setText(QString::number(enAz));
    ui->EnCoklabel->setText(QString::number(enCok));
    ui->Ortalamalabel->setText(QString::number(anahtarlar.isEmpty() ? 0 : toplam / anahtarlar.count(), 'f', 2));

    barSeries->setBarWidth(1);
    barSeries->setVisible(true);
    barSeries->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    barSeries->setLabelsVisible(true);
    barSeries->append(barset);

    chart->removeAllSeries();
    const auto eskiEksenler = chart->axes();
    for (auto eksen : eskiEksenler) {
        chart->removeAxis(eksen);
        eksen->deleteLater();
    }
    chart->addSeries(barSeries);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    categoryaxis->setLabelsAngle(90);
    categoryaxis->setLabelsVisible(true);

    chart->addAxis(categoryaxis, Qt::AlignBottom);
    barSeries->attachAxis(categoryaxis);
    QValueAxis *degerEkseni = new QValueAxis();
    degerEkseni->setLabelFormat("%g");
    degerEkseni->setRange(qMin(0.0f, enAz), qMax(1.0f, enCok));
    degerEkseni->applyNiceNumbers();
    chart->addAxis(degerEkseni, Qt::AlignLeft);
    barSeries->attachAxis(degerEkseni);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}

void SatisGrafigiForm::on_EklepushButton_clicked()
{

}
