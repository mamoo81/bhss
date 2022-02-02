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
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Lütfen 0 'dan büyük bir değer giriniz.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

void KgForm::setBirimi(const QString &newBirimi)
{
    birimi = newBirimi;
    if(birimi == "METRE"){
        ui->label->setText("Metre giriniz");
    }
}


