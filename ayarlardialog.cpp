#include "ayarlardialog.h"
#include "ui_ayarlardialog.h"
#include "kullanicidialogform.h"
//**************************************
#include <QList>
#include <QSettings>
#include <QStandardPaths>
#include <QPrinterInfo>
#include <QFileDialog>
#include <QDebug>

AyarlarDialog::AyarlarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AyarlarDialog)
{
    ui->setupUi(this);

    formLoad();
}

AyarlarDialog::~AyarlarDialog()
{
    delete ui;
}

void AyarlarDialog::formLoad()
{
    ui->tabWidget->setCurrentIndex(0);
    // sistemdeki yazıcıların okunması
    QStringList yazicilar = QPrinterInfo::availablePrinterNames();
    ui->fisYazicisicomboBox->addItems(yazicilar);

    //genel ayarların okunması başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    //yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("fis-yazici");
    ui->herZamancheckBox->setChecked(genelAyarlar.value("herZaman").toBool());
    int yaziciIndexi = ui->fisYazicisicomboBox->findText(genelAyarlar.value("yazici").toString());
    ui->fisYazicisicomboBox->setCurrentIndex(yaziciIndexi);
    ui->SirketAdilineEdit->setText(genelAyarlar.value("sirketAdi").toString());
    ui->SirketAdreslineEdit->setText((genelAyarlar.value("sirketAdres").toString()));
    genelAyarlar.endGroup();
    //yazici ayarlari okuma bitiş

    //genel ayarların okunması bitiş
}

void AyarlarDialog::setCurrentUser(const User &newCurrentUser)
{
    if(newCurrentUser.getUserName() == "admin")
    {
        ui->tabWidget->setTabVisible(1, true);
        getUsers();
    }
    else{
        ui->tabWidget->setTabVisible(1, false);
    }
    currentUser = newCurrentUser;
}

void AyarlarDialog::getUsers()
{
    ui->KullanicilarlistWidget->clear();
    QList<QString> k = vt.GetUsers();
    foreach (QString user, k) {
        ui->KullanicilarlistWidget->addItem(user);
    }
}

void AyarlarDialog::on_YeniPushButton_clicked()
{
    KullaniciDialogForm *yeniKullaniciform = new KullaniciDialogForm(this);
    yeniKullaniciform->setYeniMi(true);
    yeniKullaniciform->exec();
    getUsers();
}


void AyarlarDialog::on_KullanicilarlistWidget_itemSelectionChanged()
{
    User u = vt.GetUserInfos(ui->KullanicilarlistWidget->currentItem()->text());
    ui->KasacheckBox->setChecked(u.getKasaYetki());
    ui->IadecheckBox->setChecked(u.getIadeYetki());
    ui->StokcheckBox->setChecked(u.getStokYetki());
}


void AyarlarDialog::on_DuzenlePushButton_clicked()
{
    if(ui->KullanicilarlistWidget->currentIndex().row() != -1){
        KullaniciDialogForm *yeniKullaniciform = new KullaniciDialogForm(this);
        yeniKullaniciform->setYeniMi(false);
        yeniKullaniciform->setDuzenlenecekUser(ui->KullanicilarlistWidget->currentItem()->text());
        yeniKullaniciform->exec();
        getUsers();
    }
    else{
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Listeden bir kullanıcı seçin.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}


void AyarlarDialog::on_SilPushButton_clicked()
{
    if(ui->KullanicilarlistWidget->currentIndex().row() != -1 && ui->KullanicilarlistWidget->currentItem()->text() != "admin"){
        vt.deleteUser(ui->KullanicilarlistWidget->currentItem()->text());
        getUsers();
    }
    else{
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Listeden bir kullanıcı seçin.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}


void AyarlarDialog::on_pushButton_clicked()
{
    // genel.ini dosyasına kayıt etme başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    // yazıcı ayarları kayıt başlangıç
    genelAyarlar.beginGroup("fis-yazici");
    if(ui->herZamancheckBox->isChecked()){
        genelAyarlar.setValue("herZaman", true);
    }
    else{
        genelAyarlar.setValue("herZaman", false);
    }
    genelAyarlar.setValue("yazici", ui->fisYazicisicomboBox->currentText());
    genelAyarlar.setValue("sirketAdi", ui->SirketAdilineEdit->text());
    genelAyarlar.setValue("sirketAdres", ui->SirketAdreslineEdit->text());
    genelAyarlar.endGroup();
    // yazıcı ayarları kayıt bitiş.
    // genel.ini dosyasına kayıt etme bitiş.




    this->close();
}


void AyarlarDialog::on_YedeklepushButton_clicked()
{
    QString dosyaYolu = QFileDialog::getExistingDirectory(this, "Kayt Yeri Seçiniz", QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    if(!dosyaYolu.isEmpty()){
        bool ok = vt.veritabaniYedekle(dosyaYolu);
        if(ok){
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Veritabanı Yedeklendi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        else{
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Veritabanı yedeklenemedi!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
}


void AyarlarDialog::on_GeriYuklepushButton_clicked()
{
    QString dosyaYolu = QFileDialog::getOpenFileName(this, "Veritabanı yedeğini seçiniz.",
                                                     QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                     tr("Veritabanı dump dpsyası (*.dump)"));
    if(!dosyaYolu.isEmpty()){
        bool ok = vt.veritabaniYedektenGeriYukle(dosyaYolu);
        if(ok){
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Veritabanı yedekten geri yüklendi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        else{
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Veritabanı yedekten geri yüklenemedi!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
}

