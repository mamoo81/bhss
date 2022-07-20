/*#####################################
MIT LICENCE                           #
#######################################
Copyright 2021 Mehmet AKDEMİR         #
bilgi@basat.dev                       #
#######################################
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
#include "stokgircikdialog.h"
#include "ui_stokgircikdialog.h"

StokGirCikDialog::StokGirCikDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StokGirCikDialog)
{
    ui->setupUi(this);
    ui->doubleSpinBox->selectAll();
    ui->doubleSpinBox->setFocus();
}

StokGirCikDialog::~StokGirCikDialog()
{
    delete ui;
}

void StokGirCikDialog::setIslem(const QString &newIslem)
{
    islem = newIslem;
    this->setWindowTitle("Stok " + islem + " Yap");
}

void StokGirCikDialog::on_pushButton_clicked()
{
    bool basarilimi = vt->setStokMiktari(kullanici, stokKartiID, islem, ui->doubleSpinBox->value());
    if(basarilimi){
        this->close();
    }
    else{
        ui->doubleSpinBox->selectAll();
        ui->doubleSpinBox->setFocus();
    }
}

void StokGirCikDialog::setKullanici(const User &newKullanici)
{
    kullanici = newKullanici;
}

void StokGirCikDialog::setStokKartiID(const QString &newStokKartiID)
{
    stokKartiID = newStokKartiID;
}


void StokGirCikDialog::on_iptalpushButton_clicked()
{
    this->close();
}
