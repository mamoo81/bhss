#include "stokfrom.h"
#include "ui_stokfrom.h"
#include "stokgrupform.h"
#include "veritabani.h"
#include "user.h"
#include "birimekleform.h"
#include "satisgrafigiform.h"
#include "stokgircikdialog.h"
#include "toplustokyukledialog.h"
#include "stokhareketleridialog.h"
#include "resimekledialog.h"
#include "stokkartlarimodel.h"
//***************************
#include <QSqlQueryModel>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QDateTime>
#include <QModelIndex>
#include <QSound>
#include <QSettings>
#include <QStandardPaths>
#include <QRandomGenerator>

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

void StokFrom::StokMiktarlariniKontrolEt()
{
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    genelAyarlar.beginGroup("stok");
    bool takipAktifMi = genelAyarlar.value("takip").toBool();
    double UyariMiktari = genelAyarlar.value("uyarimiktar").toDouble();
    double MevcutMiktar = 0;
    genelAyarlar.endGroup();
    if(takipAktifMi){
        for (int row = 0; row < ui->StokKartlaritableView->model()->rowCount(); ++row) {
            MevcutMiktar = ui->StokKartlaritableView->model()->index(row, 5).data().toDouble();
            QModelIndex satir = ui->StokKartlaritableView->model()->index(row, 5);
            if(MevcutMiktar <= UyariMiktari && MevcutMiktar >= 1){

            }
            else if(MevcutMiktar < 1){

            }
        }
    }
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
    // stok birimlerini çekme
    ui->BirimiComboBox->clear();
    ui->BirimiComboBox->addItems(vt->getStokBirimleri());
    // üreticileri çekme
    ui->ureticicomboBox->addItems(vt->getUreticiler());
    // tedarikçileri getirme
    ui->tedarikcicomboBox->addItems(vt->getTedarikciler());
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
    QStringList gruplar = vt->stokGruplariGetir();
    foreach (auto grup, gruplar) {
        ui->StokGrubuComboBox->addItem(grup);
    }
}


void StokFrom::stokKartlariniListele()
{
    _stokKartlariTableModel = new StokKartlariModel();
    sortModel->setSourceModel(_stokKartlariTableModel);
    ui->StokKartlaritableView->setModel(sortModel);
    ui->StokKartiAdetLabel->setText(QString::number(ui->StokKartlaritableView->model()->rowCount()));
    ui->StokKartlaritableView->setSortingEnabled(true);
    ui->StokKartlaritableView->resizeColumnsToContents();
    ui->StokKartlaritableView->sortByColumn(3, Qt::AscendingOrder);
    selectionModel = ui->StokKartlaritableView->selectionModel();
    QModelIndex modelindex = ui->StokKartlaritableView->model()->index(0, 0);
    selectionModel->select(modelindex, QItemSelectionModel::Clear);
    connect(ui->StokKartlaritableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),SLOT(alanlariDoldur()));
    StokMiktarlariniKontrolEt();
}

void StokFrom::on_YeniBtn_clicked()
{
    if(!ui->BarkodLnEdit->isEnabled())
    {
        //butonlar
        ui->YeniBtn->setEnabled(false);
        ui->DuzenleBtn->setEnabled(false);
        ui->SilBtn->setEnabled(false);
        ui->StokKartlaritableView->setEnabled(false);
        ui->AraLineEdit->setEnabled(false);
        ui->araBtn->setEnabled(false);
        ui->StokGirBtn->setEnabled(false);
        ui->StokCikBtn->setEnabled(false);
        //textbox ve butonlar
        ui->BarkodLnEdit->setEnabled(true);
        ui->BarkodOlusturBtn->setEnabled(true);
        ui->StokKoduLnEdit->setEnabled(true);
        ui->StokAdiLnEdit->setEnabled(true);
        ui->BirimiComboBox->setEnabled(true);
        ui->MiktarLnEdit->setEnabled(true);
        ui->StokGrubuComboBox->setEnabled(true);
        ui->ureticicomboBox->setEnabled(true);
        ui->tedarikcicomboBox->setEnabled(true);
        ui->AFiyatdoubleSpinBox->setEnabled(true);
        ui->SFiyatdoubleSpinBox->setEnabled(true);
        ui->KDVspinBox->setEnabled(true);
        ui->OTVspinBox->setEnabled(true);
        ui->KDVcheckbox->setEnabled(true);
        ui->OTVcheckbox->setEnabled(true);
        ui->ResimEkleBtn->setEnabled(true);
        ui->ResimSilBtn->setEnabled(true);
        ui->SurekliYenicheckBox->setEnabled(true);
        ui->KaydetBtn->setEnabled(true);
        ui->IptalBtn->setEnabled(true);
        yeniKayit = true;
    }
    alanlariTemizle();
    ui->BarkodLnEdit->setFocus();
}

void StokFrom::on_DuzenleBtn_clicked()
{
    seciliIndex = ui->StokKartlaritableView->currentIndex().row();
    if(ui->StokKartlaritableView->currentIndex().row() > -1)
    {
        if(!ui->BarkodLnEdit->isEnabled())
        {
            //butonlar
            ui->YeniBtn->setEnabled(false);
            ui->DuzenleBtn->setEnabled(false);
            ui->SilBtn->setEnabled(false);
            ui->StokKartlaritableView->setEnabled(false);
            ui->AraLineEdit->setEnabled(false);
            ui->araBtn->setEnabled(false);
            ui->StokGirBtn->setEnabled(false);
            ui->StokCikBtn->setEnabled(false);
            ui->MiktarLnEdit->setEnabled(false);
            //textbox ve butonlar
            ui->BarkodLnEdit->setEnabled(true);
            ui->StokKoduLnEdit->setEnabled(true);
            ui->StokAdiLnEdit->setEnabled(true);
            ui->BirimiComboBox->setEnabled(true);
            ui->ureticicomboBox->setEnabled(true);
            ui->tedarikcicomboBox->setEnabled(true);
            ui->StokGrubuComboBox->setEnabled(true);
            ui->AFiyatdoubleSpinBox->setEnabled(true);
            ui->SFiyatdoubleSpinBox->setEnabled(true);
            ui->KDVspinBox->setEnabled(true);
            ui->OTVspinBox->setEnabled(true);
            ui->KDVcheckbox->setEnabled(true);
            ui->OTVcheckbox->setEnabled(true);
            ui->ResimEkleBtn->setEnabled(true);
            ui->ResimSilBtn->setEnabled(true);
            ui->KaydetBtn->setEnabled(true);
            ui->IptalBtn->setEnabled(true);
            ui->BarkodLnEdit->setFocus();
            yeniKayit = false;
        }
    }
    else
    {
        uyariSes.play();
        QMessageBox msg(this);
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


void StokFrom::on_IptalBtn_clicked()
{
    alanlariTemizle();
    ui->YeniBtn->setEnabled(true);
    ui->DuzenleBtn->setEnabled(true);
    ui->SilBtn->setEnabled(true);
    ui->StokKartlaritableView->setEnabled(true);
    ui->AraLineEdit->setEnabled(true);
    ui->araBtn->setEnabled(true);
    ui->StokGirBtn->setEnabled(true);
    ui->StokCikBtn->setEnabled(true);

    ui->BarkodLnEdit->setEnabled(false);
    ui->BarkodOlusturBtn->setEnabled(false);
    ui->StokKoduLnEdit->setEnabled(false);
    ui->StokAdiLnEdit->setEnabled(false);
    ui->BirimiComboBox->setEnabled(false);
    ui->MiktarLnEdit->setEnabled(false);
    ui->StokGrubuComboBox->setEnabled(false);
    ui->ureticicomboBox->setEnabled(false);
    ui->tedarikcicomboBox->setEnabled(false);
    ui->AFiyatdoubleSpinBox->setEnabled(false);
    ui->SFiyatdoubleSpinBox->setEnabled(false);
    ui->KDVspinBox->setEnabled(false);
    ui->OTVspinBox->setEnabled(false);
    ui->KDVcheckbox->setEnabled(false);
    ui->OTVcheckbox->setEnabled(false);
    ui->ResimEkleBtn->setEnabled(false);
    ui->ResimSilBtn->setEnabled(false);
    ui->SurekliYenicheckBox->setEnabled(false);
    ui->KaydetBtn->setEnabled(false);
    ui->IptalBtn->setEnabled(false);
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
    ui->StokKoduLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 2).data().toString());
    ui->StokAdiLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 3).data().toString());
    for (int i = 0; i <ui->BirimiComboBox->count(); ++i)
    {
        int result = QString::compare(ui->BirimiComboBox->itemText(i), seciliSatirModel->model()->index(seciliSatirIndex, 4).data().toString(), Qt::CaseInsensitive);
        if(result == 0)
        {
            ui->BirimiComboBox->setCurrentIndex(i);
            break;
        }
    }
    ui->MiktarLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 5).data().toString());
    for (int i = 0; i <ui->StokGrubuComboBox->count(); ++i)
    {
        int result = QString::compare(ui->StokGrubuComboBox->itemText(i), seciliSatirModel->model()->index(seciliSatirIndex, 6).data().toString(), Qt::CaseInsensitive);
        if(result == 0)
        {
            ui->StokGrubuComboBox->setCurrentIndex(i);
            break;
        }
    }
    ui->AFiyatdoubleSpinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 7).data().toDouble());
    ui->SFiyatdoubleSpinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 8).data().toDouble());
    ui->KDVspinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 9).data().toInt());
    ui->OTVspinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 10).data().toInt());
    ui->KDVcheckbox->setChecked(seciliSatirModel->model()->index(seciliSatirIndex, 11).data().toBool());
    ui->OTVcheckbox->setChecked(seciliSatirModel->model()->index(seciliSatirIndex, 12).data().toBool());
    ui->ureticicomboBox->setCurrentIndex(ui->ureticicomboBox->findText(seciliSatirModel->model()->index(seciliSatirIndex, 14).data().toString()));
    ui->tedarikcicomboBox->setCurrentIndex(ui->tedarikcicomboBox->findText(seciliSatirModel->model()->index(seciliSatirIndex, 15).data().toString()));
    ui->AciklamaLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 16).data().toString());
    ui->UrunResimlabel->setPixmap(vt->getStokKarti(seciliSatirModel->model()->index(seciliSatirIndex, 1).data().toString()).getResim());
}

void StokFrom::closeEvent(QCloseEvent *)
{

}

void StokFrom::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        if(ui->KaydetBtn->isEnabled() && ui->IptalBtn->isEnabled()){
            uyariSes.play();
            QMessageBox msg(this);
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
//        if(ui->stokKartBilGroupBox->isEnabled()){
//            emit on_KaydetBtn_clicked();
//        }
        if(ui->AraLineEdit->isEnabled()){
            emit on_araBtn_clicked();
        }
    }
}


void StokFrom::on_KaydetBtn_clicked()
{
    if(ui->SFiyatdoubleSpinBox->value() < ui->AFiyatdoubleSpinBox->value()){
        uyariSes.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Satış fiyatı alış fiyatından düşük olamaz");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        ui->SFiyatdoubleSpinBox->setFocus();
        ui->SFiyatdoubleSpinBox->selectAll();
        return;
    }
    if(yeniKayit){// true ise yeni stok kartı kaydı oluşturur.
        if(!vt->barkodVarmi(ui->BarkodLnEdit->text())){
            StokKarti yeniStokKarti = StokKarti();
            yeniStokKarti.setBarkod(ui->BarkodLnEdit->text());
            yeniStokKarti.setKod(ui->StokKoduLnEdit->text());
            yeniStokKarti.setAd(QLocale().toUpper(ui->StokAdiLnEdit->text()));
            yeniStokKarti.setBirim(ui->BirimiComboBox->currentText());
            yeniStokKarti.setMiktar(ui->MiktarLnEdit->text().toFloat());
            yeniStokKarti.setGrup(ui->StokGrubuComboBox->currentText());
            yeniStokKarti.setAFiyat(ui->AFiyatdoubleSpinBox->value());
            yeniStokKarti.setSFiyat(ui->SFiyatdoubleSpinBox->value());
            yeniStokKarti.setKdv(ui->KDVspinBox->value());
            yeniStokKarti.setOtv(ui->OTVspinBox->value());
            yeniStokKarti.setKdvdahil(ui->KDVcheckbox->isChecked());
            yeniStokKarti.setOtvdahil(ui->OTVcheckbox->isChecked());
            yeniStokKarti.setTarih(QDateTime::currentDateTime());
            yeniStokKarti.setUretici(ui->ureticicomboBox->currentText());
            yeniStokKarti.setTedarikci(ui->tedarikcicomboBox->currentText());
            yeniStokKarti.setAciklama(QLocale().toUpper("stok kartı oluşturuldu"));
            vt->yeniStokKartiOlustur(yeniStokKarti, &kullanici);

            alanlariTemizle();
            stokKartlariniListele();
            alanlariDoldur();
            ui->StokKartlaritableView->selectRow(seciliIndex);
            ui->AraLineEdit->setFocus();
            ui->AraLineEdit->selectAll();
            emit on_IptalBtn_clicked();
        }
        else{
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Bu barkod veritabanında başka bir stok kartına tanımlı!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
    else if(!yeniKayit){// yenikayit false ise mevcut stok kartını günceller.

        seciliSatirIndex = ui->StokKartlaritableView->currentIndex().row();
        seciliSatirModel = ui->StokKartlaritableView->selectionModel();
        QString duzenlenecekStokKartiID(seciliSatirModel->model()->index(seciliSatirIndex, 0).data().toString());
        StokKarti yeniStokKarti = StokKarti();
        yeniStokKarti.setId(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 0).data().toString());
        yeniStokKarti.setBarkod(ui->BarkodLnEdit->text());
        yeniStokKarti.setAd(QLocale().toUpper(ui->StokAdiLnEdit->text()));
        yeniStokKarti.setBirim(ui->BirimiComboBox->currentText());
        yeniStokKarti.setMiktar(ui->MiktarLnEdit->text().toFloat());
        yeniStokKarti.setGrup(ui->StokGrubuComboBox->currentText());
        yeniStokKarti.setAFiyat(ui->AFiyatdoubleSpinBox->value());
        yeniStokKarti.setSFiyat(ui->SFiyatdoubleSpinBox->value());
        yeniStokKarti.setKdv(ui->KDVspinBox->value());
        yeniStokKarti.setOtv(ui->OTVspinBox->value());
        yeniStokKarti.setKdvdahil(ui->KDVcheckbox->isChecked());
        yeniStokKarti.setOtvdahil(ui->OTVcheckbox->isChecked());
        yeniStokKarti.setTarih(QDateTime::currentDateTime());
        yeniStokKarti.setUretici(ui->ureticicomboBox->currentText());
        yeniStokKarti.setTedarikci(ui->tedarikcicomboBox->currentText());
        yeniStokKarti.setAciklama("stok kartı güncelleme");
        vt->stokKartiniGuncelle(duzenlenecekStokKartiID, yeniStokKarti, &kullanici);

        // düzenlenen stok kartı hızlı ürün butonlarına ekliyse ini dosyası ve buton bilgisini düzeltme başlangıcı
        vt->setHizliButon(yeniStokKarti);

        alanlariTemizle();
        stokKartlariniListele();
        alanlariDoldur();
        ui->StokKartlaritableView->selectRow(seciliIndex);
        ui->AraLineEdit->setFocus();
        ui->AraLineEdit->selectAll();
        emit on_IptalBtn_clicked();
    }
}


void StokFrom::on_SilBtn_clicked()
{
    if(ui->StokKartlaritableView->currentIndex().row() != -1){
        seciliSatirIndex = ui->StokKartlaritableView->currentIndex().row();
        seciliSatirModel = ui->StokKartlaritableView->selectionModel();
        uyariSes.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Question);
        msg.setText(QString("%1 barkod numaralı \n%2 isimli stok kartını silmek istediğinize emin misiniz?").arg(seciliSatirModel->model()->index(seciliSatirIndex, 1).data().toString()).arg(seciliSatirModel->model()->index(seciliSatirIndex, 2).data().toString()));
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        msg.setDefaultButton(QMessageBox::Yes);
        int cevap = msg.exec();
        if(cevap == QMessageBox::Yes){
            vt->stokKartiSil(seciliSatirModel->model()->index(seciliSatirIndex, 0).data().toString());
            stokKartlariniListele();
            emit on_IptalBtn_clicked();
        }
    }
    else{
        uyariSes.play();
        QMessageBox msg(this);
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

void StokFrom::stokKartiAra(QString aranacakMetin)
{
    bool varmi = false;
    if(ui->barkodRadioButton->isChecked()){
        for (int i = 0; i < ui->StokKartlaritableView->model()->rowCount(); ++i) {
            if(ui->StokKartlaritableView->model()->index(i, 1).data().toString() == aranacakMetin){
                ui->StokKartlaritableView->selectRow(i);
                varmi = true;
                break;
            }
        }
    }

    if(!varmi){
        uyariSes.play();
        QMessageBox msg(this);
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
    TopluStokYukleDialog *stokYukleForm = new TopluStokYukleDialog(this);
    stokYukleForm->exec();
    stokKartlariniListele();
    delete stokYukleForm;
}


void StokFrom::on_StokBirimBtn_clicked()
{
    BirimekleForm *birimForm = new BirimekleForm(this);
    birimForm->exec();
}


void StokFrom::on_SatisGrafikBtn_clicked()
{
    SatisGrafigiForm *grafikForm = new SatisGrafigiForm(this);
    grafikForm->exec();
}



void StokFrom::on_StokGirBtn_clicked()
{
    int seciliStokIndex = ui->StokKartlaritableView->currentIndex().row();
    StokGirCikDialog *stokMiktarigirForm = new StokGirCikDialog(this);
    stokMiktarigirForm->setKullanici(kullanici);
    stokMiktarigirForm->setWindowTitle("Stok Gir");
    stokMiktarigirForm->setIslem("GİRİŞ");
    stokMiktarigirForm->setStokKartiID(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 0).data().toString());
    stokMiktarigirForm->exec();
    stokKartlariniListele();
    delete stokMiktarigirForm;
    ui->StokKartlaritableView->selectRow(seciliStokIndex);
}


void StokFrom::on_StokCikBtn_clicked()
{
    int seciliStokIndex = ui->StokKartlaritableView->currentIndex().row();
    StokGirCikDialog *stokMiktarigirForm = new StokGirCikDialog(this);
    stokMiktarigirForm->setKullanici(kullanici);
    stokMiktarigirForm->setWindowTitle("Stok Çık");
    stokMiktarigirForm->setIslem("ÇIKIŞ");
    stokMiktarigirForm->setStokKartiID(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 0).data().toString());
    stokMiktarigirForm->exec();
    stokKartlariniListele();
    delete stokMiktarigirForm;
    ui->StokKartlaritableView->selectRow(seciliStokIndex);
}


void StokFrom::on_StokKartlaritableView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    StokHareketleriDialog *hareketlerForm = new StokHareketleriDialog(this);
    hareketlerForm->setStokBarkod(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString());
    hareketlerForm->exec();
    delete hareketlerForm;
}


void StokFrom::on_BarkodOlusturBtn_clicked()
{
    QString uretilenBarkod(QString::number(QRandomGenerator::global()->generate()));
    if(!vt->barkodVarmi(uretilenBarkod)){
        ui->BarkodLnEdit->setText(uretilenBarkod);
    }
}


void StokFrom::on_AraLineEdit_textChanged(const QString &arg1)
{
    if(ui->adRadioButton->isChecked()){
        for (int i = 0; i < ui->StokKartlaritableView->model()->rowCount(); ++i) {
            if(!ui->StokKartlaritableView->model()->index(i, 3).data().toString().contains(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(arg1), Qt::CaseInsensitive)){
                ui->StokKartlaritableView->hideRow(i);
            }
            else{
                ui->StokKartlaritableView->showRow(i);
            }
        }
    }
}


void StokFrom::on_ResimEkleBtn_clicked()
{
    ResimEkleDialog *resimEkleForm = new ResimEkleDialog(this);
    resimEkleForm->setUrunBarkod(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString());
    resimEkleForm->exec();
    ui->UrunResimlabel->setPixmap(vt->getStokKarti(seciliSatirModel->model()->index(seciliSatirIndex, 1).data().toString()).getResim());
}

void StokFrom::on_ResimSilBtn_clicked()
{
    if(!vt->getStokKarti(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString()).getResim().isNull()){
        uyariSes.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Ürün resmini silmek istediğinize emin misiniz?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        msg.setDefaultButton(QMessageBox::Yes);
        int cevap = msg.exec();
        if(cevap == QMessageBox::Yes){
            QFile resimDosya(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/urunler-image/" + ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString() + ".jpg");
            if(resimDosya.remove()){
                uyariSes.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Uyarı");
                msg.setIcon(QMessageBox::Information);
                msg.setText("Ürün resmi silindi.");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
                ui->UrunResimlabel->setPixmap(vt->getStokKarti(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString()).getResim());
            }
            else{
                uyariSes.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Uyarı");
                msg.setIcon(QMessageBox::Warning);
                msg.setText("Ürün resmi silinemedi.");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
            }
        }
    }
}

