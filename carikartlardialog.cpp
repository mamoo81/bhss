/*####################################
MIT LICENCE                          #
######################################
Copyright 2021 Mehmet AKDEMİR        #
bilgi@basat.dev                      #
######################################
Permission is hereby granted, free of charge,
to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "carikartlardialog.h"
#include "ui_carikartlardialog.h"
#include "carihareketiekleform.h"
#include "yenicarikartdialog.h"
#include "satisgosterdialog.h"
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
    ui->CariKartHareketleritableView->setSortingEnabled(true);
    ui->cariToplamAlacaklabel->setText(QString::number(vt.getCariToplamAlacak(cariID), 'f', 2));
    ui->cariToplamBorclabel->setText(QString::number(vt.getCariToplamBorc(cariID), 'f', 2));
    // butonların aktif/pasif durumları ayarlama
    if(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString() == "1"){//direkt carisi ise
        ui->CaridenTahsilatYaptoolButton->setEnabled(false);
        ui->CariyeOdemeYaptoolButton->setEnabled(false);
        ui->CariyiBorclandirtoolButton->setEnabled(false);
        ui->CariyiAlacaklandirtoolButton->setEnabled(false);
        ui->CariBankatoolButton->setEnabled(false);
        ui->CariyeStokluSatistoolButton->setEnabled(false);
        ui->CariyeStoksuzSatistoolButton->setEnabled(false);
    }
    else{
        // carinin borcu 0 veya düşükse tahsilat butonu pasif yap.
        if(ui->cariToplamBorclabel->text().toDouble() <= 0){
            ui->CaridenTahsilatYaptoolButton->setEnabled(false);
        }
        else{
            ui->CaridenTahsilatYaptoolButton->setEnabled(true);
        }
        // carinin alacağı 0 veya düşükse tahsilat butonu pasif yap
        if(ui->cariToplamAlacaklabel->text().toDouble() <= 0){
            ui->CariyeOdemeYaptoolButton->setEnabled(false);
        }
        else{
            ui->CariyeOdemeYaptoolButton->setEnabled(true);
        }
        ui->CariyiBorclandirtoolButton->setEnabled(true);
        ui->CariyiAlacaklandirtoolButton->setEnabled(true);
        ui->CariBankatoolButton->setEnabled(true);
        ui->CariyeStokluSatistoolButton->setEnabled(true);
        ui->CariyeStoksuzSatistoolButton->setEnabled(true);
    }

    double carilerToplamBorc = vt.getcarilerToplamBorc();
    double carilerToplamAlacak = vt.getCarilerToplamAlacak();
    ui->CarilerToplamBorclabel->setText(QString::number(carilerToplamBorc, 'f', 2));
    ui->CarilerToplamAlacaklabel->setText(QString::number(carilerToplamAlacak, 'f', 2));
    ui->CarilerBakiyelabel->setText(QString::number((carilerToplamBorc - carilerToplamAlacak), 'f', 2));
}

void CariKartlarDialog::cariHareketleriTableSelectionChanged()
{
    if(ui->CariKartHareketleritableView->currentIndex().row() > -1){
        ui->CariislemsiltoolButton->setEnabled(true);

        if(ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 2).data().toString() == "ÖDEME"){
            ui->OdemeMakbuzuBastoolButton->setEnabled(true);
            ui->TahsilatMakbuzuBastoolButton->setEnabled(false);
        }
        else if(ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 2).data().toString() == "TAHSİLAT"){
            ui->TahsilatMakbuzuBastoolButton->setEnabled(true);
            ui->OdemeMakbuzuBastoolButton->setEnabled(false);
        }
        else{
            ui->TahsilatMakbuzuBastoolButton->setEnabled(false);
            ui->OdemeMakbuzuBastoolButton->setEnabled(false);
        }
    }
    else{
        ui->CariislemsiltoolButton->setEnabled(false);
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


void CariKartlarDialog::on_CariKartHareketleritableView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    if(ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 2).data().toString() == "SATIŞ"){
        SatisGosterDialog *gecmisSatisForm = new SatisGosterDialog(this);
        gecmisSatisForm->setSatisFaturaNo(ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 0).data().toString());
        gecmisSatisForm->sepetiCek();
        gecmisSatisForm->exec();
        delete gecmisSatisForm;
    }
}


void CariKartlarDialog::on_TahsilatMakbuzuBastoolButton_clicked()
{
    if(ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 2).data().toString() == "TAHSİLAT"){
        yazici.tahsilatMakbuzuBas(kullanici,
                                  vt.getCariKart(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString()),
                                  ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 4).data().toDouble(),
                                  ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 0).data().toString(),
                                  ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 1).data().toDateTime(),
                                  ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 8).data().toString());
        QMessageBox msg(this);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Tahsilat makbuzu yazdırıldı.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Cari hareketlerinden bir tahsilat seçiniz.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}


void CariKartlarDialog::on_CariislemsiltoolButton_clicked()
{
    if(ui->CariKartHareketleritableView->selectionModel()->hasSelection()){

        QString hareketTipi = ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 2).data().toString();

        if(hareketTipi == "SATIŞ"){
            QString silinecekCariHareketFaturaNo = ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 0).data().toString();

            uyariSesi->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Warning);
            msg.setText(QString("%1 nolu satış faturası silinecek\nSatılan ürünler stoğa geri eklenecek\nKasadan fatura turarı ve kar düşülecek.\n\n"
                                "İşlemi yapmak istediğinize emin misiniz?").arg(silinecekCariHareketFaturaNo));
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setDefaultButton(QMessageBox::No);
            if(msg.exec() == QMessageBox::Yes){
                Cari cari = vt.getCariKart(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
                vt.cariHareketiSil(silinecekCariHareketFaturaNo, kullanici, cari);
            }
        }
        cariHareketleriListele();
    }
}
