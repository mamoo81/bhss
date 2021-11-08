#include "satisyapform.h"
#include "ui_satisyapform.h"
#include "satisform.h"
#include "sepet.h"
//***************************
#include <QString>
#include <QCompleter>
#include <QDebug>
#include <QList>

SatisYapForm::SatisYapForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SatisYapForm)
{
    ui->setupUi(this);

    formLoad();
}

SatisYapForm::~SatisYapForm()
{
    delete ui;
}

void SatisYapForm::formLoad()
{
    satisYapildimi = false;
    cariKartlar = vt_satisFormu.getCariKartlar();
    foreach (auto cari, cariKartlar) {
        cariAdlari.append(cari.getAd());
    }
    QCompleter *tamamlayici = new QCompleter(cariAdlari, this);
    tamamlayici->setCompletionMode(QCompleter::InlineCompletion);
    tamamlayici->setCaseSensitivity(Qt::CaseInsensitive);
    ui->CariLineEdit->setCompleter(tamamlayici);
    ui->OdenendoubleSpinBox->setFocus();
}


void SatisYapForm::on_satBtn_clicked()
{
    if(cariAdlari.contains(ui->CariLineEdit->text())){
        int index = cariAdlari.indexOf(ui->CariLineEdit->text());
        //veritabani clasına satiş gönderme
        vt_satisFormu.satisYap(satilacakSepet, kullanici, cariKartlar.at(index).getId());

        ui->OdenendoubleSpinBox->setValue(0);
        ui->toplamLBL->setText(0);
        satisYapildimi = true;
        this->close();
    }
    else{
        QMessageBox::information(this, "Dikkat", "Girilen cari hesap bulunamadı tekrar kontrol ediniz.", QMessageBox::Ok);
    }
}

void SatisYapForm::setSatilacakSepet(const Sepet &newSatilacakSepet)
{
    satilacakSepet = newSatilacakSepet;
    ui->toplamLBL->setText("₺" + QString::number(satilacakSepet.sepetToplamTutari()));
}


void SatisYapForm::on_iptalBtn_clicked()
{
    ui->OdenendoubleSpinBox->setValue(0);
    ui->toplamLBL->setText(0);
    this->close();
}



void SatisYapForm::on_OdenendoubleSpinBox_valueChanged(double arg1)
{
    double toplamTutar = satilacakSepet.sepetToplamTutari();
    double paraUstu = arg1 - toplamTutar;
    ui->ParaUstuLBL->setText("₺" + QString::number(paraUstu));
    if(paraUstu > 0){
        ui->ParaUstuLBL->setStyleSheet("QLabel {color: green;}");
    }
    else{
        ui->ParaUstuLBL->setStyleSheet("QLabel {color: red;}");
    }
}

void SatisYapForm::setKullanici(const User &newKullanici)
{
    kullanici = newKullanici;
}

