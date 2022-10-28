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
    // ilkleme
    barset = new QBarSet(kart.getAd());
    barSeries = new QBarSeries();
    categoryaxis = new QBarCategoryAxis();

    if(ui->gunlukradioButton->isChecked()){

        // günlük bazında gösterileceği için 2 tarih arası kaç gün olduğunu bulma
        qint64 kacGun = ui->bitisdateEdit->date().toJulianDay() - ui->baslangicdateEdit->date().toJulianDay();

        // gün sayısı kadar günleri listeye ekleme
        for (int var = 0; var <= kacGun; ++var) {
            gunler.append(ui->baslangicdateEdit->date().addDays(var).toString("dd.MM.yyyy dddd"));
            gunlerTamFormat.append(ui->baslangicdateEdit->date().addDays(var).toString());// aşağıdaki karşılaştırma için.
        }
        categoryaxis->append(gunler);// gün adlarını chart altına yazıyorum.

        // adetleri bulma ve girme
        QHash<QString, float> adetlerList = vt.getgunlukAdetler(ui->baslangicdateEdit->date(), ui->bitisdateEdit->date(), kart);
        if(!adetlerList.isEmpty()){
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
        else{
            ui->EnAzlabel->setText(QString::number(0));
            ui->EnCoklabel->setText(QString::number(0));
            ui->Ortalamalabel->setText(QString::number(0));
        }
    }
    if(ui->aylikradioButton->isChecked()){

        // aylik bazında gösterileceği için 2 tarih arası kaç gün olduğunu bulma
        qint64 kacGunluk = ui->bitisdateEdit->date().toJulianDay() - ui->baslangicdateEdit->date().toJulianDay();

        int KacAylik = kacGunluk/30;

        // ay sayısı kadar ayları listeye ekleme
        for (int var = 0; var <= KacAylik; ++var) {
            aylar.append(ui->baslangicdateEdit->date().addMonths(var).toString("MM.yyyy MMMM"));
            aylarTamFormat.append(ui->baslangicdateEdit->date().addMonths(var).toString("MM.yyyy MMMM"));// aşağıdaki karşılaştırma için.
        }
        categoryaxis->append(aylar);// ay adlarını chart altına yazıyorum.

        // adetleri bulma ve girme
        QHash<QString, float> adetlerList = vt.getAylikAdetler(ui->baslangicdateEdit->date(), ui->bitisdateEdit->date(), kart);
        if(!adetlerList.isEmpty()){
            for (int var = 0; var < aylar.count(); ++var) {
                if(adetlerList.contains(aylarTamFormat.value(var))){
                    barset->append(adetlerList.value(aylarTamFormat.at(var)));
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
        else{
            ui->EnAzlabel->setText(QString::number(0));
            ui->EnCoklabel->setText(QString::number(0));
            ui->Ortalamalabel->setText(QString::number(0));
        }
    }
    if(ui->yillikradioButton->isChecked()){

        // yıllık bazında gösterileceği için 2 tarih arası kaç gün olduğunu bulma
        qint64 kacGunluk = ui->bitisdateEdit->date().toJulianDay() - ui->baslangicdateEdit->date().toJulianDay();

        int KacYillik = kacGunluk/365;

        // yıl sayısı kadar yılları listeye ekleme
        for (int var = 0; var <= KacYillik; ++var) {
            yillar.append(ui->baslangicdateEdit->date().addYears(var).toString("yyyy"));
            yillarTamFormat.append(ui->baslangicdateEdit->date().addYears(var).toString("yyyy"));// aşağıdaki karşılaştırma için.
        }
        categoryaxis->append(yillar);// yıl adlarını chart altına yazıyorum.

        // adetleri bulma ve girme
        QHash<QString, float> adetlerList = vt.getYillikAdetler(ui->baslangicdateEdit->date(), ui->bitisdateEdit->date(), kart);
        if(!adetlerList.isEmpty()){
            for (int var = 0; var < yillar.count(); ++var) {
                if(adetlerList.contains(yillarTamFormat.value(var))){
                    barset->append(adetlerList.value(yillarTamFormat.at(var)));
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
        else{
            ui->EnAzlabel->setText(QString::number(0));
            ui->EnCoklabel->setText(QString::number(0));
            ui->Ortalamalabel->setText(QString::number(0));
        }
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

void SatisGrafigiForm::on_EklepushButton_clicked()
{

}
