#include "satisyapform.h"
#include "ui_satisyapform.h"
#include "satisform.h"
//***************************
#include <QString>
#include <QCompleter>
#include <QDebug>
#include <QList>
#include <QSettings>
#include <QStandardPaths>
#include <QKeyEvent>

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

    //genel ayarların okunması başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    //yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("fis-yazici");
    ui->fischeckBox->setChecked(genelAyarlar.value("herZaman").toBool());
    genelAyarlar.endGroup();
    //yazici ayarlari okuma bitiş

    //genel ayarların okunması bitiş

    ui->OdenendoubleSpinBox->installEventFilter(this);
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
        satilacakSepet.setOdenenTutar(ui->OdenendoubleSpinBox->value());
        vt_satisFormu.satisYap(satilacakSepet, kullanici, cariKartlar.at(index).getId());
        // fiş yazdırma
        if(ui->fischeckBox->isChecked()){
            QString sonIslemNo = vt_satisFormu.sonIslemNumarasi();
            fisYazici.fisBas(sonIslemNo, satilacakSepet);
        }
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
    ui->toplamLBL->setText("₺" + QString::number(satilacakSepet.sepetToplamTutari(), 'f', 2));
    ui->OdenendoubleSpinBox->setValue(satilacakSepet.sepetToplamTutari());
}


void SatisYapForm::on_iptalBtn_clicked()
{
    ui->OdenendoubleSpinBox->setValue(0);
    ui->toplamLBL->setText(0);
    this->close();
}

void SatisYapForm::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){
        SatisYapForm::on_satBtn_clicked();
    }
}

bool SatisYapForm::eventFilter(QObject *filtrelenecekObje, QEvent *event){
    if(filtrelenecekObje == ui->OdenendoubleSpinBox && event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_F){
            if(ui->fischeckBox->isChecked()){
                ui->fischeckBox->setChecked(false);
            }
            else{
                ui->fischeckBox->setChecked(true);
            }
            return true;
        }
        if(keyEvent->key() == Qt::Key_Escape){
            SatisYapForm::on_iptalBtn_clicked();
        }
        return false;
    }
    return false;
}

void SatisYapForm::on_OdenendoubleSpinBox_valueChanged(double arg1)
{
    double toplamTutar = satilacakSepet.sepetToplamTutari();
    double paraUstu = (arg1 - toplamTutar);
    ui->ParaUstuLBL->setText("₺" + QString::number(paraUstu, 'f', 2));
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

