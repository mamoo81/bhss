#include "satisform.h"
#include "ui_satisform.h"
#include "user.h"
#include "stokfrom.h"


User *kullanici;

SatisForm::SatisForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SatisForm)
{
    ui->setupUi(this);

    formLoad();
}

SatisForm::~SatisForm()
{
    delete ui;
}

void SatisForm::setUser(User *user)
{
    kullanici = user;
    this->setWindowTitle("MHSS - " + kullanici->getAdi());
}

void SatisForm::formLoad()
{

}

void SatisForm::on_StokKartlariBtn_clicked()
{
    StokFrom *stokKartiFromu = new StokFrom(this);
    stokKartiFromu->exec();
}

