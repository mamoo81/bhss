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
#include "carihareketiekleform.h"
#include "ui_carihareketiekleform.h"
//********************************************
#include <QDebug>
#include <QMessageBox>
#include <QMetaEnum>

CariHareketiEkleForm::CariHareketiEkleForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CariHareketiEkleForm)
{
    ui->setupUi(this);

    FormLoad();
}

CariHareketiEkleForm::~CariHareketiEkleForm()
{
    delete ui;
}

void CariHareketiEkleForm::setKullanici(const User &newKullanici)
{
    kullanici = newKullanici;
}

void CariHareketiEkleForm::setFaturaTip(FaturaTipi newFaturaTip)
{
    faturaTip = newFaturaTip;
    ui->AciklamaplainTextEdit->setPlainText(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(QVariant::fromValue(faturaTip).toString()) + " YAPILDI. YETKİLİ: " + ui->YetkililineEdit->text());
    if(faturaTip == 4){// cariye ödeme yapılacaksa yetkiliye kullanıcı adı yazsın
        ui->YetkililineEdit->setText(kullanici.getUserName());
    }
    else if(faturaTip == 5){// cariden tahsilat yapılacaksa yetkiliye tahsilat yapılacak cari karttaki cari yetkilisi yazsın.
        ui->YetkililineEdit->setText(cariKart.getYetkili());
    }
    else if(faturaTip == 1){// cari alacaklandırılacaksa yetkiliye carikart yetkilisi yazılacak
        ui->YetkililineEdit->setText(cariKart.getYetkili());
    }
    else if(faturaTip == 2){// cari borçlandırılacak ise yetkiliye satış yapan kullanıcı adı girilecek
        ui->YetkililineEdit->setText(kullanici.getUserName());
    }
}

void CariHareketiEkleForm::setCariID(const QString &newCariID)
{
    cariID = newCariID;
    cariKart = cariYonetimi.getCariKart(cariID);
    ui->unvanlineEdit->setText(cariKart.getAd());
    ui->islemdateEdit->setDate(QDate::currentDate());
    ui->OdemeTipcomboBox->addItems(vt->getOdemeTipleri());
}

void CariHareketiEkleForm::FormLoad()
{

}

void CariHareketiEkleForm::on_KaydetpushButton_clicked()
{
    switch (faturaTip) {
    case tahsilat://cariden tahsilat yapma
        if(ui->islemTutaridoubleSpinBox->value() >= 1){
            cariYonetimi.caridenTahsilatYap(cariID,
                                   ui->islemTutaridoubleSpinBox->value(),
                                   ui->islemdateEdit->dateTime(),
                                   faturaTip,
                                   odemeTip,
                                   kullanici,
                                   QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->EvrakNolineEdit->text()),
                                   QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->AciklamaplainTextEdit->toPlainText()));
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(QVariant::fromValue(faturaTip).toString()) + " Kaydedildi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            this->close();
        }else{
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Lütfen en az 1 TL veya daha büyük bir rakam giriniz!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        break;
    case odeme:// cariye ödeme yapma
        if(ui->islemTutaridoubleSpinBox->value() >= 1){
            double cariToplamAlacak = cariYonetimi.getCariToplamAlacak(cariID);
            if(cariToplamAlacak < ui->islemTutaridoubleSpinBox->value()){// cariye yapılan ödeme toplamAlacaktan büyükse
                ui->islemTutaridoubleSpinBox->setValue(cariToplamAlacak);
            }
            // cariye ödeme yapma
            cariYonetimi.cariyeOdemeYap(cariID,
                                   ui->islemTutaridoubleSpinBox->value(),
                                   ui->islemdateEdit->dateTime(),
                                   faturaTip,
                                   odemeTip,
                                   kullanici,
                                   QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->EvrakNolineEdit->text()),
                                   QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->AciklamaplainTextEdit->toPlainText()));
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(QVariant::fromValue(faturaTip).toString()) + " Kaydedildi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            this->close();
        }else{
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Lütfen en az 1 TL veya daha büyük bir rakam giriniz!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        break;
    case satis:// satis
        if(ui->islemTutaridoubleSpinBox->value() >= 1){
            cariYonetimi.cariyiAlacaklandır(cariID,
                                   ui->islemTutaridoubleSpinBox->value(),
                                   ui->islemdateEdit->dateTime(),
                                   faturaTip,
                                   odemeTip,
                                   kullanici,
                                   QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->EvrakNolineEdit->text()),
                                   QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->AciklamaplainTextEdit->toPlainText()));
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(QVariant::fromValue(faturaTip).toString()) + " Kaydedildi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            this->close();
        }else{
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Lütfen en az 1 TL veya daha büyük bir rakam giriniz!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        break;
    case alis:
        if(ui->islemTutaridoubleSpinBox->value() >= 1){
            cariYonetimi.cariyiBorclandir(cariID,
                                   ui->islemTutaridoubleSpinBox->value(),
                                   ui->islemdateEdit->dateTime(),
                                   faturaTip,
                                   odemeTip,
                                   kullanici,
                                   QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->EvrakNolineEdit->text()),
                                   QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->AciklamaplainTextEdit->toPlainText()));
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(QVariant::fromValue(faturaTip).toString()) + " Kaydedildi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            this->close();
        }else{
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Lütfen en az 1 TL veya daha büyük bir rakam giriniz!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        break;
    }

}

void CariHareketiEkleForm::on_OdemeTipcomboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    switch (ui->OdemeTipcomboBox->currentIndex()) {
    case 0:
        odemeTip = nakit;
        break;
    case 1:
        odemeTip = kredikarti;
        break;
    case 2:
        odemeTip = havale;
        break;
    case 3:
        odemeTip = cek;
        break;
    case 4:
        odemeTip = senet;
        break;
    }
}


void CariHareketiEkleForm::on_YetkililineEdit_textChanged(const QString &arg1)
{
    ui->AciklamaplainTextEdit->setPlainText(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(QVariant::fromValue(faturaTip).toString()) + " YAPILDI. YETKİLİ: " + arg1);
}


void CariHareketiEkleForm::on_iptalpushButton_clicked()
{
    this->close();
}

