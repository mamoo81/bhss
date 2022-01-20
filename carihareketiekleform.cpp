#include "carihareketiekleform.h"
#include "ui_carihareketiekleform.h"

CariHareketiEkleForm::CariHareketiEkleForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CariHareketiEkleForm)
{
    ui->setupUi(this);

    FormLoad();
}

CariHareketiEkleForm::~CariHareketiEkleForm()
{
    delete ui;
}


void CariHareketiEkleForm::FormLoad()
{
    ui->islemdateEdit->setDate(QDate::currentDate());
    ui->OdemeTipcomboBox->addItems(vt->getOdemeTipleri());
}
