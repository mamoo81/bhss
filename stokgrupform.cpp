#include "stokgrupform.h"
#include "ui_stokgrupform.h"

StokGrupForm::StokGrupForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StokGrupForm)
{
    ui->setupUi(this);
}

StokGrupForm::~StokGrupForm()
{
    delete ui;
}
