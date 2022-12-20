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
#include <QStandardItemModel>

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

void CariKartlarDialog::CariAlacakBorcHesapla()
{
    Cari cari = cariYonetimi.getCariKart(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());

    // carinin toplam alacak/borç
    ui->cariToplamBorclabel->setText(QString::number(cariYonetimi.getCariToplamBorc(QString::number(cari.getId())), 'f', 2));
    ui->CarilerToplamBorclabel->setText(QString::number(cariYonetimi.getcarilerToplamBorc(ui->baslangicdateEdit->dateTime(), ui->bitisdateEdit->dateTime()), 'f', 2));
}


void CariKartlarDialog::formLoad()
{
    setVergiDaireleri(cariYonetimi.getVergiDaireleri());
    //cari kartların getirilmesi
    CariKartlarDialog::cariKartlariListele();
    //tarihleri ayarlama
    ui->bitisdateEdit->setDateTime(QDateTime::currentDateTime());
    connect(ui->CariKartHareketleritableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(cariHareketleriTableSelectionChanged()));
    connect(ui->CariKartlartableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(cariKartlarTableViewSelectionChanged()));
}

void CariKartlarDialog::cariKartlariListele()
{
    ui->CariKartlartableView->setModel(cariYonetimi.getCariKartIsimleri());
    ui->CariKartlartableView->resizeColumnsToContents();
    connect(ui->CariKartlartableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(cariHareketleriListele()));
    ui->CariKartlartableView->selectRow(0);
}
void CariKartlarDialog::cariHareketleriListele()
{
    //cari seçilince yapılacaklar
    Cari cariKart = cariYonetimi.getCariKart(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
    ui->CariKartHareketleritableView->setModel(cariYonetimi.getCariHareketleri(QString::number(cariKart.getId())));
    ui->CariKartHareketleritableView->resizeColumnsToContents();
    ui->CariKartHareketleritableView->setSortingEnabled(true);

    //carinin borcunu getirme
    ui->cariToplamAlacaklabel->setText(QString::number(cariYonetimi.getCariToplamAlacak(QString::number(cariKart.getId())), 'f', 2));
    ui->cariToplamBorclabel->setText(QString::number(cariYonetimi.getCariToplamBorc(QString::number(cariKart.getId())), 'f', 2));
    ui->CariBakiyesilabel->setText(QString::number(ui->cariToplamBorclabel->text().toDouble() - ui->cariToplamAlacaklabel->text().toDouble(), 'f', 2));

    // carilerin borcunu getirme
    double carilerToplamBorc = 0;
    double carilerToplamAlacak = 0;

//    carilerToplamBorc = cariYonetimi.getcarilerToplamBorc(true, ui->baslangicdateEdit->dateTime(), ui->bitisdateEdit->dateTime());
//    carilerToplamAlacak = cariYonetimi.getCarilerToplamAlacak();

    ui->CarilerToplamBorclabel->setText(QString::number(carilerToplamBorc, 'f', 2));
    ui->CarilerToplamAlacaklabel->setText(QString::number(carilerToplamAlacak, 'f', 2));
    ui->CarilerBakiyelabel->setText(QString::number((carilerToplamBorc - carilerToplamAlacak), 'f', 2));

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
    ui->CariislemsiltoolButton->setEnabled(false);
}

void CariKartlarDialog::cariHareketleriTableSelectionChanged()
{
    if(ui->CariKartHareketleritableView->selectionModel()->hasSelection()){
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

void CariKartlarDialog::cariKartlarTableViewSelectionChanged()
{
    if(ui->CariKartlartableView->selectionModel()->hasSelection()){
        ui->tumHareketlerisiltoolButton->setEnabled(true);
    }
    else{
        ui->tumHareketlerisiltoolButton->setEnabled(false);
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
        cariYonetimi.cariKartSil(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
        CariKartlarDialog::cariKartlariListele();
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
    YeniCariKartDialog *yeniCari = new YeniCariKartDialog(this);
    yeniCari->setWindowTitle("Cari Kartı Düzenle");
    yeniCari->setDuzenle(true);
    yeniCari->setDuzenlenecekCariID(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
    yeniCari->exec();
    CariKartlarDialog::cariKartlariListele();
    delete yeniCari;
}


void CariKartlarDialog::on_CariKartHareketleritableView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    if(ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 2).data().toString() == "SATIŞ"){
        SatisGosterDialog *gecmisSatisForm = new SatisGosterDialog(this);
        gecmisSatisForm->setSatisFaturaNo(ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 0).data().toString());
        gecmisSatisForm->setCari(cariYonetimi.getCariKart(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString()));
        gecmisSatisForm->sepetiCek();
        gecmisSatisForm->exec();
        delete gecmisSatisForm;
    }
}


void CariKartlarDialog::on_TahsilatMakbuzuBastoolButton_clicked()
{
    if(ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 2).data().toString() == "TAHSİLAT"){
        yazici.tahsilatMakbuzuBas(kullanici,
                                  cariYonetimi.getCariKart(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString()),
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
        QString silinecekCariHareketFaturaNo = ui->CariKartHareketleritableView->model()->index(ui->CariKartHareketleritableView->currentIndex().row(), 0).data().toString();
        if(hareketTipi == "SATIŞ"){

            uyariSesi->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Warning);
            msg.setText(QString("%1 nolu satış faturası silinecek\nSatılan ürünler stoğa geri eklenecek\nKasadan fatura turarı ve kar düşülecek.\n\n"
                                "İşlemi yapmak istediğinize emin misiniz?").arg(silinecekCariHareketFaturaNo));
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setDefaultButton(QMessageBox::No);
            if(msg.exec() == QMessageBox::Yes){
                Cari cari = cariYonetimi.getCariKart(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
                if(cariYonetimi.cariHareketiSil(silinecekCariHareketFaturaNo, kullanici, cari)){
                    uyariSesi->play();
                    QMessageBox msg(this);
                    msg.setWindowTitle("Bilgi");
                    msg.setIcon(QMessageBox::Information);
                    msg.setText("Cari hareket silindi.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
                else{
                    uyariSesi->play();
                    QMessageBox msg(this);
                    msg.setWindowTitle("Bilgi");
                    msg.setIcon(QMessageBox::Information);
                    msg.setText("Cari hareket silinemedi.\n\nDaha önceden iade alınmış olabilir.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
            }
        }
        if(hareketTipi == "TAHSİLAT"){

            uyariSesi->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Warning);
            msg.setText(QString("%1 nolu TAHSİLAT silinecek\nKasadan TAHSİLAT turarı düşülecek.\n\n"
                                "İşlemi yapmak istediğinize emin misiniz?").arg(silinecekCariHareketFaturaNo));
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setDefaultButton(QMessageBox::No);
            if(msg.exec() == QMessageBox::Yes){
                Cari cari = cariYonetimi.getCariKart(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
                if(cariYonetimi.cariHareketiSil(silinecekCariHareketFaturaNo, kullanici, cari)){
                    uyariSesi->play();
                    QMessageBox msg(this);
                    msg.setWindowTitle("Bilgi");
                    msg.setIcon(QMessageBox::Information);
                    msg.setText("Cari hareket silindi.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
                else{
                    uyariSesi->play();
                    QMessageBox msg(this);
                    msg.setWindowTitle("Bilgi");
                    msg.setIcon(QMessageBox::Information);
                    msg.setText("Cari hareket silinemedi.\n\nDaha önceden iade alınmış olabilir.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
            }
        }
        if(hareketTipi == "ALIŞ"){

            uyariSesi->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Warning);
            msg.setText(QString("Carinin %1 nolu ALACAĞI silinecek\n\n"
                                "İşlemi yapmak istediğinize emin misiniz?").arg(silinecekCariHareketFaturaNo));
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setDefaultButton(QMessageBox::No);
            if(msg.exec() == QMessageBox::Yes){
                Cari cari = cariYonetimi.getCariKart(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
                if(cariYonetimi.cariHareketiSil(silinecekCariHareketFaturaNo, kullanici, cari)){
                    uyariSesi->play();
                    QMessageBox msg(this);
                    msg.setWindowTitle("Bilgi");
                    msg.setIcon(QMessageBox::Information);
                    msg.setText("Cari hareket silindi.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
                else{
                    uyariSesi->play();
                    QMessageBox msg(this);
                    msg.setWindowTitle("Bilgi");
                    msg.setIcon(QMessageBox::Information);
                    msg.setText("Cari hareket silinemedi.\n\nDaha önceden iade alınmış olabilir.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.exec();
                }
            }
        }
        if(hareketTipi == "ÖDEME"){

        }
        cariHareketleriListele();
    }
}


void CariKartlarDialog::on_tumHareketlerisiltoolButton_clicked()
{
    uyariSesi->play();
    QMessageBox msg(this);
    msg.setWindowTitle("Dikkat");
    msg.setIcon(QMessageBox::Question);
    msg.setText("Seçili carinin tüm geçmiş hareketleri silinecek emin misiniz?\n\nBu işlem tüm alacak/borç bilgisini siler.");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::No);
    if(msg.exec() == QMessageBox::Yes){
        Cari cari = cariYonetimi.getCariKart(ui->CariKartlartableView->model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
        if(cariYonetimi.carininTumHareketleriniSil(cari)){
            uyariSesi->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Tüm geçmiş hareketler silindi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();

            cariHareketleriListele();
        }
        else{
            uyariSesi->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Tüm geçmiş hareketler silinemedi");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }
    }
}


void CariKartlarDialog::on_aratoolButton_clicked()
{

//    if(ui->AralineEdit->text().length() > 3){
//        for (int var = 0; var < ui->CariKartlartableView->model()->rowCount(); ++var) {
//            if(ui->CariKartlartableView->model()->index(var,1).data().toString().contains(ui->AralineEdit->text())){

//            }
//        }
//    }
}


void CariKartlarDialog::on_AralineEdit_textChanged(const QString &arg1)
{
    ui->CariKartlartableView->clearSelection();
    for (int i = 0; i < ui->CariKartlartableView->model()->rowCount(); ++i) {
        if(!ui->CariKartlartableView->model()->index(i, 1).data().toString().contains(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(arg1), Qt::CaseInsensitive)){
            ui->CariKartlartableView->hideRow(i);
        }
        else{
            ui->CariKartlartableView->showRow(i);
        }
    }
}

