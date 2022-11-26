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

    ENTER = new QShortcut(this);
    ENTER->setKey(Qt::Key_Enter);
    connect(ENTER, SIGNAL(activated()), this, SLOT(on_pushButton_clicked()));

    RETURN = new QShortcut(this);
    RETURN->setKey(Qt::Key_Return);
    connect(RETURN, SIGNAL(activated()), this, SLOT(on_pushButton_clicked()));
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
    if(ui->doubleSpinBox->value() > 0){
        miktar = ui->doubleSpinBox->value();
    }
    else{
        miktar = 0;
        ui->doubleSpinBox->selectAll();
        ui->doubleSpinBox->setFocus();
    }
    this->close();
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
    miktar = 0;
    this->close();
}

const User &StokGirCikDialog::getKullanici() const
{
    return kullanici;
}

const QString &StokGirCikDialog::getStokKartiID() const
{
    return stokKartiID;
}

const QString &StokGirCikDialog::getIslem() const
{
    return islem;
}

float StokGirCikDialog::getMiktar() const
{
    return miktar;
}
