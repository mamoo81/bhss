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
#include "stokgrupform.h"
#include "ui_stokgrupform.h"
//*****************************
#include <QMessageBox>
#include <QDebug>
#include <QLocale>

StokGrupForm::StokGrupForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StokGrupForm)
{
    ui->setupUi(this);

    formLoad();
}


StokGrupForm::~StokGrupForm()
{
    delete ui;
}

void StokGrupForm::formLoad()
{
    stokGruplariGetir();
    ui->YenilineEdit->setFocus();
}

void StokGrupForm::on_EkleBtn_clicked()
{
    if(!ui->YenilineEdit->text().isEmpty())
    {
        QString aranacak = ui->YenilineEdit->text().toUpper();
        QList<QListWidgetItem *> eslesme = ui->gruplarlistWidget->findItems(aranacak,Qt::MatchExactly);
        if(eslesme.size() > 0)
        {
            QMessageBox msg(this);
            msg.setWindowTitle("Hata");
            msg.setIcon(QMessageBox::Critical);
            msg.setText("Bu stok gurubu zaten mevcut.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        else
        {
            stokYonetimi.stokGrupEkle(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->YenilineEdit->text()));
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Stok gurubu başarıyla eklendi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            ui->YenilineEdit->clear();
            stokGruplariGetir();
            ui->YenilineEdit->setFocus();
        }
    }
}


void StokGrupForm::on_SilBtn_clicked()
{
    if(ui->gruplarlistWidget->selectedItems().size() != 0 )
    {
        if(ui->gruplarlistWidget->currentRow() != 0){
            stokYonetimi.stokGrupSil(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->gruplarlistWidget->currentItem()->text()));
            QListWidgetItem *silinecek_item = ui->gruplarlistWidget->takeItem(ui->gruplarlistWidget->currentRow());
            delete silinecek_item;
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Stok gurubu silindi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            stokGruplariGetir();
        }
    }
    else
    {
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Listeden silinecek gurubu seçin.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

void StokGrupForm::on_KaydetBtn_clicked()
{
    this->close();
}

void StokGrupForm::stokGruplariGetir()
{
    ui->gruplarlistWidget->model()->removeRows(0, ui->gruplarlistWidget->count());
    QStringList liste = stokYonetimi.stokGruplariGetir();
    foreach (auto item, liste) {
        ui->gruplarlistWidget->addItem(item);
    }
}

void StokGrupForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
}

