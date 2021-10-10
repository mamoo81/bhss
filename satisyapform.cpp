#include "satisyapform.h"
#include "ui_satisyapform.h"

SatisYapForm::SatisYapForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SatisYapForm)
{
    ui->setupUi(this);
}

SatisYapForm::~SatisYapForm()
{
    delete ui;
}
