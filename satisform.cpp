#include "satisform.h"
#include "ui_satisform.h"

SatisForm::SatisForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SatisForm)
{
    ui->setupUi(this);
}

SatisForm::~SatisForm()
{
    delete ui;
}
