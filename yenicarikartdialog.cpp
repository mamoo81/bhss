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
    ui->CariTipicomboBox->addItems(cariYonetimi.getCariTipleri());
    // vergi dairelerni veritabanından alıp ilgili objeye yazarken otomatik doldurma için.
    QCompleter *tamamlayici = new QCompleter(cariYonetimi.getVergiDaireleri(), this);
    tamamlayici->setCompletionMode(QCompleter::PopupCompletion);
    tamamlayici->setCaseSensitivity(Qt::CaseInsensitive);
    ui->VergiDairesilineEdit->setCompleter(tamamlayici);
    //
    ui->ilcomboBox->addItems(vt->getIller());
    ui->CariTipicomboBox->setFocus();

    LineEditBackColorPaletteDefault = ui->CariAdilineEdit->style()->standardPalette();
    LineEditBackColorPaletteRed = QPalette();
    LineEditBackColorPaletteRed.setColor(QPalette::Base, Qt::red);
    LineEditBackColorPaletteRed.setColor(QPalette::Text, Qt::black);

    RegEXPcariAdi = QRegExp("[a-zöçşıiğü A-ZÖÇŞIİĞÜ 0-9]{5,}");
    ui->CariAdilineEdit->setValidator(new QRegExpValidator(RegEXPcariAdi, this));
    ui->YetkililineEdit->setValidator(new QRegExpValidator(RegEXPcariAdi, this));

    RegEXPVergiNo = QRegExp("[0-9]{11}");
    ui->VergiNolineEdit->setValidator(new QRegExpValidator(RegEXPVergiNo, this));

    RegEXPmail = QRegExp("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    RegEXPmail.setCaseSensitivity(Qt::CaseInsensitive);
    ui->MaillineEdit->setValidator(new QRegExpValidator(RegEXPmail, this));

    RegEXPtelefon = QRegExp("[0-9]{11}");
    ui->TelefonlineEdit->setValidator(new QRegExpValidator(RegEXPtelefon, this));
}

void YeniCariKartDialog::on_iptalpushButton_clicked()
{
    this->close();
}


void YeniCariKartDialog::on_ilcomboBox_currentIndexChanged(int index)
{
    ui->ilcecomboBox->clear();
    ui->ilcecomboBox->addItems(vt->getIlceler(index));

    if(index <= 0){
        ui->ilcomboBox->setPalette(LineEditBackColorPaletteRed);
    }
    else{
        ui->ilcomboBox->setPalette(LineEditBackColorPaletteDefault);
    }
}


void YeniCariKartDialog::on_KaydetpushButton_clicked()
{
    QMessageBox msg(this);
    msg.setWindowTitle("Uyarı");
    msg.setIcon(QMessageBox::Warning);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, "Tamam");

    if(ui->CariAdilineEdit->text().isEmpty()){
        uyariSes->play();
        msg.setText("Cari adını giriniz.");
        msg.exec();
        return;
    }
    if(!RegEXPcariAdi.exactMatch(ui->CariAdilineEdit->text())){
        uyariSes->play();
        msg.setText("Cari adı uygun formatta olmalı");
        msg.exec();
        return;
    }
    if(!ui->YetkililineEdit->text().isEmpty()){
        if(!RegEXPcariAdi.exactMatch(ui->YetkililineEdit->text())){
            uyariSes->play();
            msg.setText("Cari yetkili adı uygun formatta olmalı");
            msg.exec();
            return;
        }
    }
    if(!ui->VergiNolineEdit->text().isEmpty()){
        if(!RegEXPVergiNo.exactMatch(ui->VergiNolineEdit->text())){
            uyariSes->play();
            msg.setText("Cari vergi numarası uygun formatta olmalı");
            msg.exec();
            return;
        }
    }
    if(!ui->VergiNolineEdit->text().isEmpty()){
        if(!RegEXPcariAdi.exactMatch(ui->VergiDairesilineEdit->text())){
            uyariSes->play();
            msg.setText("Cari vergi dairesi adı uygun formatta olmalı");
            msg.exec();
            return;
        }
    }
    if(!ui->MaillineEdit->text().isEmpty()){
        if(!RegEXPmail.exactMatch(ui->MaillineEdit->text())){
            uyariSes->play();
            msg.setText("Cari mail adresi uygun formatta olmalı");
            msg.exec();
            return;
        }
    }
    if(!ui->TelefonlineEdit->text().isEmpty()){
        if(!RegEXPtelefon.exactMatch(ui->TelefonlineEdit->text())){
            uyariSes->play();
            msg.setText("Cari telefon numarası uygun formatta olmalı");
            msg.exec();
            return;
        }
    }
    if(ui->ilcomboBox->currentIndex() <= 0){
        uyariSes->play();
        msg.setText("Cari ilini seçin");
        msg.exec();
        return;
    }
    if(ui->ilcecomboBox->currentIndex() <= 0){
        uyariSes->play();
        msg.setText("Cari ilçesini seçin");
        msg.exec();
        return;
    }

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
        duzenlenecekCariKart.setGuncelBorcHesaplama(ui->guncelFiyatcheckBox->isChecked());
        bool sonuc = cariYonetimi.cariKartDuzenle(duzenlenecekCariKart);
        if(sonuc){
            uyariSes->play();
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
            uyariSes->play();
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
        yeniCari.setGuncelBorcHesaplama(ui->guncelFiyatcheckBox->isChecked());

        if(cariYonetimi.yeniCariKart(yeniCari)){
            uyariSes->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Yeni cari kart oluşturuldu.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        else{
            uyariSes->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Yeni cari kart oluşturulamadı!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
    this->close();
}

void YeniCariKartDialog::setDuzenlenecekCariID(const QString &newDuzenlenecekCariID)
{
    duzenlenecekCariID = newDuzenlenecekCariID;
    if(duzenle){
        duzenlenecekCariKart = cariYonetimi.getCariKart(duzenlenecekCariID);
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
        if(duzenlenecekCariKart.getGuncelBorcHesaplama()){
            ui->guncelFiyatcheckBox->setChecked(true);
        }
        else{
            ui->guncelFiyatcheckBox->setChecked(false);
        }
    }
}


void YeniCariKartDialog::on_CariAdilineEdit_textChanged(const QString &arg1)
{
    if(RegEXPcariAdi.exactMatch(arg1)){
        ui->CariAdilineEdit->setPalette(LineEditBackColorPaletteDefault);
    }
    else{
        ui->CariAdilineEdit->setPalette(LineEditBackColorPaletteRed);
    }
}

void YeniCariKartDialog::on_YetkililineEdit_textChanged(const QString &arg1)
{
    if(RegEXPcariAdi.exactMatch(arg1)){
        ui->YetkililineEdit->setPalette(LineEditBackColorPaletteDefault);
    }
    else{
        ui->YetkililineEdit->setPalette(LineEditBackColorPaletteRed);
    }
}

void YeniCariKartDialog::on_VergiNolineEdit_textChanged(const QString &arg1)
{
    if(RegEXPVergiNo.exactMatch(arg1)){
        ui->VergiNolineEdit->setPalette(LineEditBackColorPaletteDefault);
    }
    else{
        ui->VergiNolineEdit->setPalette(LineEditBackColorPaletteRed);
    }
}

void YeniCariKartDialog::on_MaillineEdit_textChanged(const QString &arg1)
{
    if(RegEXPmail.exactMatch(arg1)){
        ui->MaillineEdit->setPalette(LineEditBackColorPaletteDefault);
    }
    else{
        ui->MaillineEdit->setPalette(LineEditBackColorPaletteRed);
    }
}

void YeniCariKartDialog::on_TelefonlineEdit_textChanged(const QString &arg1)
{
    if(RegEXPtelefon.exactMatch(arg1)){
        ui->TelefonlineEdit->setPalette(LineEditBackColorPaletteDefault);
    }
    else{
        ui->TelefonlineEdit->setPalette(LineEditBackColorPaletteRed);
    }
}

void YeniCariKartDialog::on_ilcecomboBox_currentIndexChanged(int index)
{
    if(index <= 0){
        ui->ilcecomboBox->setPalette(LineEditBackColorPaletteRed);
    }
    else{
        ui->ilcecomboBox->setPalette(LineEditBackColorPaletteDefault);
    }
}
