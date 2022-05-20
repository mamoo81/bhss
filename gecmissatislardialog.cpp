#include "gecmissatislardialog.h"
#include "ui_gecmissatislardialog.h"
#include "satisgosterdialog.h"

GecmisSatislarDialog::GecmisSatislarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GecmisSatislarDialog)
{
    ui->setupUi(this);

    FormLoad();
}

GecmisSatislarDialog::~GecmisSatislarDialog()
{
    delete ui;
}

void GecmisSatislarDialog::FormLoad()
{
    ui->SonSatislarlistWidget->clear();
    ui->SonSatislarlistWidget->addItems(vt.getSonIslemler());
    ui->IslemSayisilabel->setText(QString::number(ui->SonSatislarlistWidget->count()));
}

void GecmisSatislarDialog::on_SonSatislarlistWidget_itemDoubleClicked(QListWidgetItem *item)
{
    SatisGosterDialog *satisGosterForm = new SatisGosterDialog(this);
    satisGosterForm->setSatisFaturaNo(item->text());
    satisGosterForm->sepetiCek();
    satisGosterForm->exec();
    delete satisGosterForm;
}

