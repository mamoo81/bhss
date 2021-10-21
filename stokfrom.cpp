#include "stokfrom.h"
#include "ui_stokfrom.h"
#include "stokgrupform.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QDateTime>
#include <QModelIndex>

QSqlDatabase db_stok;
QSqlQuery sorgu_stok;
QSqlQueryModel *sorgu_model;
QItemSelectionModel *seciliSatirModel;
int seciliSatirIndex;
//QModelIndex index;

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
    stkgrupform->exec();
}

void StokFrom::formLoad()
{
    db_stok = QSqlDatabase::addDatabase("QPSQL","db_stok");
    db_stok.setDatabaseName("mhss_data");
    db_stok.setHostName("localhost");
    db_stok.setUserName("admin");
    db_stok.setPassword("admin");
    if(!db_stok.isOpen()){
        db_stok.open();
        StokFrom::grupComboboxDoldur();
        StokFrom::stokKartlariniListele();
    }

    connect(ui->StokKartlaritableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),SLOT(alanlariDoldur()));

    QRegExp rgx("(|\"|/|\\.|[0-9]){13}");// lineEdit'e sadece rakam girmesi için QRegExp tanımlaması.
    ui->BarkodLnEdit->setValidator(new QRegExpValidator(rgx, this));// setValidator'üne QRegExpValidator'ü belirtme.

    ui->AraLineEdit->setFocus();
}

void StokFrom::grupComboboxDoldur()
{
    sorgu_stok = QSqlQuery("SELECT grup FROM stokgruplari", db_stok);
    while (sorgu_stok.next()) {
        ui->StokGrubuComboBox->addItem(sorgu_stok.value(0).toString());
    }
}

void StokFrom::tableViewAyarla()
{
    sorgu_model->setHeaderData(0,Qt::Horizontal,"Stok ID");
    sorgu_model->setHeaderData(1,Qt::Horizontal,"Barkod");
    sorgu_model->setHeaderData(2,Qt::Horizontal,"Adı");
    sorgu_model->setHeaderData(3,Qt::Horizontal,"Birim");
    sorgu_model->setHeaderData(4,Qt::Horizontal,"Miktar");
    sorgu_model->setHeaderData(5,Qt::Horizontal,"Stok Grup");
    sorgu_model->setHeaderData(6,Qt::Horizontal,"Alış Fiyat");
    sorgu_model->setHeaderData(7,Qt::Horizontal,"Satış Fiyat");
    sorgu_model->setHeaderData(11,Qt::Horizontal,"Gün. tarihi");
    sorgu_model->setHeaderData(12,Qt::Horizontal,"Açıklama");
}

void StokFrom::stokKartlariniListele()
{
    sorgu_model = new QSqlQueryModel();
    sorgu_stok = QSqlQuery(db_stok);
    sorgu_stok.exec("SELECT stokid, barkod, ad, birim, miktar, stokgrup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv1, kdv2, kdv3, songuntarih, aciklama FROM stokkartlari");
    sorgu_model->setQuery(sorgu_stok);
    tableViewAyarla();
    ui->StokKartlaritableView->setModel(sorgu_model);
    ui->StokKartiAdetLabel->setText(QString::number(ui->StokKartlaritableView->model()->rowCount()));
    QItemSelectionModel *selectionModel = ui->StokKartlaritableView->selectionModel();
    QModelIndex modelindex = ui->StokKartlaritableView->model()->index(0, 0);
    selectionModel->select(modelindex, QItemSelectionModel::Clear);

}


void StokFrom::on_YeniBtn_clicked()
{
    if(!ui->stokKartBilGroupBox->isEnabled())
    {
        ui->stokKartBilGroupBox->setEnabled(true);
        ui->DuzenleBtn->setEnabled(false);
        ui->SilBtn->setEnabled(false);
        ui->StokKartlaritableView->setEnabled(false);
        ui->BarkodLnEdit->setFocus();
    }
}


void StokFrom::on_DuzenleBtn_clicked()
{
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
            ui->BarkodLnEdit->setFocus();
        }
        else
        {
            QMessageBox::information(this, "Uyarı", "Bir stok kartı seçiniz.",QMessageBox::Ok);
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
    ui->AciklamaLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 12).data().toString());
}

void StokFrom::closeEvent(QCloseEvent *)
{
    if(db_stok.isOpen()){
        db_stok.close();
        db_stok.removeDatabase("QPSQL");
    }
}

void StokFrom::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        if(ui->stokKartBilGroupBox->isEnabled()){
            QMessageBox::StandardButton cevap = QMessageBox::question(this, "Uyarı", "Stok kartını düzenlemeyi bitirmediniz. Yinede çıkmak istiyormusunuz?", QMessageBox::Yes | QMessageBox::No);
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
        else{
            emit on_araBtn_clicked();
        }
    }
}


void StokFrom::on_KaydetBtn_clicked()
{
    if(ui->YeniBtn->isEnabled()){// yenibtn aktifse yeni stok kartı kaydı oluşturur.
        sorgu_stok = QSqlQuery(db_stok);
        sorgu_stok.prepare("SELECT barkod FROM stokkartlari WHERE barkod = ?");
        sorgu_stok.bindValue(0, ui->BarkodLnEdit->text());
        sorgu_stok.exec();
        if(!sorgu_stok.next())
        {
            sorgu_stok.prepare("INSERT INTO stokkartlari (stokID, barkod, ad, birim, miktar, stokGrup, aFiyat, sFiyat, kdv1, kdv2, kdv3, songuntarih, aciklama) "
            "VALUES (nextval('stokID_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            sorgu_stok.bindValue(0, ui->BarkodLnEdit->text());
            sorgu_stok.bindValue(1, ui->StokAdiLnEdit->text());
            sorgu_stok.bindValue(2, ui->BirimiComboBox->currentText());
            sorgu_stok.bindValue(3, ui->MiktarLnEdit->text().toFloat());
            sorgu_stok.bindValue(4, ui->StokGrubuComboBox->currentText());
            sorgu_stok.bindValue(5, ui->AFiyatdoubleSpinBox->value());
            sorgu_stok.bindValue(6, ui->SFiyatdoubleSpinBox->value());
            sorgu_stok.bindValue(7, 0);
            sorgu_stok.bindValue(8, 0);
            sorgu_stok.bindValue(9, 0);
            sorgu_stok.bindValue(10, QDateTime::currentDateTime());
            sorgu_stok.bindValue(11, "stok kartı oluşturuldu");// buraya ilerde giriş yapan kullanıcı adını ekle.
            if(sorgu_stok.exec())
            {
                QMessageBox::information(this, "Uyarı", "Stok kartı başarıyla oluşturuldu.", QMessageBox::Ok);
                stokKartlariniListele();
                alanlariTemizle();
                emit on_IptalBtn_clicked();
            }
        }
        else
        {
            QMessageBox::critical(this, "Hata", "Girdiğiniz barkod veritabanında zaten mevcut.", QMessageBox::Ok);
        }
    }
    else if(ui->DuzenleBtn->isEnabled()){// duzenlebtn aktifse mevcut stok kartını günceller.
        seciliSatirIndex = ui->StokKartlaritableView->currentIndex().row();
        seciliSatirModel = ui->StokKartlaritableView->selectionModel();
        QString duzenlenecekStokId = seciliSatirModel->model()->index(seciliSatirIndex, 0).data().toString();
        sorgu_stok = QSqlQuery(db_stok);
        sorgu_stok.prepare("UPDATE stokkartlari SET (barkod, ad, birim, miktar, stokgrup, afiyat, sfiyat, kdv1, kdv2, kdv3, songuntarih, aciklama)"
                            "=(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) "
                            "WHERE stokid = ?");
        sorgu_stok.bindValue(0, ui->BarkodLnEdit->text());
        sorgu_stok.bindValue(1, ui->StokAdiLnEdit->text());
        sorgu_stok.bindValue(2, ui->BirimiComboBox->currentText());
        sorgu_stok.bindValue(3, ui->MiktarLnEdit->text().toFloat());
        sorgu_stok.bindValue(4, ui->StokGrubuComboBox->currentText());
        sorgu_stok.bindValue(5, ui->AFiyatdoubleSpinBox->value());
        sorgu_stok.bindValue(6, ui->SFiyatdoubleSpinBox->value());
        sorgu_stok.bindValue(7, 0);
        sorgu_stok.bindValue(8, 0);
        sorgu_stok.bindValue(9, 0);
        sorgu_stok.bindValue(10, QDateTime::currentDateTime());
        sorgu_stok.bindValue(11, "stok kartı güncelleme: ");// buraya ileride kullanıcı adını ekle.
        sorgu_stok.bindValue(12, duzenlenecekStokId);
        if(sorgu_stok.exec()){
            QMessageBox::information(this, "Uyarı", "Stok kartı güncellendi.", QMessageBox::Ok);
            StokFrom::alanlariTemizle();
            StokFrom::stokKartlariniListele();
            emit on_IptalBtn_clicked();
        }
        else{
            QMessageBox::warning(this, "Hata", "Stok kartı güncellenemedi!", QMessageBox::Ok);
        }
    }
}


void StokFrom::on_SilBtn_clicked()
{
    if(ui->StokKartlaritableView->currentIndex().row() != -1){
        seciliSatirIndex = ui->StokKartlaritableView->currentIndex().row();
        seciliSatirModel = ui->StokKartlaritableView->selectionModel();
        QString mesajMetni = seciliSatirModel->model()->index(seciliSatirIndex, 1).data().toString() + " nolu\n" + seciliSatirModel->model()->index(seciliSatirIndex, 2).data().toString() + " isimli stok kartını silmek istediğinize emin misiniz?";
        QMessageBox::StandardButton cevap = QMessageBox::question(this, "Dikkat", mesajMetni, QMessageBox::Yes | QMessageBox::No);
        if(cevap == QMessageBox::Yes){
            sorgu_stok = QSqlQuery(db_stok);
            sorgu_stok.prepare("DELETE FROM stokkartlari WHERE barkod = ?");
            sorgu_stok.bindValue(0, seciliSatirModel->model()->index(seciliSatirIndex, 1).data().toString());
            if(sorgu_stok.exec()){
                QMessageBox::information(this, "Uyarı", "Stok kartı silindi.", QMessageBox::Ok);
                StokFrom::stokKartlariniListele();
            }
        }
    }
    else{
        QMessageBox::information(this, "Uyarı", "Bir stok kartı seçiniz.", QMessageBox::Ok);
    }
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
        QMessageBox::warning(this, "Uyarı", "Stok kartı bulunamadı.", QMessageBox::Ok);
        ui->StokKartlaritableView->clearSelection();
        StokFrom::alanlariTemizle();
    }
}


void StokFrom::on_araBtn_clicked()
{
    stokKartiAra(ui->AraLineEdit->text());
}



