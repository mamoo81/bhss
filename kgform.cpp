#include "kgform.h"
#include "ui_kgform.h"

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
    girilenKilo = ui->doubleSpinBox->value();
    this->close();
}


