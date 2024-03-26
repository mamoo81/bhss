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
#include "adetdialogform.h"
#include "ui_adetdialogform.h"
//*******************************
#include <QKeyEvent>
#include <QMessageBox>

AdetDialogForm::AdetDialogForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdetDialogForm)
{
    ui->setupUi(this);
    ui->spinBox->setFocus();
}

AdetDialogForm::~AdetDialogForm()
{
    delete ui;
}

void AdetDialogForm::on_pushButton_clicked()
{
    if(ui->spinBox->value() > 0){
        carpilacakAdet = ui->spinBox->value();
        this->close();
    }
    else{
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("0 dan büyük bir rakam giriniz.");
        msg.setStandardButtons(QMessageBox::Ok);
        // msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

void AdetDialogForm::keyPressEvent(QKeyEvent *keyEvent)
{
    if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return){
        AdetDialogForm::on_pushButton_clicked();
    }
    else if(keyEvent->key() == Qt::Key_Escape){
        this->close();
    }
}
