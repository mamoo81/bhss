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
#include "yenicarikartdialog.h"
#include "ui_yenicarikartdialog.h"
#include "cari.h"
//************************************
#include <QCompleter>
#include <QMessageBox>

YeniCariKartDialog::YeniCariKartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YeniCariKartDialog)
{
    ui->setupUi(this);
    FormLoad();
}

YeniCariKartDialog::~YeniCariKartDialog()
{
    delete ui;
}

void YeniCariKartDialog::setDuzenle(bool newDuzenle)
{
    duzenle = newDuzenle;
}

void YeniCariKartDialog::FormLoad()
{
    ui->CariTipicomboBox->addItems(vt->getCariTipleri());
    // vergi dairelerni veritabanından alıp ilgili objeye yazarken otomatik doldurma için.
    QCompleter *tamamlayici = new QCompleter(vt->getVergiDaireleri(), this);
    tamamlayici->setCompletionMode(QCompleter::PopupCompletion);
    tamamlayici->setCaseSensitivity(Qt::CaseInsensitive);
    ui->VergiDairesilineEdit->setCompleter(tamamlayici);
    //
    ui->ilcomboBox->addItems(vt->getIller());
    ui->CariAdilineEdit->setFocus();
}

void YeniCariKartDialog::on_iptalpushButton_clicked()
{
    this->close();
}


void YeniCariKartDialog::on_ilcomboBox_currentIndexChanged(int index)
{
    ui->ilcecomboBox->clear();
    ui->ilcecomboBox->addItems(vt->getIlceler(index));
}


void YeniCariKartDialog::on_KaydetpushButton_clicked()
{
    QLocale turkce(QLocale::Turkish, QLocale::Turkey);
    if(duzenle){
        duzenlenecekCariKart.setAd(turkce.toUpper(ui->CariAdilineEdit->text()));
        duzenlenecekCariKart.setTip(ui->CariTipicomboBox->currentIndex() + 1);// caritipleri tablosunda ki tip id'sine denk gelmesi için +1 ekliyorum.
        duzenlenecekCariKart.setVerigino(ui->VergiNolineEdit->text());
        duzenlenecekCariKart.setVergiDaire(turkce.toUpper(ui->VergiDairesilineEdit->text()));
        duzenlenecekCariKart.setIl(turkce.toUpper(ui->ilcomboBox->currentText()));
        duzenlenecekCariKart.setIlce(turkce.toUpper(ui->ilcecomboBox->currentText()));
        duzenlenecekCariKart.setAdres(turkce.toUpper(ui->AdresplainTextEdit->toPlainText()));
        duzenlenecekCariKart.setMail(ui->MaillineEdit->text());
        duzenlenecekCariKart.setTelefon(ui->TelefonlineEdit->text());
        duzenlenecekCariKart.setAciklama(turkce.toUpper(ui->AciklamaplainTextEdit->toPlainText()));
        duzenlenecekCariKart.setYetkili(turkce.toUpper(ui->YetkililineEdit->text()));
        bool sonuc = vt->cariKartDuzenle(duzenlenecekCariKart);
        if(sonuc){
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Cari kart düzenlendi");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            this->close();
        }
        else{
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Cari kart düzenlenlenemedi!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
    else{
        Cari yeniCari;
        yeniCari.setTarih(QDateTime::currentDateTime());
        yeniCari.setAd(turkce.toUpper(ui->CariAdilineEdit->text()));
        yeniCari.setYetkili(turkce.toUpper(ui->YetkililineEdit->text()));
        yeniCari.setTip(ui->CariTipicomboBox->currentIndex() + 1);// currentindex 'e +1 ekliyorum. veritabanındaki caritipleri id'sine denk gelsin diye
        yeniCari.setVerigino(ui->VergiNolineEdit->text());
        yeniCari.setVergiDaire(turkce.toUpper(ui->VergiDairesilineEdit->text()));
        yeniCari.setIl(turkce.toUpper(ui->ilcomboBox->currentText()));
        yeniCari.setIlce(turkce.toUpper(ui->ilcecomboBox->currentText()));
        yeniCari.setAdres(turkce.toUpper(ui->AdresplainTextEdit->toPlainText()));
        yeniCari.setMail(ui->MaillineEdit->text());
        yeniCari.setTelefon(ui->TelefonlineEdit->text());
        yeniCari.setAciklama(turkce.toUpper(ui->AciklamaplainTextEdit->placeholderText()));
        vt->yeniCariKart(yeniCari);
    }
    this->close();
}

void YeniCariKartDialog::setDuzenlenecekCariID(const QString &newDuzenlenecekCariID)
{
    duzenlenecekCariID = newDuzenlenecekCariID;
    if(duzenle){
        duzenlenecekCariKart = vt->getCariKart(duzenlenecekCariID);
        ui->CariAdilineEdit->setText(duzenlenecekCariKart.getAd());
        ui->YetkililineEdit->setText(duzenlenecekCariKart.getYetkili());
        ui->CariTipicomboBox->setCurrentIndex(duzenlenecekCariKart.getTip() - 1);// currentindex 'e -1 ekliyorum. veritabanındaki caritipleri id'sine denk gelsin diye
        ui->VergiNolineEdit->setText(duzenlenecekCariKart.getVerigino());
        ui->VergiDairesilineEdit->setText(duzenlenecekCariKart.getVergiDaire());
        ui->ilcomboBox->setCurrentIndex(ui->ilcomboBox->findText(duzenlenecekCariKart.getIl()));
        ui->ilcecomboBox->setCurrentIndex(ui->ilcecomboBox->findText(duzenlenecekCariKart.getIlce()));
        ui->AdresplainTextEdit->setPlainText(duzenlenecekCariKart.getAdres());
        ui->MaillineEdit->setText(duzenlenecekCariKart.getMail());
        ui->TelefonlineEdit->setText(duzenlenecekCariKart.getTelefon());
        ui->AciklamaplainTextEdit->setPlainText(duzenlenecekCariKart.getAciklama());
    }
}


