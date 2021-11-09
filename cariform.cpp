#include "cariform.h"
#include "ui_cariform.h"

CariForm::CariForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CariForm)
{
    ui->setupUi(this);
}

CariForm::~CariForm()
{
    delete ui;
}
