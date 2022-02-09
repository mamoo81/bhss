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
    //veritabani ayarları okuma başlangıcı
    genelAyarlar.beginGroup("veritabani");
    ui->OtomatikYedekcheckBox->setChecked(genelAyarlar.value("otomatik").toBool());
    ui->Konumlabel->setText(genelAyarlar.value("yedekleme-konum").toString());
    ui->SiklikcomboBox->setCurrentText(genelAyarlar.value("periyod").toString());
    ui->SaatcomboBox->setCurrentText(genelAyarlar.value("saat").toString());
    genelAyarlar.endGroup();
    // veritabani ayarlari okuma bitiş
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
    genelAyarlar.beginGroup("veritabani");
    if(ui->OtomatikYedekcheckBox->isChecked() && !ui->Konumlabel->text().isEmpty()){
        genelAyarlar.setValue("otomatik", true);
        genelAyarlar.setValue("yedekleme-konum", ui->Konumlabel->text());
        genelAyarlar.setValue("periyod", ui->SiklikcomboBox->currentText());
        genelAyarlar.setValue("saat", ui->SaatcomboBox->currentText());
        genelAyarlar.endGroup();
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Veritabanının otomatik yedeklenmesi için yedekleneceği konumu belirtin!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        ui->tabWidget->setCurrentIndex(3);
        return;
    }
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
    QString dosyaYolu = QFileDialog::getSaveFileName(this, "Kayt Yeri Seçiniz", QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
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
    QMessageBox msg(this);
    msg.setWindowTitle("Dikkat");
    msg.setIcon(QMessageBox::Warning);
    msg.setText("Bu işlem sonucunda tüm işlemler veritabanı yedeğine geri dönecek\n\nYapmak istediğinize emin misiniz?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setButtonText(QMessageBox::Yes, "Evet");
    msg.setButtonText(QMessageBox::No, "Hayır");
    msg.setDefaultButton(QMessageBox::No);
    int cevap = msg.exec();
    if(cevap == QMessageBox::Yes){
        QFileDialog dialog(this);
        dialog.setWindowTitle("Veritabanı yedeğini seçiniz");
        dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
        dialog.setNameFilter(tr("Veritabanı dump dosyası (*.dump)"));
        dialog.setLabelText(QFileDialog::Accept, tr("Seç"));
        dialog.setLabelText(QFileDialog::Reject, tr("Vazgeç"));
        dialog.setLabelText(QFileDialog::FileName, tr("Dosya"));
        dialog.setLabelText(QFileDialog::FileType, tr("dosya tipi"));
        dialog.exec();
        QString dosyaYolu = dialog.selectedFiles().first();
        if(dosyaYolu.contains(".dump")){
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
        else{
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Veritabanı yedeği seçilmedi.\n\nİşlemden vazgeçildi!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
}


void AyarlarDialog::on_KonuSecpushButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Veritabanı yedeklenecek dizini seçin");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    dialog.setLabelText(QFileDialog::Accept, tr("Seç"));
    dialog.setLabelText(QFileDialog::Reject, tr("Vazgeç"));
    dialog.setLabelText(QFileDialog::FileName, tr("Dizin"));
    dialog.exec();
    ui->Konumlabel->setText(dialog.selectedFiles().first());
}

void AyarlarDialog::on_OtomatikYedekcheckBox_clicked()
{
    if(ui->OtomatikYedekcheckBox->isChecked()){
        ui->KonuSecpushButton->setEnabled(true);
        ui->SiklikcomboBox->setEnabled(true);
        ui->SaatcomboBox->setEnabled(true);
    }
    else{
        ui->KonuSecpushButton->setEnabled(false);
        ui->SiklikcomboBox->setEnabled(false);
        ui->SaatcomboBox->setEnabled(false);
    }
}


