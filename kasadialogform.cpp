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
#include "kasadialogform.h"
#include "ui_kasadialogform.h"
#include "kasahareketekledialog.h"
//**************************************
#include <QDebug>
#include <QDateTime>

KasaDialogForm::KasaDialogForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KasaDialogForm)
{
    ui->setupUi(this);
    FormLoad();
}

KasaDialogForm::~KasaDialogForm()
{
    delete ui;
}

void KasaDialogForm::FormLoad()
{
    ui->BaslangicdateTimeEdit->setDate(QDate::currentDate());
    ui->BitisdateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void KasaDialogForm::on_BaslangicdateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    baslangicTarih = dateTime;
    baslangicTarih.setTime(QTime());// boş QTime() atayarak seçili tarihin saatini 0:0:0 yapıyorum
    KasaHareketleriListele();
}


void KasaDialogForm::on_BitisdateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    bitisTarih = dateTime;
    bitisTarih.setTime(QTime(23, 59, 59, 999));
    KasaHareketleriListele();
}

void KasaDialogForm::KasaHareketleriListele()
{
    // kasa hareketlerini getirme
    ui->KasaHareketleritableView->setModel(kasaYonetimi.getKasaHareketleri(baslangicTarih, bitisTarih));
    ui->KasaHareketleritableView->setSortingEnabled(true);
    ui->KasaHareketleritableView->resizeColumnsToContents();
    ui->KasaHareketleritableView->hideColumn(0);
    ui->KasaHareketleritableView->clearSelection();
    // kasaya toplam giren parayı getirme
    ui->ToplamGirislabel->setText("₺" + QString::number(kasaYonetimi.getKasaToplamGiren(baslangicTarih, bitisTarih), 'f', 2));
    // kasadan toplam çıkan parayı getirme
    ui->ToplamCikislabel->setText("₺" + QString::number(kasaYonetimi.getKasaToplamCikan(baslangicTarih, bitisTarih), 'f', 2));
    // net kârı getirme
    ui->NetKarlabel->setText("₺" + QString::number(kasaYonetimi.getNetKar(baslangicTarih, bitisTarih), 'f', 2));
    //kasadaki parayı getirme
    ui->KasadakiParalabel->setText("₺" + QString::number(kasaYonetimi.getKasadakiPara(), 'f', 2));
}

void KasaDialogForm::on_KasaGirisYaptoolButton_clicked()
{
    KasaHareketEkleDialog *hareketForm = new KasaHareketEkleDialog(this);
    hareketForm->setKullanici(kullanici);
    hareketForm->setTarih(QDateTime::currentDateTime());
    hareketForm->setHareket(KasaYonetimi::KasaHareketi::Giris);// giriş hareketi seçilsin.
    hareketForm->sethareketDuzenle(false);
    hareketForm->setWindowTitle("Kasaya Giriş Yap");
    hareketForm->exec();
    delete hareketForm;
    KasaHareketleriListele();
}

void KasaDialogForm::setKullanici(User newKullanici)
{
    kullanici = newKullanici;
}


void KasaDialogForm::on_KasaCikisYaptoolButton_clicked()
{
    KasaHareketEkleDialog *hareketForm = new KasaHareketEkleDialog(this);
    hareketForm->setKullanici(kullanici);
    hareketForm->setTarih(QDateTime::currentDateTime());
    hareketForm->setHareket(KasaYonetimi::KasaHareketi::Cikis);// çıkış hareketi seçilsin.
    hareketForm->sethareketDuzenle(false);
    hareketForm->setWindowTitle("Kasadan Çıkış Yap");
    hareketForm->exec();
    delete hareketForm;
    KasaHareketleriListele();
}


void KasaDialogForm::on_DuzelttoolButton_clicked()
{
    KasaHareketEkleDialog *hareketForm = new KasaHareketEkleDialog(this);
    hareketForm->setKullanici(kullanici);
    hareketForm->sethareketDuzenle(true);
    hareketForm->setWindowTitle("Kasa Hareketi Düzenle");
    hareketForm->setHareketID(ui->KasaHareketleritableView->model()->index(ui->KasaHareketleritableView->currentIndex().row(), 0).data().toString());
    hareketForm->setTarih(ui->KasaHareketleritableView->model()->index(ui->KasaHareketleritableView->currentIndex().row(), 3).data().toDateTime());
    hareketForm->setTutar(ui->KasaHareketleritableView->model()->index(ui->KasaHareketleritableView->currentIndex().row(), 2).data().toDouble());
    hareketForm->setAciklama(ui->KasaHareketleritableView->model()->index(ui->KasaHareketleritableView->currentIndex().row(), 6).data().toString());
    hareketForm->setEvrakNo(ui->KasaHareketleritableView->model()->index(ui->KasaHareketleritableView->currentIndex().row(), 5).data().toString());
    if(ui->KasaHareketleritableView->model()->index(ui->KasaHareketleritableView->currentIndex().row(), 1).data().toString() == "GİRİŞ"){
        hareketForm->setHareket(KasaYonetimi::KasaHareketi::Giris);
    }
    else if(ui->KasaHareketleritableView->model()->index(ui->KasaHareketleritableView->currentIndex().row(), 1).data().toString() == "ÇIKIŞ"){
        hareketForm->setHareket(KasaYonetimi::KasaHareketi::Cikis);
    }
    hareketForm->exec();
    delete hareketForm;
    KasaHareketleriListele();
}


void KasaDialogForm::on_SiltoolButton_clicked()
{
    QItemSelectionModel *seciliSatir = ui->KasaHareketleritableView->selectionModel();
    if(seciliSatir->hasSelection()){
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Seçili işlemi silmek istediğinize emin misiniz?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        int msgsonuc = msg.exec();
        if(QMessageBox::Yes == msgsonuc){
            bool sonuc = kasaYonetimi.kasaHareketiSil(ui->KasaHareketleritableView->model()->index(ui->KasaHareketleritableView->currentIndex().row(), 0).data().toString(),
                                             ui->KasaHareketleritableView->model()->index(ui->KasaHareketleritableView->currentIndex().row(), 1).data().toString(),
                                             ui->KasaHareketleritableView->model()->index(ui->KasaHareketleritableView->currentIndex().row(), 2).data().toDouble());
            if(sonuc == true){
                QMessageBox msg(this);
                msg.setWindowTitle("Uyarı");
                msg.setIcon(QMessageBox::Information);
                msg.setText("Kasa hareketi silindi.");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
            }
            KasaHareketleriListele();
        }
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Listeden bir işlem seçin");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    delete seciliSatir;
}


void KasaDialogForm::on_KapatpushButton_clicked()
{
    this->close();
}


void KasaDialogForm::on_RaporpushButton_clicked()
{

}

