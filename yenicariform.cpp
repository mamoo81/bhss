#include "yenicariform.h"
#include "ui_yenicariform.h"

YeniCariForm::YeniCariForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YeniCariForm)
{
    ui->setupUi(this);
}

YeniCariForm::~YeniCariForm()
{
    delete ui;
}
