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
#include <QProcess>
#include <QRegularExpression> // text dosyasında metin arama(karşılaştırma) için.
#include <QDir>

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
    ui->SiklikcomboBox->setCurrentIndex(0);
    ui->SaatcomboBox->setCurrentIndex(0);
    ui->dakikacomboBox->setCurrentIndex(0);
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
    ui->SaatcomboBox->setCurrentIndex(genelAyarlar.value("saat").toInt());
    ui->dakikacomboBox->setCurrentIndex(genelAyarlar.value("dakika").toInt());
    genelAyarlar.endGroup();
    // veritabani ayarlari okuma bitiş
    //hizliürün tabpage sayfa adları okuma
    genelAyarlar.beginGroup("hizlisayfa");
    ui->sayfalineEdit1->setText(genelAyarlar.value("0").toString());
    ui->sayfalineEdit2->setText(genelAyarlar.value("1").toString());
    ui->sayfalineEdit3->setText(genelAyarlar.value("2").toString());
    ui->sayfalineEdit4->setText(genelAyarlar.value("3").toString());
    ui->sayfalineEdit5->setText(genelAyarlar.value("4").toString());
    genelAyarlar.endGroup();
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
        genelAyarlar.endGroup();
    }
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
    if(!betik.exists()){
        betik.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner);
        betik.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
        QTextStream betikYazilacak(&betik);
        betikYazilacak << "#!/bin/bash" << Qt::endl;
        betikYazilacak << "/usr/bin/pg_dump -Fc -U postgres mhss_data > " << ui->Konumlabel->text() << "/\"mhss-data-\"`date +\"%d-%m-%Y.dump\"`" << Qt::endl;
        betik.close();
        //betiğe çalıştırılabilir dosya izni verme
        QString yetkilendirmeCMD = "chmod +x " + QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/yedekle-betik.sh";
        system(qPrintable(yetkilendirmeCMD));
    }

    // kullanıcı adını alma
    QString userName = qgetenv("USER");
        if (userName.isEmpty()){
            userName = qgetenv("USERNAME");
        }
    QFile cronMevcutDosya("/var/spool/cron/" + userName);
    QDir().mkdir("/tmp/mhss-cronjob/");// ilgili klasörün /tmp altına oluşturulması
    QFile cronGeciciDosya("/tmp/mhss-cronjob/" + userName);

    // crontab dosyası önceden oluşturulmuşsa /tmp/ altına al onu düzenle yoksa /tmp/ altında oluştur onu düzenle
    if(cronMevcutDosya.exists()){
        QDir().mkdir("/tmp/mhss-cronjob/");// ilgili klasörün /tmp altına oluşturulması
        cronMevcutDosya.copy("/tmp/mhss-cronjob/" + userName);
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
    QString cmd = "sudoui -c \"mv /tmp/mhss-cronjob/" + userName + " /var/spool/cron/\"";
    int exitCode = system(qPrintable(cmd));
    if(exitCode == QProcess::NormalExit){
        qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarıldı";
    }
    else{
        qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarılamadı";
    }
}

void AyarlarDialog::cronJobSil()
{
    // kullanıcı adını alma
    QString userName = qgetenv("USER");
        if (userName.isEmpty()){
            userName = qgetenv("USERNAME");
        }
    QFile cronMevcutDosya("/var/spool/cron/" + userName);
    // crontab dosyası önceden oluşturulmuşsa /tmp/ altına al onu düzenle yoksa /tmp/ altında oluştur onu düzenle
    if(cronMevcutDosya.exists()){
        QDir().mkdir("/tmp/mhss-cronjob/");// ilgili klasörün /tmp altına oluşturulması
        cronMevcutDosya.copy("/tmp/mhss-cronjob/" + userName);
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
    QString cmd = "sudoui -c \"mv /tmp/mhss-cronjob/" + userName + " /var/spool/cron/\"";
    int exitCode = system(qPrintable(cmd));
    if(exitCode == QProcess::NormalExit){
        qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarıldı";
    }
    else{
        qDebug() << Qt::endl << "Mesaj: cronjob /var/spool/cron/ altına aktarılamadı";
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
    msg.setButtonText(QMessageBox::Yes, "Evet");
    msg.setButtonText(QMessageBox::No, "Hayır");
    msg.setDefaultButton(QMessageBox::No);
    int cevap = msg.exec();
    if(cevap == QMessageBox::Yes){
        bool ok = vt.veritabaniSifirla();
        if(ok){
            QMessageBox msg(this);
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Veritabanı sıfırlandı.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        else{
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Veritabanı sıfırlanamadı.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
}

