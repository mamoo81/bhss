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
#include "birimekleform.h"
#include "ui_birimekleform.h"
//****************************
#include <QMessageBox>
#include <QList>
#include <QListWidgetItem>
#include <QDebug>

BirimekleForm::BirimekleForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BirimekleForm)
{
    ui->setupUi(this);

    FormLoad();
}

BirimekleForm::~BirimekleForm()
{
    delete ui;
}

void BirimekleForm::FormLoad()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(stokYonetimi.getStokBirimleri());
}

void BirimekleForm::on_eklepushButton_clicked()
{
    QList<QListWidgetItem *> aranacak = ui->listWidget->findItems(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->lineEdit->text()), Qt::MatchExactly);
    if(aranacak.size() > 0){
         QMessageBox msg(this);
         msg.setWindowTitle("Dikkat");
         msg.setIcon(QMessageBox::Warning);
         msg.setText("Girdiğiniz birim zaten mevcut");
         msg.setStandardButtons(QMessageBox::Ok);
         msg.setButtonText(QMessageBox::Ok, "Tamam");
         msg.exec();
    }
    else{
        stokYonetimi.stokBirimEkle(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->lineEdit->text()));
        ui->listWidget->clear();
        ui->listWidget->addItems(stokYonetimi.getStokBirimleri());
    }
}


void BirimekleForm::on_silpushButton_clicked()
{
    if(ui->listWidget->currentRow() != 0){
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Question);
        msg.setText("Silmek istediğinize emin misiniz?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        msg.setDefaultButton(QMessageBox::Yes);
        msg.exec();
        if(QMessageBox::Yes == msg.result())
        {
            stokYonetimi.stokBirimSil(ui->listWidget->currentItem()->text());
            delete ui->listWidget->currentItem();
        }
    }
}

