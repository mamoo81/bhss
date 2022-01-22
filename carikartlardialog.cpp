#include "carikartlardialog.h"
#include "ui_carikartlardialog.h"
#include "carihareketiekleform.h"
//**************************
#include <QCompleter>
#include <QLocale>
#include <QDateTime>
#include <QDebug>

QLocale turkce(QLocale::Turkish, QLocale::Turkey);

CariKartlarDialog::CariKartlarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CariKartlarDialog)
{
    ui->setupUi(this);

    formLoad();
}

CariKartlarDialog::~CariKartlarDialog()
{
    delete ui;
}

void CariKartlarDialog::setKullanici(User newKullanici)
{
    kullanici = newKullanici;
}


void CariKartlarDialog::formLoad()
{
    setVergiDaireleri(vt.getVergiDaireleri());

    ui->CariKartlartableView->setModel(vt.getCariKartIsimleri());
    ui->CariKartlartableView->resizeColumnsToContents();
    connect(ui->CariKartlartableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(cariHareketleriListele()));
    ui->CariKartlartableView->selectRow(0);
    //tarihleri ayarlama
    ui->bitisdateEdit->setDateTime(QDateTime::currentDateTime());
    connect(ui->CariKartHareketleritableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(cariHareketleriTableSelectionChanged()));
}

void CariKartlarDialog::cariHareketleriListele()
{
    //cari seçilince yapılacaklar
    QString cariID = ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString();
    ui->CariKartHareketleritableView->setModel(vt.getCariHareketleri(cariID));
    ui->CariKartHareketleritableView->resizeColumnsToContents();
    ui->cariToplamAlacaklabel->setText(QString::number(vt.getCariToplamAlacak(cariID), 'f', 2));
    ui->cariToplamBorclabel->setText(QString::number(vt.getCariToplamBorc(cariID), 'f', 2));

    // butonların aktif/pasif durumları ayarlama
    if(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString() != "1000"){//direkt carisi ise es geç
        ui->CaridenTahsilatYaptoolButton->setEnabled(true);
        ui->CariyeOdemeYaptoolButton->setEnabled(true);
        ui->CariyiBorclandirtoolButton->setEnabled(true);
        ui->CariyiAlacaklandirtoolButton->setEnabled(true);
        ui->CariBankatoolButton->setEnabled(true);
        ui->CariyeStokluSatistoolButton->setEnabled(true);
        ui->CariyeStoksuzSatistoolButton->setEnabled(true);
    }
    else{
        ui->CaridenTahsilatYaptoolButton->setEnabled(false);
        ui->CariyeOdemeYaptoolButton->setEnabled(false);
        ui->CariyiBorclandirtoolButton->setEnabled(false);
        ui->CariyiAlacaklandirtoolButton->setEnabled(false);
        ui->CariBankatoolButton->setEnabled(false);
        ui->CariyeStokluSatistoolButton->setEnabled(false);
        ui->CariyeStoksuzSatistoolButton->setEnabled(false);
    }
    // cari seçimi değişince ödeme ve tahsilat makbuzları butonlarını pasif yapma
    ui->OdemeMakbuzuBastoolButton->setEnabled(false);
    ui->TahsilatMakbuzuBastoolButton->setEnabled(false);
}

void CariKartlarDialog::cariHareketleriTableSelectionChanged()
{
    if(ui->CariKartHareketleritableView->currentIndex().row() > -1){
        if(ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 1).data().toString() == "ÖDEME"){
            ui->OdemeMakbuzuBastoolButton->setEnabled(true);
            ui->TahsilatMakbuzuBastoolButton->setEnabled(false);
        }
        else if(ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 1).data().toString() == "TAHSİLAT"){
            ui->TahsilatMakbuzuBastoolButton->setEnabled(true);
            ui->OdemeMakbuzuBastoolButton->setEnabled(false);
        }
        else{
            ui->TahsilatMakbuzuBastoolButton->setEnabled(false);
            ui->OdemeMakbuzuBastoolButton->setEnabled(false);
        }
    }
}

void CariKartlarDialog::on_YenitoolButton_clicked()
{
    ui->SiltoolButton->setEnabled(false);
}

void CariKartlarDialog::setVergiDaireleri(const QStringList &newVergiDaireleri)
{
    vergiDaireleri = newVergiDaireleri;
}

void CariKartlarDialog::on_CaridenTahsilatYaptoolButton_clicked()
{
    CariHareketiEkleForm *cariHareketForm = new CariHareketiEkleForm(this);
//    qDebug() << "cariid :" << ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString();
    cariHareketForm->setCariID(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
    cariHareketForm->setFaturaTip(CariHareketiEkleForm::FaturaTipi(tahsilat));
    cariHareketForm->setWindowTitle("Cariden Tahsilat Yap");
    cariHareketForm->setKullanici(kullanici);
    cariHareketForm->exec();
}

