#include "hizliurunekleformdialog.h"
#include "ui_hizliurunekleformdialog.h"
#include "veritabani.h"
//******************************
#include <QSqlQuery>
#include <QDebug>
#include <QKeyEvent>

HizliUrunEkleFormDialog::HizliUrunEkleFormDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HizliUrunEkleFormDialog)
{
    ui->setupUi(this);

    key_Down = new QShortcut(this);
    key_Down->setKey(Qt::Key_Down);
    connect(key_Down, SIGNAL(activated()), this, SLOT(key_Down_Slot()));

    key_DownArrow = new QShortcut(this);
    key_DownArrow->setKey(Qt::DownArrow);
    connect(key_DownArrow, SIGNAL(activated()), this, SLOT(key_Down_Slot()));

    key_Up = new QShortcut(this);
    key_Up->setKey(Qt::Key_Up);
    connect(key_Up, SIGNAL(activated()), this, SLOT(key_UP_Slot()));

    key_UpArrow = new QShortcut(this);
    key_UpArrow->setKey(Qt::UpArrow);
    connect(key_UpArrow, SIGNAL(activated()), this, SLOT(key_UP_Slot()));

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
    delete vt;
    // aşağı/yukarı yön tuşları ile seçim yapabilsin diye gosterilenSatirlar listesine indexleri atıyorum.
    // arama sonrası aşağı/yukarı tabloda gizlenen satırları da seçiyordu. satır gizli olduğu için seçim gözükmüyordu. ilk form yüklendiğinde de seçim yapabilmek için aşağıda ki foru kullandım.
    for (int var = 0; var < ui->StokKartlaritableView->model()->rowCount(); ++var) {
        gosterilenSatirlar.append(var);
    }
    ui->StokKartlaritableView->selectRow(0);
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
    gosterilenSatirlar.clear();
    ui->StokKartlaritableView->clearSelection();

    for (int i = 0; i < ui->StokKartlaritableView->model()->rowCount(); ++i) {
        if(!ui->StokKartlaritableView->model()->index(i, 2).data().toString().contains(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(arg1), Qt::CaseInsensitive)){
            ui->StokKartlaritableView->hideRow(i);
        }
        else{
            ui->StokKartlaritableView->showRow(i);
            gosterilenSatirlar.append(i);// aşağı/yukarı yön tuşları ile seçim yaparken gizlenen satırları es geçmek için gösterilen satırların indexini atıyorum.
        }
    }

    if(gosterilenSatirlar.count() > 0){// aranan hiç bir satırda yoksa olmayan bir satırı seçmesin diye.
        ui->StokKartlaritableView->selectRow(gosterilenSatirlar.first());// gösterilen satırlardan ilk indexi seçiyorum.
    }

    if(arg1.count() == 0){// arama metni uzunluğu 0 ise satır seçimini ve son gösterilenindexi 0 yap
        ui->StokKartlaritableView->selectRow(0);
        sonSecilenGosterilenSatirIndexi = 0;
    }

    ui->lineEdit->setFocus();
}

void HizliUrunEkleFormDialog::key_Down_Slot()
{
    // gizlenen satırları seçim yapmasın diye satırı bu şekilde seçtiriyorum
    if(sonSecilenGosterilenSatirIndexi < (gosterilenSatirlar.count() - 1)){
        ui->StokKartlaritableView->selectRow(gosterilenSatirlar.at(sonSecilenGosterilenSatirIndexi + 1));
        sonSecilenGosterilenSatirIndexi++;
    }
    ui->lineEdit->setFocus();
}

void HizliUrunEkleFormDialog::key_UP_Slot()
{
    if(sonSecilenGosterilenSatirIndexi > 0){
        ui->StokKartlaritableView->selectRow(gosterilenSatirlar.at(sonSecilenGosterilenSatirIndexi - 1));
        sonSecilenGosterilenSatirIndexi--;
    }
    ui->lineEdit->setFocus();
}


void HizliUrunEkleFormDialog::on_StokKartlaritableView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    ui->lineEdit->setFocus();
}

