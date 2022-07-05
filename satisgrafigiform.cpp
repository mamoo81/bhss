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
#include <QBarSet>
#include <QChart>
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QChartView>
#include <QCategoryAxis>
#include <QHash>

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
    ui->bitisdateEdit->setDate(QDate::currentDate());

    // chartview in sayfaya yerleştirilmesi
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setParent(ui->horizontalFrame);
    chartview->setFixedSize(ui->horizontalFrame->size());
}

SatisGrafigiForm::~SatisGrafigiForm()
{
    delete ui;
}

void SatisGrafigiForm::AralıkBelirle()
{
    if(ui->gunlukradioButton->isChecked()){

    }
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
}

void SatisGrafigiForm::on_gosterpushButton_clicked()
{
    QBarSet *barset = new QBarSet(kart.getAd());
    QBarSeries *barSeries = new QBarSeries();
    QBarCategoryAxis *categoryaxis = new QBarCategoryAxis();

    if(ui->gunlukradioButton->isChecked()){

        // chart başlığını ayarlama
        chart->setTitle("Günlük satış grafiği");

        // günlük bazında gösterileceği için 2 tarih arası kaç gün olduğunu bulma
        qint64 kacGun = ui->bitisdateEdit->date().toJulianDay() - ui->baslangicdateEdit->date().toJulianDay();

        // gün sayısı kadar günleri listeye ekleme
        QStringList gunler;
        QStringList gunlerTamFormat;
        for (int var = 0; var <= kacGun; ++var) {
            gunler.append(ui->baslangicdateEdit->date().addDays(var).toString("dd.MM.yyyy dddd"));
            gunlerTamFormat.append(ui->baslangicdateEdit->date().addDays(var).toString());// aşağıdaki karşılaştırma için.
        }
        categoryaxis->append(gunler);// gün adlarını chart altına yazıyorum.

        // adetleri bulma ve girme
        QHash<QString, float> adetlerList = vt.getgunlukAdetler(ui->baslangicdateEdit->date(), ui->bitisdateEdit->date(), kart);
        for (int var = 0; var < gunler.count(); ++var) {
            if(adetlerList.contains(gunlerTamFormat.value(var))){
                barset->append(adetlerList.value(gunlerTamFormat.at(var)));
            }
            else{
                barset->append(0);
            }
        }
        // en çok, en az ve ortalama alma
        QList<float> lii = adetlerList.values();

        qSort(lii.begin(), lii.end());
        ui->EnAzlabel->setText(QString::number(lii.first()));
        ui->EnCoklabel->setText(QString::number(lii.last()));
        float toplam = 0;
        for (int var = 0; var < lii.count(); ++var) {
            toplam = toplam + lii.value(var);
        }
        ui->Ortalamalabel->setText(QString::number((toplam / lii.count()), 'f', 2));
    }
    if(ui->aylikradioButton->isChecked()){
        // chart başlığını ayarlama
        chart->setTitle("Aylık satış grafiği");
    }


    //önemsiz detaylar.
    barSeries->setBarWidth(1);
    barSeries->setVisible(true);
    barSeries->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    barSeries->setLabelsVisible(true);
    //
    barSeries->append(barset);

    chart->removeAllSeries();
    chart->addSeries(barSeries);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    categoryaxis->setLabelsAngle(90);
    categoryaxis->setLabelsVisible(true);

    chart->createDefaultAxes();
    chart->setAxisX(categoryaxis, barSeries);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

}
