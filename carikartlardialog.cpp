#include "carikartlardialog.h"
#include "ui_carikartlardialog.h"

CariKartlarDialog::CariKartlarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CariKartlarDialog)
{
    ui->setupUi(this);
}

CariKartlarDialog::~CariKartlarDialog()
{
    delete ui;
}
