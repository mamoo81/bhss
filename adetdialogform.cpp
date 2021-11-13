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
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

void AdetDialogForm::keyPressEvent(QKeyEvent *keyEvent)
{
    if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return){
        AdetDialogForm::on_pushButton_clicked();
    }
}
