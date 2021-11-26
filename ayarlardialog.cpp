#include "ayarlardialog.h"
#include "ui_ayarlardialog.h"

AyarlarDialog::AyarlarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AyarlarDialog)
{
    ui->setupUi(this);
}

AyarlarDialog::~AyarlarDialog()
{
    delete ui;
}
