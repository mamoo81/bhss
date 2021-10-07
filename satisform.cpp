#include "satisform.h"
#include "ui_satisform.h"
#include "user.h"
#include "stokfrom.h"
#include "sepet.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QKeyEvent>
#include <QCloseEvent>

User *kullanici;
QSqlDatabase db_satis;
QSqlQuery sorgu_satis;

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
    db_satis.addDatabase("QPSQL", "db_satis");
    db_satis.setDatabaseName("mhss_data");
    db_satis.setHostName("localhost");
    db_satis.setUserName("admin");
    db_satis.setPassword("admin");
    if(!db_satis.isOpen())
    {
        db_satis.open();
    }

    initTableWidgets();


    ui->barkodLineEdit->setFocus();
}

void SatisForm::on_StokKartlariBtn_clicked()
{
    StokFrom *stokKartiFormu = new StokFrom(this);
    stokKartiFormu->exec();
}

void SatisForm::initTableWidgets()
{
    ui->SepetlertabWidget->setCurrentIndex(0);
    ui->HizliUrunlertabWidget->setCurrentIndex(0);
    ui->sepet1TableWidget->setColumnWidth(0,135);
    ui->sepet1TableWidget->setColumnWidth(1,200);
    ui->sepet1TableWidget->setColumnWidth(2,95);
    ui->sepet1TableWidget->setColumnWidth(3,88);
    ui->sepet1TableWidget->setColumnWidth(4,45);
    ui->sepet1TableWidget->setColumnWidth(5,100);
    ui->sepet2TableWidget->setColumnWidth(0,135);
    ui->sepet2TableWidget->setColumnWidth(1,200);
    ui->sepet2TableWidget->setColumnWidth(2,95);
    ui->sepet2TableWidget->setColumnWidth(3,88);
    ui->sepet2TableWidget->setColumnWidth(4,45);
    ui->sepet2TableWidget->setColumnWidth(5,100);
    ui->sepet3TableWidget->setColumnWidth(0,135);
    ui->sepet3TableWidget->setColumnWidth(1,200);
    ui->sepet3TableWidget->setColumnWidth(2,95);
    ui->sepet3TableWidget->setColumnWidth(3,88);
    ui->sepet3TableWidget->setColumnWidth(4,45);
    ui->sepet3TableWidget->setColumnWidth(5,100);
    ui->sepet4TableWidget->setColumnWidth(0,135);
    ui->sepet4TableWidget->setColumnWidth(1,200);
    ui->sepet4TableWidget->setColumnWidth(2,95);
    ui->sepet4TableWidget->setColumnWidth(3,88);
    ui->sepet4TableWidget->setColumnWidth(4,45);
    ui->sepet4TableWidget->setColumnWidth(5,100);
}


void SatisForm::on_SepetlertabWidget_currentChanged(int index)
{
    qDebug() << index;
}

void SatisForm::barkodBak(QString bakilacakBarkod)
{

}

