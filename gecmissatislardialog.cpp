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
#include "gecmissatislardialog.h"
#include "ui_gecmissatislardialog.h"
#include "satisgosterdialog.h"

GecmisSatislarDialog::GecmisSatislarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GecmisSatislarDialog)
{
    ui->setupUi(this);

    FormLoad();
}

GecmisSatislarDialog::~GecmisSatislarDialog()
{
    delete ui;
}

void GecmisSatislarDialog::FormLoad()
{
    ui->SonSatislarlistWidget->clear();
    ui->SonSatislarlistWidget->addItems(vt.getSonIslemler());
    ui->IslemSayisilabel->setText(QString::number(ui->SonSatislarlistWidget->count()));

    iadeleriRenklendir();
}

void GecmisSatislarDialog::iadeleriRenklendir()
{
    for (int var = 0; var < ui->SonSatislarlistWidget->count(); ++var) {
        if(faturaYonetimi.iadeAlinmismi(ui->SonSatislarlistWidget->item(var)->text().split(' ').first())){
            ui->SonSatislarlistWidget->item(var)->setBackgroundColor(Qt::yellow);
        }
    }
}

void GecmisSatislarDialog::on_SonSatislarlistWidget_itemDoubleClicked(QListWidgetItem *item)
{
    SatisGosterDialog *satisGosterForm = new SatisGosterDialog(this);
    satisGosterForm->setSatisFaturaNo(item->text());
    satisGosterForm->sepetiCek();
    satisGosterForm->setKullanici(kullanici);
    satisGosterForm->exec();
    delete satisGosterForm;

    iadeleriRenklendir();
}


void GecmisSatislarDialog::on_KapatpushButton_clicked()
{
    this->close();
}

void GecmisSatislarDialog::setKullanici(const User &value)
{
    kullanici = value;
}
