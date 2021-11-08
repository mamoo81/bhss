#include "hizliurunekleformdialog.h"
#include "ui_hizliurunekleformdialog.h"
#include "veritabani.h"
//******************************
#include <QSqlQuery>
#include <QDebug>

HizliUrunEkleFormDialog::HizliUrunEkleFormDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HizliUrunEkleFormDialog)
{
    ui->setupUi(this);
    stokKartlariniListele();
}

HizliUrunEkleFormDialog::~HizliUrunEkleFormDialog()
{
    delete ui;
}


void HizliUrunEkleFormDialog::stokKartlariniListele()
{
    QString sorgu("SELECT barkod, ad FROM stokkartlari ORDER BY ad ASC");
    Veritabani *vt = new Veritabani();
    ui->StokKartlaritableView->setModel(vt->getStokKartlari(sorgu));
    QItemSelectionModel *selectionModel = ui->StokKartlaritableView->selectionModel();
    QModelIndex modelindex = ui->StokKartlaritableView->model()->index(0, 0);
    selectionModel->select(modelindex, QItemSelectionModel::Clear);
    ui->StokKartlaritableView->setColumnWidth(0, 140);
    ui->StokKartlaritableView->setColumnWidth(1, 345);
    delete vt;
}

void HizliUrunEkleFormDialog::on_KaydetpushButton_clicked()
{
    hizliUrunBarkod = ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 0).data().toString();
    hizliUrunAd = ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString();
    ok = true;
    this->close();
}


void HizliUrunEkleFormDialog::on_IptalpushButton_clicked()
{
    ok = false;
    this->close();
}

