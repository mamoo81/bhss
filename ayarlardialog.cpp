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
#include "ayarlardialog.h"
#include "ui_ayarlardialog.h"
#include "kullanicidialogform.h"
//**************************************
#include <QList>
#include <QSettings>
#include <QStandardPaths>
#include <QPrinterInfo>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QProcess>
#include <QRegularExpression> // text dosyasında metin arama(karşılaştırma) için.
#include <QDir>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QSysInfo>

AyarlarDialog::AyarlarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AyarlarDialog)
{
    ui->setupUi(this);

    formLoad();
    uyariSesi.setSource(QUrl("qrc:/sounds/sounds/warning-sound.wav"));
}

AyarlarDialog::~AyarlarDialog()
{
    delete ui;
}

void AyarlarDialog::formLoad()
{
    ui->labelVersion->setText("0.2.3");

    // oto başlangıç okuma ayarını okuma
    // oto başlangıç için İşletim sistemi belirleme
    if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){
        // milis oto başlangıç wayfire.ini ayarlama
        QSettings wayfireini(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/wayfire.ini", QSettings::IniFormat);
        wayfireini.beginGroup("autostart");
        if(wayfireini.contains("mhss")){
            ui->otoMhsscheckBox->setChecked(true);
        }
        else{
            ui->otoMhsscheckBox->setChecked(false);
        }
        wayfireini.endGroup();
    }
    else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
        // pardus oto başlangıç için desktop dosyasının autostart dizini altına alınması
        if(QFileInfo(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/mhss.desktop").exists()){
            ui->otoMhsscheckBox->setChecked(true);
        }
        else{
            ui->otoMhsscheckBox->setChecked(false);
        }
    }

    //terazileri markalarını getirme
    ui->TeraziMarkacomboBox->addItems(vt.getTeraziler());
    // kullanılabilir seriportları getirme
    auto portlar = QSerialPortInfo::availablePorts();
    foreach (auto port, portlar) {
        ui->SeriPortcomboBox->addItem(port.portName());
    }

    ui->tabWidget->setCurrentIndex(0);
    ui->SiklikcomboBox->setCurrentIndex(0);
    ui->SaatcomboBox->setCurrentIndex(0);
    ui->dakikacomboBox->setCurrentIndex(0);

    SeriPortComboBoxDoldur();

    // sistemdeki yazıcıların okunması
    ui->fisYazicisicomboBox->addItems(QPrinterInfo::availablePrinterNames());
    ui->EtiketYazicicomboBox->addItems(QPrinterInfo::availablePrinterNames());

    //genel ayarların okunması başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    //veritabani ayarları okuma başlangıcı
    genelAyarlar.beginGroup("veritabani");
    ui->OtomatikYedekcheckBox->setChecked(genelAyarlar.value("otomatik").toBool());
    ui->Konumlabel->setText(genelAyarlar.value("yedekleme-konum").toString());
    ui->SiklikcomboBox->setCurrentText(genelAyarlar.value("periyod").toString());
    ui->SaatcomboBox->setCurrentIndex(genelAyarlar.value("saat").toInt());
    ui->dakikacomboBox->setCurrentIndex(genelAyarlar.value("dakika").toInt());
    genelAyarlar.endGroup();
    // veritabani ayarlari okuma bitiş

    //hizliürün tabpage sayfa adları okuma
    genelAyarlar.beginGroup("hizlisayfa");
    if(genelAyarlar.contains("0")){
        ui->sayfalineEdit1->setText(genelAyarlar.value("0").toString());
    }
    else{
        ui->sayfalineEdit1->setText("BARKODSUZ");
        genelAyarlar.setValue("0", "BARKODSUZ");
    }
    if(genelAyarlar.contains("1")){
        ui->sayfalineEdit2->setText(genelAyarlar.value("1").toString());
    }
    else{
        ui->sayfalineEdit2->setText("MANAV");
        genelAyarlar.setValue("1", "MANAV");
    }
    if(genelAyarlar.contains("2")){
        ui->sayfalineEdit3->setText(genelAyarlar.value("2").toString());
    }
    else{
        ui->sayfalineEdit3->setText("HIRDAVAT");
        genelAyarlar.setValue("2", "HIRDAVAT");
    }
    if(genelAyarlar.contains("3")){
        ui->sayfalineEdit4->setText(genelAyarlar.value("3").toString());
    }
    else{
        ui->sayfalineEdit4->setText("KASAP");
        genelAyarlar.setValue("3", "KASAP");
    }
    if(genelAyarlar.contains("4")){
        ui->sayfalineEdit5->setText(genelAyarlar.value("4").toString());
    }
    else{
        ui->sayfalineEdit5->setText("GENEL");
        genelAyarlar.setValue("4", "GENEL");
    }
    genelAyarlar.endGroup();

    //sepet kurtarma ayarları okuma başlangıç
    genelAyarlar.beginGroup("sepetKurtarma");
    ui->sepetKurtarmacheckBox->setChecked(genelAyarlar.value("kurtar").toBool());
    genelAyarlar.endGroup();

    //stok takibi ayarı okuma başlangıç
    genelAyarlar.beginGroup("stok");
    ui->StokTakibicheckBox->setChecked(genelAyarlar.value("takip").toBool());
    ui->StokMiktarspinBox->setValue(genelAyarlar.value("uyarimiktar").toInt());
    genelAyarlar.endGroup();
    //stok takibi ayarı okuma

    // uyari sesleri ayarları okuma başlangıç
    genelAyarlar.beginGroup("uyari-sesleri");
    ui->checkBoxSatisSesUyari->setChecked(genelAyarlar.value("odeme-alindi").toBool());
    genelAyarlar.endGroup();
    // uyarı sesleri okuma bitiş

    // fis yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("fis-yazici");
    ui->herZamancheckBox->setChecked(genelAyarlar.value("herZaman").toBool());
    ui->herZamanKasacheckBox->setChecked(genelAyarlar.value("raporHerZaman").toBool());
    ui->fisYazicisicomboBox->setCurrentIndex(ui->fisYazicisicomboBox->findText(genelAyarlar.value("yazici").toString()));
    ui->SirketAdilineEdit->setText(genelAyarlar.value("sirketAdi").toString());
    ui->SirketAdreslineEdit->setText((genelAyarlar.value("sirketAdres").toString()));
    genelAyarlar.endGroup();
    // fis yazici ayarlari okuma bitiş

    // etiket yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("etiket-yazici");
    ui->EtiketYazicicomboBox->setCurrentIndex(ui->EtiketYazicicomboBox->findText(genelAyarlar.value("yazici").toString()));
    ui->VarsayilanKagitcomboBox->setCurrentIndex(genelAyarlar.value("kagit").toInt());
    genelAyarlar.endGroup();
    // etiket yazıcı ayarları okuma bitiş

    // terazi ayarları okuma başlangıç
    genelAyarlar.beginGroup("terazi");
    ui->TeraziMarkacomboBox->setCurrentText(genelAyarlar.value("marka").toString());
    ui->TeraziModelcomboBox->setCurrentText(genelAyarlar.value("model").toString());
    ui->SeriPortcomboBox->setCurrentText(genelAyarlar.value("port").toString());
    ui->OtoOlcumcheckBox->setChecked(genelAyarlar.value("otomatik").toBool());
    ui->BaudRatecomboBox->setCurrentText(genelAyarlar.value("baudrate").toString());
    ui->DataBitscomboBox->setCurrentText(genelAyarlar.value("databits").toString());
    ui->ParitycomboBox->setCurrentText(genelAyarlar.value("parity").toString());
    ui->StopBitscomboBox->setCurrentText(genelAyarlar.value("stopbits").toString());
    ui->FlowControlcomboBox->setCurrentText(genelAyarlar.value("flowcontrol").toString());
    genelAyarlar.endGroup();

    //genel ayarların okunması bitiş
    VtYedeklemeButonlariniAyarla();
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
    ui->CaricheckBox->setChecked(u.getCariyetki());
    ui->AyarcheckBox->setChecked(u.getAyaryetki());
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
        uyariSesi.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Listeden bir kullanıcı seçin.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        // msg.setButtonText(QMessageBox::Ok, "Tamam");
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
        uyariSesi.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Listeden bir kullanıcı seçin.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        // msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}


void AyarlarDialog::on_pushButton_clicked()
{
    // oto başlama kayıt başlangıcı
    // oto kayıt için milis veya pardus İşletim Sistemi tespiti
    if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){
        // oto başlangıç wayfire.ini ayarlama
        QSettings wayfireini(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/wayfire.ini", QSettings::IniFormat);
        wayfireini.beginGroup("autostart");
        if(!wayfireini.contains("mhss") && ui->otoMhsscheckBox->isChecked()){
            wayfireini.setValue("mhss", "mhss");
        }
        else if(wayfireini.contains("mhss") && !ui->otoMhsscheckBox->isChecked()){
            wayfireini.remove("mhss");
        }
        wayfireini.endGroup();
    }
    else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
        if(ui->otoMhsscheckBox->isChecked()){
            if(!QFileInfo(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/mhss.desktop").exists()){
                QFile(":/dosyalar/mhss.desktop").copy(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/mhss.desktop");
            }
        }
        else{
            if(QFileInfo(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/mhss.desktop").exists()){
                QFile(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/mhss.desktop").remove();
            }
        }
    }

    // genel.ini dosyasına kayıt etme başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    // fiş yazıcı ayarları kayıt başlangıç
    genelAyarlar.beginGroup("fis-yazici");
    genelAyarlar.setValue("herZaman", ui->herZamancheckBox->isChecked());
    genelAyarlar.setValue("raporHerZaman", ui->herZamanKasacheckBox->isChecked());
    genelAyarlar.setValue("yazici", ui->fisYazicisicomboBox->currentText());
    genelAyarlar.setValue("sirketAdi", ui->SirketAdilineEdit->text());
    genelAyarlar.setValue("sirketAdres", ui->SirketAdreslineEdit->text());
    genelAyarlar.endGroup();
    // fiş yazıcı ayarları kayıt bitiş.

    // etiket yazıcı ayarkarı kayıt başlangıç
    genelAyarlar.beginGroup("etiket-yazici");
    genelAyarlar.setValue("yazici", ui->EtiketYazicicomboBox->currentText());
    genelAyarlar.setValue("kagit", ui->VarsayilanKagitcomboBox->currentIndex());
    genelAyarlar.endGroup();
    // etiket yazıcı ayarları kayıt bitiş.

    // terazi ayarları kayıt başlangıç
    genelAyarlar.beginGroup("terazi");
    genelAyarlar.setValue("marka", ui->TeraziMarkacomboBox->currentText());
    genelAyarlar.setValue("model", ui->TeraziModelcomboBox->currentText());
    genelAyarlar.setValue("port", ui->SeriPortcomboBox->currentText());
    genelAyarlar.setValue("otomatik", ui->OtoOlcumcheckBox->isChecked());
    genelAyarlar.setValue("baudrate", ui->BaudRatecomboBox->currentText());
    genelAyarlar.setValue("databits", ui->DataBitscomboBox->currentText());
    genelAyarlar.setValue("parity", ui->ParitycomboBox->currentText());
    genelAyarlar.setValue("stopbits", ui->StopBitscomboBox->currentText());
    genelAyarlar.setValue("flowcontrol", ui->FlowControlcomboBox->currentText());
    genelAyarlar.endGroup();
    // terazi ayarları kayıt bitiş.

    // sepet kurtarma kayıt başlangıç
    genelAyarlar.beginGroup("sepetKurtarma");
    genelAyarlar.setValue("kurtar", ui->sepetKurtarmacheckBox->isChecked());
    genelAyarlar.endGroup();

    //stok takibi ayarı kayıt başlangıç
    genelAyarlar.beginGroup("stok");
    genelAyarlar.setValue("takip", ui->StokTakibicheckBox->isChecked());
    genelAyarlar.setValue("uyarimiktar", ui->StokMiktarspinBox->value());
    genelAyarlar.endGroup();
    //stok takibi ayarı okuma bitiş

    // uyari sesleri kayıt başlangıç
    genelAyarlar.beginGroup("uyari-sesleri");
    genelAyarlar.setValue("odeme-alindi", ui->checkBoxSatisSesUyari->isChecked());
    genelAyarlar.endGroup();
    // uyari sesleri kayıt bitiş

    // hizli ürün sayfa adları kayıt başlangıç
    genelAyarlar.beginGroup("hizlisayfa");
    genelAyarlar.setValue("0", QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->sayfalineEdit1->text()));
    genelAyarlar.setValue("1", QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->sayfalineEdit2->text()));
    genelAyarlar.setValue("2", QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->sayfalineEdit3->text()));
    genelAyarlar.setValue("3", QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->sayfalineEdit4->text()));
    genelAyarlar.setValue("4", QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->sayfalineEdit5->text()));
    genelAyarlar.endGroup();
    // veritabanı otomatik yedekleme ayarları kaydetme başlangıç
    genelAyarlar.beginGroup("veritabani");
    if(!genelAyarlar.value("otomatik").toBool() && ui->OtomatikYedekcheckBox->isChecked()){// genel.ini'de false ve arayüzde checkbox true ise cronjob kaydet
        if(ui->OtomatikYedekcheckBox->isChecked() && !ui->Konumlabel->text().isEmpty()){
            genelAyarlar.setValue("otomatik", ui->OtomatikYedekcheckBox->isChecked());
            genelAyarlar.setValue("yedekleme-konum", ui->Konumlabel->text());
            genelAyarlar.setValue("periyod", ui->SiklikcomboBox->currentText());
            genelAyarlar.setValue("saat", ui->SaatcomboBox->currentIndex());
            genelAyarlar.setValue("dakika", ui->dakikacomboBox->currentIndex());
            genelAyarlar.endGroup();
        }
        else{
            uyariSesi.play();
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Veritabanının otomatik yedeklenmesi için yedekleneceği konumu belirtin!");
            msg.setStandardButtons(QMessageBox::Ok);
            // msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            ui->tabWidget->setCurrentIndex(3);
            return;
        }
        cronJobKaydet();
    }
    else if(genelAyarlar.value("otomatik").toBool() && !ui->OtomatikYedekcheckBox->isChecked()){// genel.ini'de true ve arayüzde checkbox false ise cronjob sil
        genelAyarlar.setValue("otomatik", ui->OtomatikYedekcheckBox->isChecked());
        genelAyarlar.setValue("yedekleme-konum", "");
        genelAyarlar.setValue("periyod", "");
        genelAyarlar.setValue("saat", "");
        genelAyarlar.setValue("dakika", "");
        genelAyarlar.endGroup();
        cronJobSil();
    }
    genelAyarlar.endGroup();
    // genel.ini dosyasına kayıt etme bitiş.
    this->close();
}


void AyarlarDialog::on_YedeklepushButton_clicked()
{
    QString dosyaYolu = QFileDialog::getSaveFileName(this, "Kayt Yeri Seçiniz", QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    if(!dosyaYolu.isEmpty()){
        if(vt.veritabaniYedekle(dosyaYolu)){
            uyariSesi.play();
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Veritabanı Yedeklendi.");
            msg.setStandardButtons(QMessageBox::Ok);
            // msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        else{
            uyariSesi.play();
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Veritabanı yedeklenemedi!");
            msg.setStandardButtons(QMessageBox::Ok);
            // msg.setButtonText(QMessageBox::Ok, "Tamam");
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
    // msg.setButtonText(QMessageBox::Yes, "Evet");
    // msg.setButtonText(QMessageBox::No, "Hayır");
    msg.setDefaultButton(QMessageBox::No);
    uyariSesi.play();
    int cevap = msg.exec();
    if(cevap == QMessageBox::Yes){
        QFileDialog dialog(this);
        dialog.setWindowTitle("Veritabanı yedeğini seçiniz");
        dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
        dialog.setNameFilter(tr("Veritabanı sql dosyası (*.sql)"));
        dialog.setLabelText(QFileDialog::Accept, tr("Seç"));
        dialog.setLabelText(QFileDialog::Reject, tr("Vazgeç"));
        dialog.setLabelText(QFileDialog::FileName, tr("Dosya"));
        dialog.setLabelText(QFileDialog::FileType, tr("dosya tipi"));
        dialog.exec();
        QString dosyaYolu = dialog.selectedFiles().first();
        if(dosyaYolu.contains(".sql")){
            if(vt.veritabaniYedektenGeriYukle(dosyaYolu)){
                uyariSesi.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Bilgi");
                msg.setIcon(QMessageBox::Information);
                msg.setText("Veritabanı yedekten geri yüklendi.");
                msg.setStandardButtons(QMessageBox::Ok);
                // msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
            }
            else{
                uyariSesi.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Uyarı");
                msg.setIcon(QMessageBox::Warning);
                msg.setText("Veritabanı yedekten geri yüklenemedi!");
                msg.setStandardButtons(QMessageBox::Ok);
                // msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
            }
        }
        else{
            uyariSesi.play();
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Veritabanı yedeği seçilmedi.\n\nİşlemden vazgeçildi!");
            msg.setStandardButtons(QMessageBox::Ok);
            // msg.setButtonText(QMessageBox::Ok, "Tamam");
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
    VtYedeklemeButonlariniAyarla();
}

void AyarlarDialog::VtYedeklemeButonlariniAyarla()
{
    if(ui->OtomatikYedekcheckBox->isChecked()){
        ui->KonuSecpushButton->setEnabled(true);
        ui->SiklikcomboBox->setEnabled(true);
        ui->SaatcomboBox->setEnabled(true);
        ui->dakikacomboBox->setEnabled(true);
    }
    else{
        ui->KonuSecpushButton->setEnabled(false);
        ui->SiklikcomboBox->setEnabled(false);
        ui->SaatcomboBox->setEnabled(false);
        ui->dakikacomboBox->setEnabled(false);
    }
}

void AyarlarDialog::cronJobKaydet()
{
    //yedekleme betiği kontrol ve oluşturma
    QFile betik(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/yedekle-betik.sh");
    betik.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner);
    betik.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);// truncate enumu dosyanın üzerine yazmak için. append dersen dosyaya ekler.
    QTextStream betikYazilacak(&betik);
    betikYazilacak << "#!/bin/bash" << Qt::endl;
    betikYazilacak << "/usr/bin/pg_dump -Fc -U postgres mhss_data > " << ui->Konumlabel->text() << "/\"mhss-data-\"`date +\"%d-%m-%Y.sql\"`" << Qt::endl;
    betik.close();
    //betiğe çalıştırılabilir dosya izni verme
    QString yetkilendirmeCMD = "chmod +x " + QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/yedekle-betik.sh";
    system(qPrintable(yetkilendirmeCMD));

    // kullanıcı adını alma
    QString userName = qgetenv("USER");
    if (userName.isEmpty()){
        userName = qgetenv("USERNAME");
    }
//    QFile cronMevcutDosya;
    if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){
        cronMevcutDosya = new QFile("/var/spool/cron/" + userName);
    }
    else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
        cronMevcutDosya = new QFile("/var/spool/cron/crontabs/" + userName);
    }
    QDir().mkdir("/tmp/mhss-cronjob/");// ilgili klasörün /tmp altına oluşturulması
    QFile cronGeciciDosya("/tmp/mhss-cronjob/" + userName);

    // crontab dosyası önceden oluşturulmuşsa /tmp/ altına al onu düzenle yoksa /tmp/ altında oluştur onu düzenle
    if(cronMevcutDosya->exists()){
        QDir().mkdir("/tmp/mhss-cronjob/");// ilgili klasörün /tmp altına oluşturulması
        cronMevcutDosya->copy("/tmp/mhss-cronjob/" + userName);
    }
    else{
        cronGeciciDosya.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner);
        cronGeciciDosya.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
        cronGeciciDosya.close();
    }

    // /tmp altına oluşturulan/kopyalanan crontab dosyasından varsa önceki görev silme
    QFile cronDosya("/tmp/mhss-cronjob/" + userName);
    cronDosya.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner);
    cronDosya.open(QIODevice::ReadWrite /*| QIODevice::Append */| QIODevice::Text);
    QTextStream in(&cronDosya);
    QStringList satirlar;
    // dosya okuyup QStringList içine atma
    {
        while (!in.atEnd()) {
            satirlar.push_back(in.readLine());
        }
        // listeden mhss satırlarını silme
        {
            int satirSayac = 0;
            foreach (QString satir, satirlar) {
                if(satir.contains("#MHSS")){
                    satirlar.removeAt(satirSayac);
                    satirlar.removeAt(satirSayac);// sonraki satiri siler. +1 yapmadım çünkü üstte sildiği için sonraki satir indexsi bir yukarı kaydı.
                    continue;
                }
                else if(satir.isEmpty()){
                    satirlar.removeAt(satirSayac);
                    continue;
                }
                satirSayac++;
            }
        }
    }
    cronDosya.close();

    // düzenlenen listeyi tekrar dosyaya yazma
    {
        cronDosya.open(QIODevice::WriteOnly | QIODevice::Text);
        cronDosya.resize(0);
        QTextStream in(&cronDosya);

        for (int var = 0; var < satirlar.size(); ++var) {
            in << satirlar[var] << Qt::endl;
        }
        cronDosya.close();
    }
    // /tmp altına oluşturulan/kopyalanan/düzenlenen crontab dosyasına görev ekleme
    cronDosya.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner);
    cronDosya.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    if(ui->SiklikcomboBox->currentText() == "Günlük"){
        in << "#MHSS" << Qt::endl;
        in << jobDakika << " " << jobSaati << " * * * ~/.config/mhss/yedekle-betik.sh > " << QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) << "/mhss/yedekle-betik.log 2>&1" << Qt::endl;
    }
    if(ui->SiklikcomboBox->currentText() == "Haftalık"){
        in << "#MHSS" << Qt::endl;
        in << jobDakika << " " << jobSaati << " * * 1 ~/.config/mhss/yedekle-betik.sh > " << QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) << "/mhss/yedekle-betik.log 2>&1" << Qt::endl;
    }
    if(ui->SiklikcomboBox->currentText() == "Aylık"){
        in << "#MHSS" << Qt::endl;
        in << jobDakika << " " << jobSaati << " 1 * * ~/.config/mhss/yedekle-betik.sh > " << QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) << "/mhss/yedekle-betik.log 2>&1" << Qt::endl;
    }
    cronDosya.close();

    // cronjob dosyasını /var/spool/cron/ altına aktarma

    //cronjob için pardus veya milis yetkili komut yürütme
    if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){
        QString cmd = "sudoui -c \"mv /tmp/mhss-cronjob/" + userName + " /var/spool/cron/\"";
        int exitCode = system(qPrintable(cmd));
        if(exitCode == QProcess::NormalExit){
            qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarıldı";
        }
        else{
            qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarılamadı";
        }
    }
    else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
        QString cmd = "pkexec sudo mv /tmp/mhss-cronjob/" + userName + " /var/spool/cron/crontabs/";
        int exitCode = system(qPrintable(cmd));
        if(exitCode == QProcess::NormalExit){
            qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarıldı";
        }
        else{
            qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarılamadı";
        }
    }
}

void AyarlarDialog::cronJobSil()
{
    // kullanıcı adını alma
    QString userName = qgetenv("USER");
    if (userName.isEmpty()){
        userName = qgetenv("USERNAME");
    }
    if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){
        cronMevcutDosya = new QFile("/var/spool/cron/" + userName);
    }
    else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
        cronMevcutDosya = new QFile("/var/spool/cron/crontabs/" + userName);
    }
    // crontab dosyası önceden oluşturulmuşsa /tmp/ altına al onu düzenle yoksa /tmp/ altında oluştur onu düzenle
    if(cronMevcutDosya->exists()){
        QDir().mkdir("/tmp/mhss-cronjob/");// ilgili klasörün /tmp altına oluşturulması
        cronMevcutDosya->copy("/tmp/mhss-cronjob/" + userName);
    }
    // /tmp altına oluşturulan/kopyalanan crontab dosyasından varsa önceki görev silme
    QFile cronDosya("/tmp/mhss-cronjob/" + userName);
    cronDosya.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner);
    cronDosya.open(QIODevice::ReadWrite /*| QIODevice::Append */| QIODevice::Text);
    QTextStream in(&cronDosya);
    QStringList satirlar;
    // dosya okuyup QStringList içine atma
    {
        while (!in.atEnd()) {
            satirlar.push_back(in.readLine());
        }
        // listeden mhss satırlarını silme
        {
            int satirSayac = 0;
            foreach (QString satir, satirlar) {
                if(satir.contains("#MHSS")){
                    satirlar.removeAt(satirSayac);
                    satirlar.removeAt(satirSayac);// sonraki satiri siler. +1 yapmadım çünkü üstte sildiği için sonraki satir indexsi bir yukarı kaydı.
                    continue;
                }
                else if(satir.isEmpty()){
                    satirlar.removeAt(satirSayac);
                    continue;
                }
                satirSayac++;
            }
        }
    }
    cronDosya.close();
    // düzenlenen listeyi tekrar dosyaya yazma
    {
        cronDosya.open(QIODevice::WriteOnly | QIODevice::Text);
        cronDosya.resize(0);
        QTextStream in(&cronDosya);

        for (int var = 0; var < satirlar.size(); ++var) {
            in << satirlar[var] << Qt::endl;
        }
        cronDosya.close();
    }

    // cronjob dosyasını /var/spool/cron/ altına aktarma
    //cronjob için pardus veya milis yetkili komut yürütme
    if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){
        QString cmd = "sudoui -c \"mv /tmp/mhss-cronjob/" + userName + " /var/spool/cron/\"";
        int exitCode = system(qPrintable(cmd));
        if(exitCode == QProcess::NormalExit){
            qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarıldı";
        }
        else{
            qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarılamadı";
        }
    }
    else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
        QString cmd = "pkexec sudo mv /tmp/mhss-cronjob/" + userName + " /var/spool/cron/crontabs/";
        int exitCode = system(qPrintable(cmd));
        if(exitCode == QProcess::NormalExit){
            qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarıldı";
        }
        else{
            qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarılamadı";
        }
    }
}


void AyarlarDialog::on_SiklikcomboBox_currentIndexChanged(const QString &arg1)
{
    periyod = arg1;
}


void AyarlarDialog::on_SaatcomboBox_currentIndexChanged(int index)
{
    jobSaati = QString::number(index);
}



void AyarlarDialog::on_dakikacomboBox_currentIndexChanged(int index)
{
    jobDakika = QString::number(index);
}


void AyarlarDialog::on_SifirlapushButton_clicked()
{
    uyariSesi.play();
    QMessageBox msg(this);
    msg.setWindowTitle("Dikkat");
    msg.setIcon(QMessageBox::Warning);
    msg.setText("Dikkat veritabanındaki tüm veriler sıfırlanacak! Sıfırlamak istediğinize emin misiniz?\n\n"
                "Silinecekler:\n"
                "Stok kartları ve Stok Hareketleri\n"
                "Kasa ve Kasa hareketleri\n"
                "Cari Kartlar ve Cari Hareketleri\n"
                "Kullanıcılar");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    // msg.setButtonText(QMessageBox::Yes, "Evet");
    // msg.setButtonText(QMessageBox::No, "Hayır");
    msg.setDefaultButton(QMessageBox::No);
    int cevap = msg.exec();
    if(cevap == QMessageBox::Yes){
        if(vt.veritabaniSifirla()){
            uyariSesi.play();
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Veritabanı sıfırlandı.");
            msg.setStandardButtons(QMessageBox::Ok);
            // msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        else{
            uyariSesi.play();
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Veritabanı sıfırlanamadı.");
            msg.setStandardButtons(QMessageBox::Ok);
            // msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
}


void AyarlarDialog::on_kapatpushButton_clicked()
{
    this->close();
}


void AyarlarDialog::on_hizliButtonSifirlapushButton_clicked()
{
    QMessageBox msg(this);
    msg.setWindowTitle("Dikkat");
    msg.setIcon(QMessageBox::Warning);
    msg.setText("Hızlı ürün butonlarını sıfırlamak istediğinize emin misiniz?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    // msg.setButtonText(QMessageBox::Yes, "Evet");
    // msg.setButtonText(QMessageBox::No, "Hayır");
    msg.setDefaultButton(QMessageBox::No);
    uyariSesi.play();
    int ret = msg.exec();
    if(ret == QMessageBox::Yes){
        QSettings hizlibutonlarini(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/hizlibutonlar.ini", QSettings::IniFormat);
        foreach (auto buton, hizlibutonlarini.childGroups()) {
            hizlibutonlarini.remove(buton);
        }
    }
}


void AyarlarDialog::on_TeraziMarkacomboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    ui->TeraziModelcomboBox->clear();
    ui->TeraziModelcomboBox->addItems(vt.getTeraziModeller(ui->TeraziMarkacomboBox->currentText()));
}

void AyarlarDialog::SeriPortComboBoxDoldur()
{
    QMap<QString, QSerialPort::BaudRate> *BaudRates = new QMap<QString, QSerialPort::BaudRate>;
    BaudRates->insert("1200", QSerialPort::Baud1200);
    BaudRates->insert("2400", QSerialPort::Baud2400);
    BaudRates->insert("4800", QSerialPort::Baud4800);
    BaudRates->insert("9600", QSerialPort::Baud9600);
    BaudRates->insert("19200", QSerialPort::Baud19200);
    BaudRates->insert("38400", QSerialPort::Baud38400);
    BaudRates->insert("57600", QSerialPort::Baud57600);
    BaudRates->insert("115200", QSerialPort::Baud115200);
    ui->BaudRatecomboBox->addItems(QStringList(BaudRates->keys()));

    QMap<QString, QSerialPort::DataBits> *DataBits = new QMap<QString, QSerialPort::DataBits>;
    DataBits->insert("Data5", QSerialPort::Data5);
    DataBits->insert("Data6", QSerialPort::Data6);
    DataBits->insert("Data7", QSerialPort::Data7);
    DataBits->insert("Data8", QSerialPort::Data8);
    ui->DataBitscomboBox->addItems(QStringList(DataBits->keys()));

    QMap<QString, QSerialPort::Parity> *Parity = new QMap<QString, QSerialPort::Parity>;
    Parity->insert("NoParity", QSerialPort::NoParity);
    Parity->insert("EvenParity", QSerialPort::EvenParity);
    Parity->insert("OddParity", QSerialPort::OddParity);
    Parity->insert("SpaceParity", QSerialPort::SpaceParity);
    Parity->insert("MarkParity", QSerialPort::MarkParity);
    ui->ParitycomboBox->addItems(QStringList(Parity->keys()));

    QMap<QString, QSerialPort::StopBits> *StopBits = new QMap<QString, QSerialPort::StopBits>;
    StopBits->insert("OneStop", QSerialPort::OneStop);
    StopBits->insert("OneAndHalfStop", QSerialPort::OneAndHalfStop);
    StopBits->insert("TwoStop", QSerialPort::TwoStop);
    ui->StopBitscomboBox->addItems(QStringList(StopBits->keys()));

    QMap<QString, QSerialPort::FlowControl> *FlowControl = new QMap<QString, QSerialPort::FlowControl>;
    FlowControl->insert("NoFlowControl", QSerialPort::NoFlowControl);
    FlowControl->insert("HardwareControl", QSerialPort::HardwareControl);
    FlowControl->insert("SoftwareControl", QSerialPort::SoftwareControl);
    ui->FlowControlcomboBox->addItems(QStringList(FlowControl->keys()));
}


void AyarlarDialog::on_SekmeVarsayilanpushButton_clicked()
{
    ui->sayfalineEdit1->setText("BARKODSUZ");
    ui->sayfalineEdit2->setText("MANAV");
    ui->sayfalineEdit3->setText("HIRDAVAT");
    ui->sayfalineEdit4->setText("KASAP");
    ui->sayfalineEdit5->setText("GENEL");
}


void AyarlarDialog::on_StokTakibicheckBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    if(ui->StokTakibicheckBox->isChecked()){
        ui->StokMiktarspinBox->setEnabled(true);
        ui->labelStokMiktar->setEnabled(true);
    }
    else{
        ui->StokMiktarspinBox->setEnabled(false);
        ui->labelStokMiktar->setEnabled(false);
    }
}


void AyarlarDialog::on_YonelimcomboBox_currentIndexChanged(int index)
{
    ui->VarsayilanKagitcomboBox->clear();
    switch (index) {
    case 0:
        ui->VarsayilanKagitcomboBox->addItem("38x80mm Termal Karton Etiket");
        break;
    case 1:
        ui->VarsayilanKagitcomboBox->addItem("38x100mm Termal Karton Etiket");
        break;
    }
}

