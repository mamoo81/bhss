#include "satisyapform.h"
#include "ui_satisyapform.h"
#include "satisform.h"

#include <QString>
#include <QCompleter>
#include <QDebug>

SatisYapForm::SatisYapForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SatisYapForm)
{
    ui->setupUi(this);

    formLoad();

}

SatisYapForm::~SatisYapForm()
{
    delete ui;
}

void SatisYapForm::formLoad()
{
    ui->toplamLBL->setText(QString::number(satilacakSepet.sepetToplamTutari()));
}


void SatisYapForm::on_satBtn_clicked()
{

}

