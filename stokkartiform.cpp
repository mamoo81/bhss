#include "stokhareketleridialog.h"
#include "stokkartiform.h"
#include "ui_stokkartiform.h"

#include <QSqlError>
#include <QRandomGenerator>

StokKartiForm::StokKartiForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StokKartiForm)
{
    ui->setupUi(this);
}

StokKartiForm::~StokKartiForm()
{
    delete ui;
}

void StokKartiForm::setKart(StokKarti newYeniKart)
{
    kart = newYeniKart;
    ui->BarkodlineEdit->setText(kart.getBarkod());
    ui->StokKodlineEdit->setText(kart.getKod());
    ui->AdlineEdit->setText(kart.getAd());
    ui->UreticicomboBox->setCurrentIndex(ui->UreticicomboBox->findText(kart.getUretici()));
    ui->BirimicomboBox->setCurrentIndex(kart.getBirim());
    ui->MiktarlineEdit->setText(QString::number(kart.getMiktar(), 'f', 3));
    ui->StokGrubucomboBox->setCurrentIndex(kart.getGrup());
    ui->AFiyatdoubleSpinBox->setValue(kart.getAFiyat());
    ui->SFiyatdoubleSpinBox->setValue(kart.getSFiyat());
    ui->KDVspinBox->setValue(kart.getKdv());
    if(kart.getKdvdahil()){
        ui->KDVDahilcomboBox->setCurrentIndex(1);
    }
    else{
        ui->KDVDahilcomboBox->setCurrentIndex(0);
    }
    ui->OTVspinBox->setValue(kart.getOtv());
    if(kart.getOtvdahil()){
        ui->OTVDahilcomboBox->setCurrentIndex(1);
    }
    else{
        ui->OTVDahilcomboBox->setCurrentIndex(0);
    }
    ui->TedarikcicomboBox->setCurrentIndex(0);
    ui->BarkodlineEdit->setFocus();
}

void StokKartiForm::FormLoad()
{
    ui->UreticicomboBox->addItems(vt.getUreticiler());
    ui->TedarikcicomboBox->addItems(vt.getTedarikciler());
    ui->BirimicomboBox->addItems(vt.getStokBirimleri());
    ui->StokGrubucomboBox->addItems(vt.stokGruplariGetir());

    if(toplustokkarti){
        ui->iptalpushButton->setEnabled(true);
    }
    else{
        ui->iptalpushButton->setEnabled(false);
    }

    if(yeniKart){
        ui->stokHareketleripushButton->setEnabled(false);
    }
    else{
        ui->stokHareketleripushButton->setEnabled(true);
    }

    regEXPbarkod = QRegExp("[0-9]{8,13}");
    ui->BarkodlineEdit->setValidator(new QRegExpValidator(regEXPbarkod, this));

    regEXPstokKod = QRegExp("[a-zöçşıiğüA-ZÖÇŞIİĞÜ0-9]{3,}");
    ui->StokKodlineEdit->setValidator(new QRegExpValidator(regEXPstokKod, this));

    regEXPstokAd = QRegExp("[a-zöçşıiğü- A-ZÖÇŞIİĞÜ- 0-9.]{3,}");
    ui->AdlineEdit->setValidator(new QRegExpValidator(regEXPstokAd, this));

    regEXPstokMiktar = new QDoubleValidator(0, 999999999, 10, this);
    regEXPstokMiktar->setNotation(QDoubleValidator::StandardNotation);
    regEXPstokMiktar->setLocale(QLocale::C);
    ui->MiktarlineEdit->setValidator(regEXPstokMiktar);

    TextColorPaletteDefault = ui->BarkodlineEdit->style()->standardPalette();

    TextColorPaletteRed = QPalette();
    TextColorPaletteRed.setColor(QPalette::Base, Qt::red);
    TextColorPaletteRed.setColor(QPalette::Text, Qt::black);

    if(fiyatGuncelle){
        ui->AFiyatdoubleSpinBox->setFocus();
        ui->AFiyatdoubleSpinBox->selectAll();
    }
}

void StokKartiForm::AlanlariTemizle()
{
    ui->BarkodlineEdit->clear();
    ui->StokKodlineEdit->clear();
    ui->AdlineEdit->clear();
    ui->UreticicomboBox->setCurrentIndex(0);
    ui->BirimicomboBox->setCurrentIndex(0);
    ui->MiktarlineEdit->clear();
    ui->StokGrubucomboBox->setCurrentIndex(0);
    ui->AFiyatdoubleSpinBox->setValue(0);
    ui->SFiyatdoubleSpinBox->setValue(0);
    ui->KDVspinBox->setValue(0);
    ui->KDVDahilcomboBox->setCurrentIndex(0);
    ui->OTVspinBox->setValue(0);
    ui->OTVDahilcomboBox->setCurrentIndex(0);
    ui->TedarikcicomboBox->setCurrentIndex(0);
}

void StokKartiForm::on_KaydetpushButton_clicked()
{
    QMessageBox msg(this);
    msg.setWindowTitle("Uyarı");
    msg.setIcon(QMessageBox::Warning);
    msg.setText("Kırmızı renkli alanları kontrol ederek düzeltin/doldurun.");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, "Tamam");
    if(ui->SFiyatdoubleSpinBox->value() < ui->AFiyatdoubleSpinBox->value()){
        uyariSes->play();
        msg.setInformativeText("\n\nSatış fiyatı alış fiyatından düşük olamaz.");
        msg.exec();
        return;
    }
    if(!regEXPbarkod.exactMatch(ui->BarkodlineEdit->text())){
        uyariSes->play();
        msg.setInformativeText("\n\nBarkod numarası uygun formatta değil.");
        msg.exec();
        return;
    }
    if(!ui->StokKodlineEdit->text().isEmpty()){
        if(!regEXPstokKod.exactMatch(ui->StokKodlineEdit->text())){
            uyariSes->play();
            msg.setInformativeText("\n\nStok kodu uygun formatta değil.");
            msg.exec();
            return;
        }
    }
    if(!regEXPstokAd.exactMatch(ui->AdlineEdit->text())){
        uyariSes->play();
        msg.setInformativeText("\n\nStok adı uygun formatta değil.");
        msg.exec();
        return;
    }
    if(ui->BirimicomboBox->currentIndex() <= 0){
        uyariSes->play();
        msg.setInformativeText("\n\nStok birimini seçiniz.");
        msg.exec();
        return;
    }
    if(ui->StokGrubucomboBox->currentIndex() <= 0){
        uyariSes->play();
        msg.setInformativeText("\n\nStok gurubunu seçiniz.");
        msg.exec();
        return;
    }
    if(yeniKart){// true ise yeni stok kartı kaydı oluşturur.
        if(!vt.barkodVarmi(ui->BarkodlineEdit->text())){
            kart.setBarkod(ui->BarkodlineEdit->text());
            kart.setKod(ui->StokKodlineEdit->text());
            kart.setAd(QLocale().toUpper(ui->AdlineEdit->text()));
            kart.setBirim(vt.getBirimID(ui->BirimicomboBox->currentText()));
            kart.setMiktar(ui->MiktarlineEdit->text().toFloat());
            kart.setGrup(vt.getGrupID(ui->StokGrubucomboBox->currentText()));
            kart.setAFiyat(ui->AFiyatdoubleSpinBox->value());
            kart.setSFiyat(ui->SFiyatdoubleSpinBox->value());
            kart.setKdv(ui->KDVspinBox->value());
            kart.setOtv(ui->OTVspinBox->value());
            kart.setKdvdahil(ui->KDVDahilcomboBox->currentIndex());
            kart.setOtvdahil(ui->OTVDahilcomboBox->currentIndex());
            kart.setTarih(QDateTime::currentDateTime());
            kart.setUretici(QString::number(vt.getUreticiID(ui->UreticicomboBox->currentText())));
            kart.setTedarikci(QString::number(vt.getTedarikciID(ui->TedarikcicomboBox->currentText())));
            kart.setAciklama(QLocale().toUpper("stok kartı oluşturuldu"));
            QSqlError hataMesajı = vt.yeniStokKartiOlustur(kart, &kullanici);
            if(!hataMesajı.isValid()){
                kayitBasarilimi = true;// kapatınca bir üst formda işlem yapmak için.
                uyariSes->play();
                QMessageBox *msg = new QMessageBox(this);
                msg->setIcon(QMessageBox::Information);
                msg->setWindowTitle("Başarılı");
                msg->setText("Yeni stok kartı oluşturuldu.");
                msg->setStandardButtons(QMessageBox::Ok);
                msg->setDefaultButton(QMessageBox::Ok);
                msg->setButtonText(QMessageBox::Ok, "Tamam");
                msg->exec();
            }
            else{
                uyariSes->play();
                QMessageBox *msg = new QMessageBox(this);
                msg->setIcon(QMessageBox::Critical);
                msg->setWindowTitle("Hata");
                msg->setText("Yeni stok kartı oluşturulamadı.");
                msg->setInformativeText(qPrintable(hataMesajı.text()));
                msg->setStandardButtons(QMessageBox::Ok);
                msg->setDefaultButton(QMessageBox::Ok);
                msg->setButtonText(QMessageBox::Ok, "Tamam");
                msg->exec();
            }
        }
        else{
            uyariSes->play();
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
    else if(!yeniKart){// yenikayit false ise mevcut stok kartını günceller.

        kart.setBarkod(ui->BarkodlineEdit->text());
        kart.setKod(ui->StokKodlineEdit->text());
        kart.setAd(QLocale().toUpper(ui->AdlineEdit->text()));
        kart.setBirim(vt.getBirimID(ui->BirimicomboBox->currentText()));
        kart.setMiktar(ui->MiktarlineEdit->text().toFloat());
        kart.setGrup(vt.getGrupID(ui->StokGrubucomboBox->currentText()));
        kart.setAFiyat(ui->AFiyatdoubleSpinBox->value());
        kart.setSFiyat(ui->SFiyatdoubleSpinBox->value());
        kart.setKdv(ui->KDVspinBox->value());
        kart.setOtv(ui->OTVspinBox->value());
        kart.setKdvdahil(ui->KDVDahilcomboBox->currentIndex());
        kart.setOtvdahil(ui->OTVDahilcomboBox->currentIndex());
        kart.setTarih(QDateTime::currentDateTime());
        kart.setUretici(QString::number(vt.getUreticiID(ui->UreticicomboBox->currentText())));
        kart.setTedarikci(QString::number(vt.getTedarikciID(ui->TedarikcicomboBox->currentText())));
        kart.setAciklama("stok kartı güncelleme");
        vt.stokKartiniGuncelle(kart.getId(), kart, &kullanici);
        fiyatGuncellendi = true;
        // düzenlenen stok kartı hızlı ürün butonlarına ekliyse ini dosyası ve buton bilgisini düzeltme başlangıcı
        vt.setHizliButon(kart);
    }
    this->close();
}


void StokKartiForm::on_iptalpushButton_clicked()
{
    toplustokkartiIptal = true;
    this->close();
}


void StokKartiForm::on_temizlepushButton_clicked()
{
    AlanlariTemizle();
}


void StokKartiForm::on_BarkodlineEdit_textChanged(const QString &arg1)
{
    if(regEXPbarkod.exactMatch(arg1)){
        ui->BarkodlineEdit->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->BarkodlineEdit->setPalette(TextColorPaletteRed);
    }
}


void StokKartiForm::on_StokKodlineEdit_textChanged(const QString &arg1)
{
    if(regEXPstokKod.exactMatch(arg1)){
        ui->StokKodlineEdit->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->StokKodlineEdit->setPalette(TextColorPaletteRed);
    }
}


void StokKartiForm::on_AdlineEdit_textChanged(const QString &arg1)
{
    if(regEXPstokAd.exactMatch(arg1)){
        ui->AdlineEdit->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->AdlineEdit->setPalette(TextColorPaletteRed);
    }
}


void StokKartiForm::on_AFiyatdoubleSpinBox_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    if(ui->AFiyatdoubleSpinBox->value() > ui->SFiyatdoubleSpinBox->value()){
        ui->AFiyatdoubleSpinBox->setPalette(TextColorPaletteRed);
    }
    else{
        ui->AFiyatdoubleSpinBox->setPalette(TextColorPaletteDefault);
    }
}


void StokKartiForm::on_SFiyatdoubleSpinBox_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    if(ui->SFiyatdoubleSpinBox->value() > ui->AFiyatdoubleSpinBox->value()){
        ui->SFiyatdoubleSpinBox->setPalette(TextColorPaletteDefault);
        ui->AFiyatdoubleSpinBox->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->SFiyatdoubleSpinBox->setPalette(TextColorPaletteRed);
    }
}


void StokKartiForm::on_MiktarlineEdit_textChanged(const QString &arg1)
{
    QString str = arg1;
    int i = 0;
    QValidator::State st = regEXPstokMiktar->validate(str, i);

    if(st == QValidator::Acceptable){
        ui->MiktarlineEdit->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->MiktarlineEdit->setPalette(TextColorPaletteRed);
    }
}

void StokKartiForm::setKullanici(const User &newKullanici)
{
    kullanici = newKullanici;
}


void StokKartiForm::on_BarkodOlusturpushButton_clicked()
{
    QString uretilenBarkod(QString::number(QRandomGenerator::global()->bounded(80000000, 89999999)));
    if(!vt.barkodVarmi(uretilenBarkod)){
        ui->BarkodlineEdit->setText(uretilenBarkod);
    }
}


void StokKartiForm::on_stokHareketleripushButton_clicked()
{
    StokHareketleriDialog *hareketlerForm = new StokHareketleriDialog(this);
    hareketlerForm->setStokKarti(kart);
    hareketlerForm->exec();
    delete hareketlerForm;
}


void StokKartiForm::on_cikispushButton_clicked()
{
    this->close();
}

