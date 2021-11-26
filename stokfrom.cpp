#include "stokfrom.h"
#include "ui_stokfrom.h"
#include "stokgrupform.h"
#include "veritabani.h"
#include "user.h"
//***************************
#include <QSqlQueryModel>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QDateTime>
#include <QModelIndex>
#include <QSound>

QItemSelectionModel *seciliSatirModel;
int seciliSatirIndex;
QSound uyariSes(":/sounds/sounds/warning-sound.wav");

StokFrom::StokFrom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StokFrom)
{
    ui->setupUi(this);

    formLoad();
}

StokFrom::~StokFrom()
{
    delete ui;
}

void StokFrom::on_StokGrupBtn_clicked()
{
    StokGrupForm *stkgrupform = new StokGrupForm(this);
    stkgrupform->setModal(true);
    stkgrupform->show();
}

void StokFrom::formLoad()
{
    grupComboboxDoldur();
    stokKartlariniListele();

//    connect(ui->StokKartlaritableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),SLOT(alanlariDoldur()));

    QRegExp rgx("(|\"|/|\\.|[0-9]){13}");// lineEdit'e sadece rakam girmesi için QRegExp tanımlaması.
    ui->BarkodLnEdit->setValidator(new QRegExpValidator(rgx, this));// setValidator'üne QRegExpValidator'ü belirtme.

    ui->AraLineEdit->setFocus();
}

void StokFrom::setUser(User user)
{
    kullanici = user;
    this->setWindowTitle("STOK KARTLARI - " + kullanici.getUserName());
}

void StokFrom::grupComboboxDoldur()
{
    Veritabani *vt = new Veritabani();
    QStringList gruplar = vt->stokGruplariGetir();
    foreach (auto grup, gruplar) {
        ui->StokGrubuComboBox->addItem(grup);
    }
    delete vt;
}


void StokFrom::stokKartlariniListele()
{
    Veritabani *vt = new Veritabani();
    ui->StokKartlaritableView->setModel(vt->getStokKartlari());
    ui->StokKartiAdetLabel->setText(QString::number(ui->StokKartlaritableView->model()->rowCount()));
    ui->StokKartlaritableView->setSortingEnabled(true);
    ui->StokKartlaritableView->resizeColumnsToContents();
    QItemSelectionModel *selectionModel = ui->StokKartlaritableView->selectionModel();
    QModelIndex modelindex = ui->StokKartlaritableView->model()->index(0, 0);
    selectionModel->select(modelindex, QItemSelectionModel::Clear);
    connect(ui->StokKartlaritableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),SLOT(alanlariDoldur()));
    delete vt;
}

void StokFrom::on_YeniBtn_clicked()
{
    if(!ui->stokKartBilGroupBox->isEnabled())
    {
        ui->stokKartBilGroupBox->setEnabled(true);
        ui->DuzenleBtn->setEnabled(false);
        ui->SilBtn->setEnabled(false);
        ui->StokKartlaritableView->setEnabled(false);
        ui->AraLineEdit->setEnabled(false);
        ui->araBtn->setEnabled(false);
        ui->BarkodLnEdit->setFocus();
    }
    alanlariTemizle();
    ui->BarkodLnEdit->setFocus();
}

void StokFrom::on_DuzenleBtn_clicked()
{
    seciliIndex = ui->StokKartlaritableView->currentIndex().row();
    if(!ui->stokKartBilGroupBox->isEnabled())
    {
        if(ui->StokKartlaritableView->currentIndex().row() > -1)
        {
            ui->stokKartBilGroupBox->setEnabled(true);
            ui->YeniBtn->setEnabled(false);
            ui->SilBtn->setEnabled(false);
            ui->StokKartlaritableView->setEnabled(false);
            ui->Fwbtn->setEnabled(false);
            ui->BckBtn->setEnabled(false);
            ui->AraLineEdit->setEnabled(false);
            ui->araBtn->setEnabled(false);
            ui->BarkodLnEdit->setFocus();
        }
        else
        {
            uyariSes.play();
            QMessageBox msg;
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Bir stok kartı seçiniz.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        //QItemSelectionModel *select = ui->StokKartlaritableView->selectionModel();
        //if(select.hasSelection()){
        //      böylede yapabilirsin.
        //}
    }
}


void StokFrom::on_IptalBtn_clicked()
{
    ui->stokKartBilGroupBox->setEnabled(false);
    alanlariTemizle();
    ui->YeniBtn->setEnabled(true);
    ui->DuzenleBtn->setEnabled(true);
    ui->SilBtn->setEnabled(true);
    ui->StokKartlaritableView->setEnabled(true);
    ui->Fwbtn->setEnabled(true);
    ui->BckBtn->setEnabled(true);
    ui->AraLineEdit->setEnabled(true);
    ui->araBtn->setEnabled(true);
    ui->StokKartlaritableView->setFocus();
    alanlariDoldur();
}

void StokFrom::alanlariTemizle()
{
    ui->BarkodLnEdit->clear();
    ui->StokAdiLnEdit->clear();
    ui->BirimiComboBox->setCurrentIndex(0);
    ui->MiktarLnEdit->clear();
    ui->StokGrubuComboBox->setCurrentIndex(-1);
    ui->AFiyatdoubleSpinBox->setValue(0);
    ui->SFiyatdoubleSpinBox->setValue(0);
    ui->KDVspinBox->setValue(0);
    ui->AciklamaLnEdit->clear();
}


void StokFrom::alanlariDoldur()
{
    seciliSatirIndex = ui->StokKartlaritableView->currentIndex().row();
    seciliSatirModel = ui->StokKartlaritableView->selectionModel();
    ui->BarkodLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 1).data().toString());
    ui->StokAdiLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 2).data().toString());
    for (int i = 0; i <ui->BirimiComboBox->count(); ++i)
    {
        int result = QString::compare(ui->BirimiComboBox->itemText(i), seciliSatirModel->model()->index(seciliSatirIndex, 3).data().toString(), Qt::CaseInsensitive);
        if(result == 0)
        {
            ui->BirimiComboBox->setCurrentIndex(i);
            break;
        }
    }
    ui->MiktarLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 4).data().toString());
    for (int i = 0; i <ui->StokGrubuComboBox->count(); ++i)
    {
        int result = QString::compare(ui->StokGrubuComboBox->itemText(i), seciliSatirModel->model()->index(seciliSatirIndex, 5).data().toString(), Qt::CaseInsensitive);
        if(result == 0)
        {
            ui->StokGrubuComboBox->setCurrentIndex(i);
            break;
        }
    }
    ui->AFiyatdoubleSpinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 6).data().toDouble());
    ui->SFiyatdoubleSpinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 7).data().toDouble());
    ui->KDVspinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 8).data().toInt());
    ui->AciklamaLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 10).data().toString());
}

void StokFrom::closeEvent(QCloseEvent *)
{

}

void StokFrom::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        if(ui->stokKartBilGroupBox->isEnabled()){
            uyariSes.play();
            QMessageBox msg;
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Question);
            msg.setText("Stok kartını düzenlemeyi bitirmediniz.\nYinede çıkmak istiyor musunuz?");
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setDefaultButton(QMessageBox::Yes);
            msg.setButtonText(QMessageBox::Yes, "Evet");
            msg.setButtonText(QMessageBox::No, "Hayır");
            int cevap = msg.exec();
            if(QMessageBox::Yes == cevap){
                this->close();
            }
        }
        else{
            this->close();
        }
    }
    else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        if(ui->stokKartBilGroupBox->isEnabled()){
            emit on_KaydetBtn_clicked();
        }
        else if(ui->AraLineEdit->isEnabled()){
            emit on_araBtn_clicked();
        }
    }
}


void StokFrom::on_KaydetBtn_clicked()
{
    Veritabani *vt = new Veritabani();
    if(ui->YeniBtn->isEnabled()){// yenibtn aktifse yeni stok kartı kaydı oluşturur.

        StokKarti *yeniStokKarti = new StokKarti();
        yeniStokKarti->setBarkod(ui->BarkodLnEdit->text());
        yeniStokKarti->setAd(QLocale().toUpper(ui->StokAdiLnEdit->text()));
        yeniStokKarti->setBirim(ui->BirimiComboBox->currentText());
        yeniStokKarti->setMiktar(ui->MiktarLnEdit->text().toFloat());
        yeniStokKarti->setGrup(ui->StokGrubuComboBox->currentText());
        yeniStokKarti->setAFiyat(ui->AFiyatdoubleSpinBox->value());
        yeniStokKarti->setSFiyat(ui->SFiyatdoubleSpinBox->value());
        yeniStokKarti->setKdv(ui->KDVspinBox->value());
        yeniStokKarti->setTarih(QDateTime::currentDateTime());
        yeniStokKarti->setAciklama(QLocale().toUpper("stok kartı oluşturuldu"));
        vt->yeniStokKartiOlustur(yeniStokKarti, &kullanici);
    }
    else if(ui->DuzenleBtn->isEnabled()){// duzenlebtn aktifse mevcut stok kartını günceller.

        seciliSatirIndex = ui->StokKartlaritableView->currentIndex().row();
        seciliSatirModel = ui->StokKartlaritableView->selectionModel();
        QString duzenlenecekStokKartiID(seciliSatirModel->model()->index(seciliSatirIndex, 0).data().toString());
        StokKarti *yeniStokKarti = new StokKarti();
        yeniStokKarti->setBarkod(ui->BarkodLnEdit->text());
        yeniStokKarti->setAd(QLocale().toUpper(ui->StokAdiLnEdit->text()));
        yeniStokKarti->setBirim(ui->BirimiComboBox->currentText());
        yeniStokKarti->setMiktar(ui->MiktarLnEdit->text().toFloat());
        yeniStokKarti->setGrup(ui->StokGrubuComboBox->currentText());
        yeniStokKarti->setAFiyat(ui->AFiyatdoubleSpinBox->value());
        yeniStokKarti->setSFiyat(ui->SFiyatdoubleSpinBox->value());
        yeniStokKarti->setKdv(ui->KDVspinBox->value());
        yeniStokKarti->setTarih(QDateTime::currentDateTime());
        yeniStokKarti->setAciklama("stok kartı güncelleme");
        vt->stokKartiniGuncelle(duzenlenecekStokKartiID, yeniStokKarti, &kullanici);

    }
    alanlariTemizle();
    stokKartlariniListele();
    ui->StokKartlaritableView->setFocus();
    alanlariDoldur();
    ui->StokKartlaritableView->selectRow(seciliIndex);
    emit on_IptalBtn_clicked();
    delete vt;
}


void StokFrom::on_SilBtn_clicked()
{
    if(ui->StokKartlaritableView->currentIndex().row() != -1){
        seciliSatirIndex = ui->StokKartlaritableView->currentIndex().row();
        seciliSatirModel = ui->StokKartlaritableView->selectionModel();
        uyariSes.play();
        QMessageBox msg;
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Question);
        msg.setText(QString("%1 barkod numaralı \n%2 isimli stok kartını silmek istediğinize emin misiniz?").arg(seciliSatirModel->model()->index(seciliSatirIndex, 1).data().toString()).arg(seciliSatirModel->model()->index(seciliSatirIndex, 2).data().toString()));
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        msg.setDefaultButton(QMessageBox::Yes);
        int cevap = msg.exec();
        if(cevap == QMessageBox::Yes){
            Veritabani *vt = new Veritabani();
            vt->stokKartiSil(seciliSatirModel->model()->index(seciliSatirIndex, 0).data().toString());
            stokKartlariniListele();
            emit on_IptalBtn_clicked();
            delete vt;
        }
    }
    else{
        uyariSes.play();
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Bir stok kartı seçiniz");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
    }
    ui->StokKartlaritableView->setFocus();
}


void StokFrom::on_BckBtn_clicked()
{
    ui->StokKartlaritableView->selectRow(ui->StokKartlaritableView->currentIndex().row() - 1);
}


void StokFrom::on_Fwbtn_clicked()
{
    ui->StokKartlaritableView->selectRow(ui->StokKartlaritableView->currentIndex().row() + 1);
}

void StokFrom::stokKartiAra(QString aranacakMetin)
{
    bool bulundumu(false);
    if(ui->barkodRadioButton->isChecked()){
        for (int i = 0; i < ui->StokKartlaritableView->model()->rowCount(); ++i) {
            if(ui->StokKartlaritableView->model()->index(i, 1).data().toString().contains(aranacakMetin, Qt::CaseInsensitive)){
                ui->StokKartlaritableView->selectRow(i);
                bulundumu = true;
                break;
            }
        }
    }
    if(ui->adRadioButton->isChecked()){
        if(ui->barkodRadioButton->isChecked()){
            for (int i = 0; i < ui->StokKartlaritableView->model()->rowCount(); ++i) {
                if(ui->StokKartlaritableView->model()->index(i, 2).data().toString().contains(aranacakMetin, Qt::CaseInsensitive)){
                    ui->StokKartlaritableView->selectRow(i);
                    bulundumu = true;
                    break;
                }
            }
        }
    }
    if(!bulundumu){
        uyariSes.play();
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Stok kartı bulunamadı");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
        ui->StokKartlaritableView->clearSelection();
        StokFrom::alanlariTemizle();
        ui->AraLineEdit->clear();
    }
}


void StokFrom::on_araBtn_clicked()
{
    stokKartiAra(ui->AraLineEdit->text());
}




void StokFrom::on_dosyadanToolButton_clicked()
{

}

