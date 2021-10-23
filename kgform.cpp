#include "kgform.h"
#include "ui_kgform.h"
//**********************
#include <QMessageBox>

KgForm::KgForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KgForm)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::Dialog);
    ui->doubleSpinBox->setFocus();
}

KgForm::~KgForm()
{
    delete ui;
}

float KgForm::getGirilenKg()
{
    return girilenKilo;
}

void KgForm::on_pushButton_clicked()
{
    if(ui->doubleSpinBox->value() > 0){
        girilenKilo = ui->doubleSpinBox->value();
        this->close();
    }
    else{
        QMessageBox msg(0);
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Uyarı");
        msg.setInformativeText("Lütfen 0 'dan büyük bir değer giriniz.");
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();

    }
}


