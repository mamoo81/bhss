#include "carikartlardialog.h"
#include "ui_carikartlardialog.h"
#include "carihareketiekleform.h"
#include "yenicarikartdialog.h"
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
    //cari kartların getirilmesi
    CariKartlarDialog::cariKartlariListele();
    //tarihleri ayarlama
    ui->bitisdateEdit->setDateTime(QDateTime::currentDateTime());
    connect(ui->CariKartHareketleritableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(cariHareketleriTableSelectionChanged()));
}

void CariKartlarDialog::cariKartlariListele()
{
    ui->CariKartlartableView->setModel(vt.getCariKartIsimleri());
    ui->CariKartlartableView->resizeColumnsToContents();
    connect(ui->CariKartlartableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(cariHareketleriListele()));
    ui->CariKartlartableView->selectRow(0);
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
    YeniCariKartDialog *yeniCari = new YeniCariKartDialog(this);
    yeniCari->setDuzenle(false);
    yeniCari->exec();
    CariKartlarDialog::cariKartlariListele();
    delete yeniCari;
}

void CariKartlarDialog::setVergiDaireleri(const QStringList &newVergiDaireleri)
{
    vergiDaireleri = newVergiDaireleri;
}

void CariKartlarDialog::on_CaridenTahsilatYaptoolButton_clicked()
{
    CariHareketiEkleForm *cariHareketForm = new CariHareketiEkleForm(this);
    cariHareketForm->setCariID(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
    cariHareketForm->setFaturaTip(CariHareketiEkleForm::FaturaTipi(tahsilat));// faturalar tablosuna tahsilat faturasi girilmesi için
    cariHareketForm->setWindowTitle("Cariden Tahsilat Yap");
    cariHareketForm->setKullanici(kullanici);
    cariHareketForm->exec();
    CariKartlarDialog::cariHareketleriListele();
    delete cariHareketForm;
}


void CariKartlarDialog::on_CariyeOdemeYaptoolButton_clicked()
{
    CariHareketiEkleForm *cariHareketForm = new CariHareketiEkleForm(this);
    cariHareketForm->setCariID(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
    cariHareketForm->setWindowTitle("Cariye Ödeme Yap");
    cariHareketForm->setKullanici(kullanici);
    cariHareketForm->setFaturaTip(CariHareketiEkleForm::FaturaTipi(odeme));// faturalar tablosuna ödeme işlemi girilmesi için
    cariHareketForm->exec();
    CariKartlarDialog::cariHareketleriListele();
    delete cariHareketForm;
}


void CariKartlarDialog::on_SiltoolButton_clicked()
{
    if(ui->CariKartlartableView->currentIndex().row() > 0){// DİREKT CARİSİNİ SİLEMESİN.
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Question);
        msg.setText(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 1).data().toString() + " carisi silinecek!\n\nSilmek istediğinize emin misiniz?");
        msg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        msg.setDefaultButton(QMessageBox::No);
        msg.exec();
        if(msg.result() == QMessageBox::Yes){
            vt.cariKartSil(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
            CariKartlarDialog::cariKartlariListele();
        }
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("DİREKT carisini silemezsiniz!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}


void CariKartlarDialog::on_CariyiAlacaklandirtoolButton_clicked()
{
    CariHareketiEkleForm *cariHareketForm = new CariHareketiEkleForm(this);
    cariHareketForm->setCariID(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
    cariHareketForm->setWindowTitle("Cariyi Alacaklandır");
    cariHareketForm->setKullanici(kullanici);
    cariHareketForm->setFaturaTip(CariHareketiEkleForm::FaturaTipi(alis)); // faturalar tablosuna alış faturası girilmesi için
    cariHareketForm->exec();
    CariKartlarDialog::cariHareketleriListele();
    delete cariHareketForm;
}


void CariKartlarDialog::on_CariyiBorclandirtoolButton_clicked()
{
    CariHareketiEkleForm *cariHareketForm = new CariHareketiEkleForm(this);
    cariHareketForm->setCariID(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
    cariHareketForm->setWindowTitle("Cariyi Borçlandır");
    cariHareketForm->setKullanici(kullanici);
    cariHareketForm->setFaturaTip(CariHareketiEkleForm::FaturaTipi(satis)); // faturalar tablosuna alış faturası girilmesi için
    cariHareketForm->exec();
    CariKartlarDialog::cariHareketleriListele();
    delete cariHareketForm;
}

void CariKartlarDialog::on_DuzenletoolButton_clicked()
{
    if(ui->CariKartlartableView->currentIndex().row() != 0){
        YeniCariKartDialog *yeniCari = new YeniCariKartDialog(this);
        yeniCari->setWindowTitle("Cari Kartı Düzenle");
        yeniCari->setDuzenle(true);
        yeniCari->setDuzenlenecekCariID(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
        yeniCari->exec();
        CariKartlarDialog::cariKartlariListele();
        delete yeniCari;
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("DİREKT carisini düzenleyemessiniz!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

