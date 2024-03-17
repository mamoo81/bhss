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
#include <QTextCodec>

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
    odemeAlindi.setLoopCount(0);
    satisYapildimi = false;

    //genel ayarların okunması başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    //yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("fis-yazici");
    ui->fischeckBox->setChecked(genelAyarlar.value("herZaman").toBool());
    genelAyarlar.endGroup();
    //yazici ayarlari okuma bitiş

    //genel ayarların okunması bitiş

    ui->OdenendoubleSpinBox->installEventFilter(this);

//    QCompleter *tamamlayici = new QCompleter(cariAdlari, this);
//    tamamlayici->setCompletionMode(QCompleter::InlineCompletion);
//    tamamlayici->setCaseSensitivity(Qt::CaseInsensitive);
//    ui->CariLineEdit->setCompleter(tamamlayici);
    cariKartlar = cariYonetimi.getCariKartlar();
    ui->caricomboBox->setLocale(QLocale(QLocale::Turkish, QLocale::Turkey));
    ui->caricomboBox->clear();
    ui->caricomboBox->addItem("DİREKT");
    foreach (Cari cariKart, cariKartlar) {
        ui->caricomboBox->addItem(cariKart.getAd());
    }
    ui->OdenendoubleSpinBox->setFocus();
    odemeAlindi.setSource(QUrl("qrc:/sounds/sounds/warning-sound.wav"));
}

void SatisYapForm::on_satBtn_clicked()
{
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);

    // ------------------------------------------   satış yapılan cari DİREKT ise ödenen tutarı sepet toplam tutarı olarak al. değilse kullanıcının girdiği tutarı baz al.
    if(ui->caricomboBox->currentText().contains("DİREKT", Qt::CaseInsensitive)){
        satilacakSepet.setOdenenTutar(satilacakSepet.sepetToplamTutari());

        // ------------------------------------------   faturaYonetimi clasına satiş gönderme direkt carisine
        faturaYonetimi.satisYap(satilacakSepet, kullanici, 1);
    }
    else{
        satilacakSepet.setOdenenTutar(ui->OdenendoubleSpinBox->value());

        // ------------------------------------------   faturaYonetimi clasına satiş gönderme seçili cariye
        satilacakSepet.setFazlaTutarAlacaklandir(ui->fazlaTutarcheckBox->isChecked());// fazla ödenen tutar cariye alacaklı olarak yazılsın mı.
        faturaYonetimi.satisYap(satilacakSepet, kullanici, cariKartlar.at(ui->caricomboBox->currentIndex() - 1).getId()); // -1 yapıyorum ki caricombobox'a direkt cari satırını manuel eklediğim için index karışmasın.
    }

    // ------------------------------------------   fiş yazdırma
    if(ui->fischeckBox->isChecked()){
        QString sonIslemNo = faturaYonetimi.sonIslemNumarasi();
        fisYazici.fisBas(sonIslemNo, satilacakSepet);
    }

    // ------------------------------------------   ödeme alındı sesi çalınsın mı.
    genelAyarlar.beginGroup("uyari-sesleri");
    if(genelAyarlar.value("odeme-alindi").toBool()){
        odemeAlindi.play();
    }
    genelAyarlar.endGroup();

    ui->OdenendoubleSpinBox->setValue(0);
    ui->toplamLBL->setText(0);
    satisYapildimi = true;
    this->close();
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
        if(ui->OdenendoubleSpinBox->value() < 99999){// yanlışlıkla barkod okuturlarsa satışı yapmasın düzgün tutar girilsin
            SatisYapForm::on_satBtn_clicked();
        }
        else{
            ui->OdenendoubleSpinBox->setValue(satilacakSepet.sepetToplamTutari());
            ui->OdenendoubleSpinBox->selectAll();
            ui->OdenendoubleSpinBox->setFocus();
        }
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

    on_caricomboBox_currentIndexChanged(ui->caricomboBox->currentIndex());
}

void SatisYapForm::setKullanici(const User &newKullanici)
{
    kullanici = newKullanici;
}


void SatisYapForm::on_toolButton200TL_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(200);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 200);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton100TL_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(100);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 100);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton50TL_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(50);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 50);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton20TL_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(20);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 20);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton10TL_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(10);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 10);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton5TL_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(5);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 5);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton1TL_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(1);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 1);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton50krs_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(0.50);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 0.50);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton25krs_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(0.25);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 0.25);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton10krs_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(0.10);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 0.10);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton5krs_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(0.05);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 0.05);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_toolButton1krs_clicked()
{
    if(butonilkBasma){
        ui->OdenendoubleSpinBox->setValue(0.01);
    }
    else{
        ui->OdenendoubleSpinBox->setValue(ui->OdenendoubleSpinBox->value() + 0.01);
    }
    ui->OdenendoubleSpinBox->setFocus();
    ui->OdenendoubleSpinBox->selectAll();
    butonilkBasma = false;
}


void SatisYapForm::on_caricomboBox_currentIndexChanged(int index)
{
    // cari seçili ise ve ödenen tutar büyükse sepet toplam tutarından
    if(index != 0 && satilacakSepet.sepetToplamTutari() < ui->OdenendoubleSpinBox->value()){
        ui->fazlaTutarcheckBox->setEnabled(true);
    }
    else{// cari seçili ise
        ui->fazlaTutarcheckBox->setEnabled(false);
    }
}

