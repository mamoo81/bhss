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
        User u = vt.GetUserInfos(newDuzenlenecekUserName);
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
    if(yeniMi){
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
        vt.CreateNewUser(NewUser);
        this->close();
    }
}


void KullaniciDialogForm::on_pushButton_2_clicked()
{
    this->close();
}

