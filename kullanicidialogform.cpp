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
#include "kullanicidialogform.h"
#include "ui_kullanicidialogform.h"

#include <QDebug>

KullaniciDialogForm::KullaniciDialogForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KullaniciDialogForm)
{
    ui->setupUi(this);
    formLoad();
}

KullaniciDialogForm::~KullaniciDialogForm()
{
    delete ui;
}

void KullaniciDialogForm::formLoad()
{
    regEXPTelefon = QRegExp("[0-9]\\d{10}");
    regEXPTelefon.setCaseSensitivity(Qt::CaseInsensitive);
    regEXPTelefon.setPatternSyntax(QRegExp::RegExp);
    ui->CepNolineEdit->setValidator(new QRegExpValidator(regEXPTelefon, this));

    regEXPkullaniciAdi = QRegExp("[a-zöçşiğüA-ZÖÇŞİĞÜ0-9.]{5,16}");
    regEXPkullaniciAdi.setCaseSensitivity(Qt::CaseInsensitive);
    ui->UserNamelineEdit->setValidator(new QRegExpValidator(regEXPkullaniciAdi, this));

    regEXPpassword = QRegExp("[a-zöçşiğüA-ZÖÇŞİĞÜ0-9.]{4,16}");
    regEXPpassword.setCaseSensitivity(Qt::CaseInsensitive);
    ui->PasswordlineEdit->setValidator(new QRegExpValidator(regEXPpassword, this));
    ui->PasswordlineEdit_2->setValidator(new QRegExpValidator(regEXPpassword, this));

    if(yeniMi){
        this->setWindowTitle("Yeni kullanıcı oluştur");
    }
    else{
        this->setWindowTitle("Kullanıcıyı düzenle");
    }
    ui->UserNamelineEdit->setFocus();
}

void KullaniciDialogForm::setDuzenlenecekUser(const QString &newDuzenlenecekUserName)
{
    if(!yeniMi){
        u = vt.GetUserInfos(newDuzenlenecekUserName);
        ui->UserNamelineEdit->setText(u.getUserName());
        ui->PasswordlineEdit->setText(u.getPassWord());
        ui->AdlineEdit->setText(u.getAd());
        ui->SoyadlineEdit->setText(u.getSoyad());
        ui->CepNolineEdit->setText(u.getCepNo());
        ui->KasacheckBox->setChecked(u.getKasaYetki());
        ui->IadecheckBox->setChecked(u.getIadeYetki());
        ui->StokcheckBox->setChecked(u.getStokYetki());
        ui->CaricheckBox->setChecked(u.getCariyetki());
        ui->AyarcheckBox->setChecked(u.getAyaryetki());
    }
    duzenlenecekUserName = newDuzenlenecekUserName;
}

void KullaniciDialogForm::setYeniMi(bool newYeniMi)
{
    yeniMi = newYeniMi;
}

void KullaniciDialogForm::on_pushButton_clicked()
{
    QMessageBox msg(this);
    msg.setWindowTitle("Uyarı");
    msg.setIcon(QMessageBox::Warning);
    msg.setText("Tüm alanları doldurunuz veya Kırmızı renkli alanları kontrol edin!");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, "Tamam");

    if(!regEXPkullaniciAdi.exactMatch(ui->UserNamelineEdit->text())){
        uyariSesi->play();
        msg.exec();
        return;
    }
    if(!regEXPpassword.exactMatch(ui->PasswordlineEdit->text()) && !regEXPpassword.exactMatch(ui->PasswordlineEdit_2->text())){
        uyariSesi->play();
        msg.exec();
        return;
    }
    if(!regEXPTelefon.exactMatch(ui->CepNolineEdit->text())){
        uyariSesi->play();
        msg.exec();
        return;
    }
    if(ui->AdlineEdit->text().isEmpty()){
        uyariSesi->play();
        msg.exec();
        return;
    }
    if(ui->SoyadlineEdit->text().isEmpty()){
        uyariSesi->play();
        msg.exec();
        return;
    }

    User NewUser;
    NewUser.setUserName(ui->UserNamelineEdit->text());
    NewUser.setPassWord(ui->PasswordlineEdit->text());
    NewUser.setAd(ui->AdlineEdit->text());
    NewUser.setSoyad(ui->SoyadlineEdit->text());
    NewUser.setCepNo(ui->CepNolineEdit->text());
    NewUser.setTarih(QDateTime::currentDateTime());
    NewUser.setKasaYetki(ui->KasacheckBox->isChecked());
    NewUser.setIadeYetki(ui->IadecheckBox->isChecked());
    NewUser.setStokYetki(ui->StokcheckBox->isChecked());
    NewUser.setCariyetki(ui->CaricheckBox->isChecked());
    NewUser.setCariyetki(ui->CaricheckBox->isChecked());
    NewUser.setAyaryetki(ui->AyarcheckBox->isChecked());

    if(yeniMi){
        if(vt.CreateNewUser(NewUser)){
            uyariSesi->play();
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Information);
            msg.setWindowTitle("Başarılı");
            msg.setText("Yeni kullanıcı oluşturuldu.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            this->close();
        }
        else{
            uyariSesi->play();
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Information);
            msg.setWindowTitle("Hata");
            msg.setText("HATA:\n\nYeni kullanıcı oluşturulamadı!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
    else{
        NewUser.setUserID(u.getUserID());
        if(vt.updateUser(NewUser)){
            uyariSesi->play();
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Information);
            msg.setWindowTitle("Başarılı");
            msg.setText("Kullanıcı güncellendi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
            this->close();
        }
        else{
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Information);
            msg.setWindowTitle("Hata");
            msg.setText("Güncelleme Başarısız!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }
    }
}


void KullaniciDialogForm::on_pushButton_2_clicked()
{
    this->close();
}


void KullaniciDialogForm::on_UserNamelineEdit_textChanged(const QString &arg1)
{
    if(regEXPkullaniciAdi.exactMatch(arg1)){
        ui->UserNamelineEdit->setStyleSheet("color: black;");
    }
    else{
        ui->UserNamelineEdit->setStyleSheet("color: red;");
    }
}

void KullaniciDialogForm::on_PasswordlineEdit_textChanged(const QString &arg1)
{
    if(regEXPpassword.exactMatch(arg1)){
        ui->PasswordlineEdit->setStyleSheet("color: black;");
    }
    else{
        ui->PasswordlineEdit->setStyleSheet("color: red;");
    }
}

void KullaniciDialogForm::on_PasswordlineEdit_2_textChanged(const QString &arg1)
{
    if(regEXPpassword.exactMatch(arg1)){
        ui->PasswordlineEdit_2->setStyleSheet("color: black;");
    }
    else{
        ui->PasswordlineEdit_2->setStyleSheet("color: red;");
    }

    if(ui->PasswordlineEdit->text() == ui->PasswordlineEdit_2->text()){
        ui->PasswordlineEdit->setStyleSheet("color: black;");
        ui->PasswordlineEdit_2->setStyleSheet("color: black;");
    }
    else{
        ui->PasswordlineEdit->setStyleSheet("color: red;");
        ui->PasswordlineEdit_2->setStyleSheet("color: red;");
    }
}

void KullaniciDialogForm::on_CepNolineEdit_textChanged(const QString &arg1)
{
    if(regEXPTelefon.exactMatch(arg1)){
        ui->CepNolineEdit->setStyleSheet("color: black;");
    }
    else{
        ui->CepNolineEdit->setStyleSheet("color: red;");
    }
}
