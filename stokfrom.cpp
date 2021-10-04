#include "stokfrom.h"
#include "ui_stokfrom.h"

StokFrom::StokFrom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StokFrom)
{
    ui->setupUi(this);
}

StokFrom::~StokFrom()
{
    delete ui;
}
