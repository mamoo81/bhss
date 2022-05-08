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
    ui->lineEdit->setFocus();
}

HizliUrunEkleFormDialog::~HizliUrunEkleFormDialog()
{
    delete ui;
}


void HizliUrunEkleFormDialog::stokKartlariniListele()
{
    QString sorgu("SELECT kod, barkod, ad, sfiyat FROM stokkartlari ORDER BY ad ASC");
    Veritabani *vt = new Veritabani();
    ui->StokKartlaritableView->setModel(vt->getStokKartlari(sorgu));
    ui->StokKartlaritableView->resizeColumnsToContents();
    QItemSelectionModel *selectionModel = ui->StokKartlaritableView->selectionModel();
    QModelIndex modelindex = ui->StokKartlaritableView->model()->index(0, 0);
    selectionModel->select(modelindex, QItemSelectionModel::Clear);
//    ui->StokKartlaritableView->setColumnWidth(0, 75);
//    ui->StokKartlaritableView->setColumnWidth(1, 140);
//    ui->StokKartlaritableView->setColumnWidth(2, 345);
    delete vt;
}

void HizliUrunEkleFormDialog::on_KaydetpushButton_clicked()
{
    hizliUrunStokID = ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 0).data().toString();
    hizliUrunBarkod = ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString();
    hizliUrunAd = ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 2).data().toString();
    ok = true;
    this->close();
}


void HizliUrunEkleFormDialog::on_IptalpushButton_clicked()
{
    ok = false;
    this->close();
}


void HizliUrunEkleFormDialog::on_StokKartlaritableView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    hizliUrunStokID = ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 0).data().toString();
    hizliUrunBarkod = ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString();
    hizliUrunAd = ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 2).data().toString();
    ok = true;
    this->close();
}


void HizliUrunEkleFormDialog::on_lineEdit_textChanged(const QString &arg1)
{
    for (int i = 0; i < ui->StokKartlaritableView->model()->rowCount(); ++i) {
        if(!ui->StokKartlaritableView->model()->index(i, 2).data().toString().contains(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(arg1), Qt::CaseInsensitive)){
            ui->StokKartlaritableView->hideRow(i);
        }
        else{
            ui->StokKartlaritableView->showRow(i);
        }
    }
}

