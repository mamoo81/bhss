#include "resimekledialog.h"
#include "stokgircikdialog.h"
#include "stokhareketleridialog.h"
#include "stokkartiform.h"
#include "ui_stokkartiform.h"

#include <QSqlError>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QEvent>

StokKartiForm::StokKartiForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StokKartiForm)
{
    ui->setupUi(this);

    ui->BarkodlineEdit->installEventFilter(this);
    uyariSesi.setSource(QUrl("qrc:/sounds/sounds/warning-sound.wav"));;
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
    ui->MiktardoubleSpinBox->setValue(kart.getMiktar());
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

void StokKartiForm::setKart(QString barkod)
{
    guncellenecekKart = stokYonetimi.getStokKarti(barkod);
    ui->BarkodlineEdit->setText(guncellenecekKart.getBarkod());
    ui->StokKodlineEdit->setText(guncellenecekKart.getKod());
    ui->AdlineEdit->setText(guncellenecekKart.getAd());
    ui->UreticicomboBox->setCurrentIndex(ui->UreticicomboBox->findText(guncellenecekKart.getUretici()));
    ui->BirimicomboBox->setCurrentIndex(guncellenecekKart.getBirim());
    ui->MiktardoubleSpinBox->setValue(guncellenecekKart.getMiktar());
    ui->StokGrubucomboBox->setCurrentIndex(guncellenecekKart.getGrup());
    ui->AFiyatdoubleSpinBox->setValue(guncellenecekKart.getAFiyat());
    ui->SFiyatdoubleSpinBox->setValue(guncellenecekKart.getSFiyat());
    ui->KDVspinBox->setValue(guncellenecekKart.getKdv());
    if(guncellenecekKart.getKdvdahil()){
        ui->KDVDahilcomboBox->setCurrentIndex(1);
    }
    else{
        ui->KDVDahilcomboBox->setCurrentIndex(0);
    }
    ui->OTVspinBox->setValue(guncellenecekKart.getOtv());
    if(guncellenecekKart.getOtvdahil()){
        ui->OTVDahilcomboBox->setCurrentIndex(1);
    }
    else{
        ui->OTVDahilcomboBox->setCurrentIndex(0);
    }
    ui->TedarikcicomboBox->setCurrentIndex(0);
    ui->UrunResimlabel->setPixmap(guncellenecekKart.getResim());
}

void StokKartiForm::FormLoad()
{
    ui->UreticicomboBox->addItems(stokYonetimi.getUreticiler());
    ui->TedarikcicomboBox->addItems(stokYonetimi.getTedarikciler());
    ui->BirimicomboBox->addItems(stokYonetimi.getStokBirimleri());
    ui->StokGrubucomboBox->addItem("Stok grubu seçin...");
    ui->StokGrubucomboBox->addItems(stokYonetimi.stokGruplariGetir());

    if(toplustokkarti){
        ui->iptalpushButton->setEnabled(true);
    }
    else{
        ui->iptalpushButton->setEnabled(false);
    }

    if(yeniKart){
        ui->stokHareketleripushButton->setEnabled(false);
        ui->MiktardoubleSpinBox->setEnabled(true);
        ui->stokGirpushButton->setEnabled(false);
        ui->stokDuspushButton->setEnabled(false);
    }
    else{
        ui->stokHareketleripushButton->setEnabled(true);
        ui->MiktardoubleSpinBox->setEnabled(false);
        ui->stokGirpushButton->setEnabled(true);
        ui->stokDuspushButton->setEnabled(true);
    }

    regEXPbarkod = QRegularExpression("[0-9]{8,13}");
    QValidator *validatorBarkod = new QRegularExpressionValidator(regEXPbarkod, this);
    ui->BarkodlineEdit->setValidator(validatorBarkod);

    regEXPstokKod = QRegularExpression("[a-zöçşıiğüA-ZÖÇŞIİĞÜ0-9]{3,}");
    QValidator *validatorStokKod = new QRegularExpressionValidator(regEXPstokKod, this);
    ui->StokKodlineEdit->setValidator(validatorStokKod);

    regEXPstokAd = QRegularExpression("[a-zöçşıiğü A-ZÖÇŞIİĞÜ 0-9.*'+-]{3,}");
    QValidator *validatorStokAd = new QRegularExpressionValidator(regEXPstokAd, this);
    ui->AdlineEdit->setValidator(validatorStokAd);

//    regEXPstokMiktar = new QDoubleValidator(0, 999999999, 10, this);
//    regEXPstokMiktar->setNotation(QDoubleValidator::StandardNotation);
//    regEXPstokMiktar->setLocale(QLocale::C);
//    ui->MiktardoubleSpinBox->setValidator(regEXPstokMiktar);

    TextColorPaletteDefault = ui->BarkodlineEdit->style()->standardPalette();

    TextColorPaletteRed = QPalette();
    TextColorPaletteRed.setColor(QPalette::Base, Qt::red);
    TextColorPaletteRed.setColor(QPalette::Text, Qt::black);

    if(fiyatGuncelle){
        ui->AFiyatdoubleSpinBox->setFocus();
        ui->AFiyatdoubleSpinBox->selectAll();
    }
    else{
        ui->BarkodlineEdit->setFocus();
    }

    ENTER_key = new QShortcut(this);
    ENTER_key->setKey(Qt::Key_Enter);
    connect(ENTER_key, SIGNAL(activated()), this, SLOT(Enter_Slot()));

    RETURN_key = new QShortcut(this);
    RETURN_key->setKey(Qt::Key_Return);
    connect(RETURN_key, SIGNAL(activated()), this, SLOT(Enter_Slot()));

    ESC_key = new QShortcut(this);
    ESC_key->setKey(Qt::Key_Escape);
    connect(ESC_key, SIGNAL(activated()), this, SLOT(ESC_Slot()));
}

void StokKartiForm::AlanlariTemizle()
{
    ui->BarkodlineEdit->clear();
    ui->StokKodlineEdit->clear();
    ui->AdlineEdit->clear();
    ui->UreticicomboBox->setCurrentIndex(0);
    ui->BirimicomboBox->setCurrentIndex(0);
    ui->MiktardoubleSpinBox->setValue(0);
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

    if(ui->SFiyatdoubleSpinBox->value() <= ui->AFiyatdoubleSpinBox->value()){
        uyariSesi.play();
        msg.setInformativeText("\n\nSatış fiyatı alış fiyatından düşük veya eşit olamaz.");
        msg.exec();
        return;
    }

    QRegularExpressionMatch barkodMatch = regEXPbarkod.match(ui->BarkodlineEdit->text());
    if(!barkodMatch.hasMatch()){
        uyariSesi.play();
        msg.setInformativeText("\n\nBarkod numarası uygun formatta değil.");
        msg.exec();
        return;
    }

    QRegularExpressionMatch stokKodMatch = regEXPstokKod.match(ui->StokKodlineEdit->text());
    if(!ui->StokKodlineEdit->text().isEmpty()){
        if(!stokKodMatch.hasMatch()){
            uyariSesi.play();
            msg.setInformativeText("\n\nStok kodu uygun formatta değil.");
            msg.exec();
            return;
        }
    }

    QRegularExpressionMatch stokAdMatch = regEXPstokAd.match(ui->AdlineEdit->text());
    if(!stokAdMatch.hasMatch()){
        uyariSesi.play();
        msg.setInformativeText("\n\nStok adı uygun formatta değil.");
        msg.exec();
        return;
    }
    if(ui->BirimicomboBox->currentIndex() <= 0){
        uyariSesi.play();
        msg.setInformativeText("\n\nStok birimini seçiniz.");
        msg.exec();
        return;
    }
    if(ui->StokGrubucomboBox->currentIndex() <= 0){
        uyariSesi.play();
        msg.setInformativeText("\n\nStok gurubunu seçiniz.");
        msg.exec();
        return;
    }
    if(yeniKart){// true ise yeni stok kartı kaydı oluşturur.
        if(!stokYonetimi.barkodVarmi(ui->BarkodlineEdit->text())){
            kart.setBarkod(ui->BarkodlineEdit->text());
            kart.setKod(ui->StokKodlineEdit->text());
            kart.setAd(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->AdlineEdit->text()));
            kart.setBirim(stokYonetimi.getBirimID(ui->BirimicomboBox->currentText()));
            kart.setMiktar(ui->MiktardoubleSpinBox->value());
            kart.setGrup(vt.getGrupID(ui->StokGrubucomboBox->currentText()));
            kart.setAFiyat(ui->AFiyatdoubleSpinBox->value());
            kart.setSFiyat(ui->SFiyatdoubleSpinBox->value());
            kart.setKdv(ui->KDVspinBox->value());
            kart.setOtv(ui->OTVspinBox->value());
            kart.setKdvdahil(ui->KDVDahilcomboBox->currentIndex());
            kart.setOtvdahil(ui->OTVDahilcomboBox->currentIndex());
            kart.setTarih(QDateTime::currentDateTime());
            kart.setUretici(QString::number(stokYonetimi.getUreticiID(ui->UreticicomboBox->currentText())));
            kart.setTedarikci(QString::number(stokYonetimi.getTedarikciID(ui->TedarikcicomboBox->currentText())));
            kart.setAciklama(QLocale().toUpper("stok kartı oluşturuldu"));
            QSqlError hataMesajı = stokYonetimi.yeniStokKartiOlustur(kart, &kullanici);
            if(!hataMesajı.isValid()){
                kayitBasarilimi = true;// kapatınca bir üst formda işlem yapmak için.
                uyariSesi.play();
                QMessageBox msg(this);
                msg.setIcon(QMessageBox::Information);
                msg.setWindowTitle("Başarılı");
                msg.setText("Yeni stok kartı oluşturuldu.");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setDefaultButton(QMessageBox::Ok);
                // msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
                this->close();
            }
            else{
                uyariSesi.play();
                QMessageBox msg(this);
                msg.setIcon(QMessageBox::Critical);
                msg.setWindowTitle("Hata");
                msg.setText("Yeni stok kartı oluşturulamadı.");
                msg.setInformativeText(qPrintable(hataMesajı.text()));
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setDefaultButton(QMessageBox::Ok);
                // msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
            }
        }
        else{
            uyariSesi.play();
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Bu barkod veritabanında başka bir stok kartına tanımlı!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            // msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
    else if(!yeniKart){// yenikayit false ise mevcut stok kartını günceller.
        guncellenenKart.setId(guncellenecekKart.getId());
        guncellenenKart.setBarkod(ui->BarkodlineEdit->text());
        guncellenenKart.setKod(ui->StokKodlineEdit->text());
        guncellenenKart.setAd(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->AdlineEdit->text()));
        guncellenenKart.setBirim(stokYonetimi.getBirimID(ui->BirimicomboBox->currentText()));
        guncellenenKart.setMiktar(ui->MiktardoubleSpinBox->value());
        guncellenenKart.setGrup(vt.getGrupID(ui->StokGrubucomboBox->currentText()));
        guncellenenKart.setAFiyat(ui->AFiyatdoubleSpinBox->value());
        guncellenenKart.setSFiyat(ui->SFiyatdoubleSpinBox->value());
        guncellenenKart.setKdv(ui->KDVspinBox->value());
        guncellenenKart.setOtv(ui->OTVspinBox->value());
        guncellenenKart.setKdvdahil(ui->KDVDahilcomboBox->currentIndex());
        guncellenenKart.setOtvdahil(ui->OTVDahilcomboBox->currentIndex());
        guncellenenKart.setTarih(QDateTime::currentDateTime());
        guncellenenKart.setUretici(QString::number(stokYonetimi.getUreticiID(ui->UreticicomboBox->currentText())));
        guncellenenKart.setTedarikci(QString::number(stokYonetimi.getTedarikciID(ui->TedarikcicomboBox->currentText())));
        guncellenenKart.setAciklama("stok kartı güncelleme");

        if(degisimVarmi(guncellenecekKart, guncellenenKart)){
            // fiyat değiştiyse stokkartiniguncelle metodunda ona göre işlem yapsın.
            QSqlError sqlcvp;
            if(guncellenecekKart.getSFiyat() != guncellenenKart.getSFiyat()){
                sqlcvp = stokYonetimi.stokKartiniGuncelle(guncellenenKart, &kullanici, true);
            }
            else{
                sqlcvp = stokYonetimi.stokKartiniGuncelle(guncellenenKart, &kullanici, false);
            }

            if(!sqlcvp.isValid()){
                fiyatGuncellendi = true;
                kayitBasarilimi = true;
                // düzenlenen stok kartı hızlı ürün butonlarına ekliyse ini dosyası ve buton bilgisini düzeltme başlangıcı
                vt.setHizliButon(guncellenenKart);

                uyariSesi.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Bilgi");
                msg.setText("Stok Kartı güncellendi");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
                this->close();
            }
            else{
                uyariSesi.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Bilgi");
                msg.setText(QString("Stok Kartı güncellenemedi!!\n\nHata Mesajı:\n\n%1").arg(sqlcvp.text()));
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
            }
        }
        else{
            fiyatGuncellendi = false;
            kayitBasarilimi = false;
            this->close();
        }
    }
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
    QRegularExpressionMatch barkodMatch = regEXPbarkod.match(arg1);
    if(barkodMatch.hasMatch()){
        ui->BarkodlineEdit->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->BarkodlineEdit->setPalette(TextColorPaletteRed);
    }
}


void StokKartiForm::on_StokKodlineEdit_textChanged(const QString &arg1)
{
    QRegularExpressionMatch stokKodMatch = regEXPstokKod.match(arg1);
    if(stokKodMatch.hasMatch()){
        ui->StokKodlineEdit->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->StokKodlineEdit->setPalette(TextColorPaletteRed);
    }
}


void StokKartiForm::on_AdlineEdit_textChanged(const QString &arg1)
{
    QRegularExpressionMatch stokAdMatch = regEXPstokAd.match(arg1);
    if(stokAdMatch.hasMatch()){
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


//void StokKartiForm::on_MiktarlineEdit_textChanged(const QString &arg1)
//{
//    QString str = arg1;
//    int i = 0;
//    QValidator::State st = regEXPstokMiktar->validate(str, i);

//    if(st == QValidator::Acceptable){
//        ui->MiktardoubleSpinBox->setPalette(TextColorPaletteDefault);
//    }
//    else{
//        ui->MiktarlineEdit->setPalette(TextColorPaletteRed);
//    }
//}

void StokKartiForm::setKullanici(const User &newKullanici)
{
    kullanici = newKullanici;
}

bool StokKartiForm::degisimVarmi(StokKarti eskiKart, StokKarti yeniKart)
{
    if(eskiKart.getBarkod() != yeniKart.getBarkod()){
//        qDebug() << "barkodu değişmiş";
        return true;
    }
    else if(eskiKart.getKod() != yeniKart.getKod()){
//        qDebug() << "kodu değişmiş";
        return true;
    }
    else if(eskiKart.getAd() != yeniKart.getAd()){
//        qDebug() << "adı değişmiş";
        return true;
    }
    else if(eskiKart.getBirim() != yeniKart.getBirim()){
//        qDebug() << "birimi değişmiş";
        return true;
    }
    else if(eskiKart.getMiktar() != yeniKart.getMiktar()){
        return true;
    }
    else if(eskiKart.getGrup() != yeniKart.getGrup()){
//        qDebug() << "grubu değişmiş";
        return true;
    }
    else if(eskiKart.getAFiyat() != yeniKart.getAFiyat()){
//        qDebug() << "alış fiyatı değişmiş";
        return true;
    }
    else if(eskiKart.getSFiyat() != yeniKart.getSFiyat()){
//        qDebug() << "satış fiyatı değişmiş";
        return true;
    }
    else if(eskiKart.getKdv() != yeniKart.getKdv()){
//        qDebug() << "kdv yüzdesi değişmiş";
        return true;
    }
    else if(eskiKart.getOtv() != yeniKart.getOtv()){
//        qDebug() << "otv yüzdesi değişmiş";
        return true;
    }
    else if(eskiKart.getKdvdahil() != yeniKart.getKdvdahil()){
//        qDebug() << "kdv değişmiş";
        return true;
    }
    else if(eskiKart.getOtvdahil() != yeniKart.getOtvdahil()){
//        qDebug() << "otv değişmiş";
        return true;
    }
    else{
        return false;
    }
}

void StokKartiForm::urunResmiKaydet(QPixmap urunResmi, QString urunBarkod)
{
    auto resimlerDizini = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/urunler-image/";
    if(!QFileInfo::exists(resimlerDizini)){
        QDir().mkdir(resimlerDizini);
    }
    // YENİ RESİM KAYDETME
    QFile yeniResim(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/urunler-image/" + urunBarkod + ".png");
    yeniResim.open(QIODevice::WriteOnly | QIODevice::Truncate);
    urunResmi.save(&yeniResim, "PNG");
    yeniResim.close();

    if(!QFileInfo(yeniResim).exists()){
        uyariSesi.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Resim kaydedilemedi.");
        msg.setStandardButtons(QMessageBox::Ok);
        // msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

bool StokKartiForm::eventFilter(QObject *filtrelenecekObject, QEvent *e)
{
    if(filtrelenecekObject == ui->BarkodlineEdit){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
            if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return){
                e->ignore();
                return true;
            }
            return false;
        }
        return false;
    }
    return StokKartiForm::eventFilter(filtrelenecekObject, e);
}


void StokKartiForm::on_BarkodOlusturpushButton_clicked()
{
    // burası for döngüsüne alarak iyileştirilebilir.

    QString uretilenBarkod(QString::number(QRandomGenerator::global()->bounded(8000000, 8999999)));

    // üretilen barkodun doğrulama kodunu bulma
    int ciftSayilar = (uretilenBarkod.at(6).digitValue() + uretilenBarkod.at(4).digitValue() + uretilenBarkod.at(2).digitValue() + uretilenBarkod.at(0).digitValue()) * 3;
    int tekSayilar = uretilenBarkod.at(5).digitValue() + uretilenBarkod.at(3).digitValue() + uretilenBarkod.at(1).digitValue();
    int dogrulamaKodu = (((tekSayilar + ciftSayilar) % 10) - 10) % 10;

    if(dogrulamaKodu < 0){// sayı negatif çıkarsa pozitife çevirmek için.
        dogrulamaKodu = dogrulamaKodu * -1;
    }

//    qDebug() << "üretilen numara: " << uretilenNumara;
//    qDebug() << "doğrulama kodu: " << dogrulamaKodu;
    uretilenBarkod.append(QString::number(dogrulamaKodu));
//    qDebug() << "tam barkod: " << uretilenNumara;

    if(!stokYonetimi.barkodVarmi(uretilenBarkod)){
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


void StokKartiForm::on_stokGirpushButton_clicked()
{
    if(!yeniKart){
        StokGirCikDialog *stokMiktarigirForm = new StokGirCikDialog(this);
        stokMiktarigirForm->setHareket(StokYonetimi::StokHareketi::Giris);
        stokMiktarigirForm->exec();
        if(stokMiktarigirForm->getMiktar() > 0){
    //        StokKarti kart = stokYonetimi.getStokKarti(ui->StokKartlaritableView->model()->index(seciliStokIndex, 1).data().toString());
            if(stokYonetimi.setStokMiktari(kullanici, guncellenecekKart, StokYonetimi::StokHareketi::Giris, stokMiktarigirForm->getMiktar())){
                uyariSesi.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Bilgi");
                msg.setIcon(QMessageBox::Information);
                msg.setText(QString("%1 adet %2 yapıldı.").arg(QString::number(stokMiktarigirForm->getMiktar(), 'f', 2), QString("giriş")));
                msg.setStandardButtons(QMessageBox::Ok);
                // msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
                ui->MiktardoubleSpinBox->setValue(ui->MiktardoubleSpinBox->value() + stokMiktarigirForm->getMiktar());
            }
        }
    }
}


void StokKartiForm::on_stokDuspushButton_clicked()
{
    if(!yeniKart){
        StokGirCikDialog *stokMiktarigirForm = new StokGirCikDialog(this);
        stokMiktarigirForm->setWindowTitle("Stok Çık");
        stokMiktarigirForm->setHareket(StokYonetimi::StokHareketi::Cikis);
        stokMiktarigirForm->exec();
        if(stokMiktarigirForm->getMiktar() > 0){
//            StokKarti kart = stokYonetimi.getStokKarti(ui->StokKartlaritableView->model()->index(seciliStokIndex, 1).data().toString());
            if(stokYonetimi.setStokMiktari(kullanici, guncellenecekKart, StokYonetimi::StokHareketi::Cikis, stokMiktarigirForm->getMiktar())){
                uyariSesi.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Bilgi");
                msg.setIcon(QMessageBox::Information);
                msg.setText(QString("%1 adet %2 yapıldı.").arg(QString::number(stokMiktarigirForm->getMiktar(), 'f', 2), QString("çıkış")));
                msg.setStandardButtons(QMessageBox::Ok);
                // msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
                ui->MiktardoubleSpinBox->setValue(ui->MiktardoubleSpinBox->value() - stokMiktarigirForm->getMiktar());
            }
        }
    }
}

void StokKartiForm::Enter_Slot()
{
    on_KaydetpushButton_clicked();
}

void StokKartiForm::ESC_Slot()
{
    on_iptalpushButton_clicked();
}


void StokKartiForm::on_ResimEklepushButton_clicked()
{
    if(!ui->BarkodlineEdit->text().isNull() || !ui->BarkodlineEdit->text().isEmpty()){
        ResimEkleDialog *resimEkleForm = new ResimEkleDialog(this);
        resimEkleForm->setUrunBarkod(ui->BarkodlineEdit->text());
        resimEkleForm->exec();
        if(QFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/urunler-image/" + ui->BarkodlineEdit->text() + ".png").exists()){
            ui->UrunResimlabel->setPixmap(QPixmap(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/urunler-image/" + ui->BarkodlineEdit->text() + ".png"));
        }
        else{
            ui->UrunResimlabel->setPixmap(QPixmap(":/images/ui/box.png"));
        }
    }
    else{
        uyariSesi.play();
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle("Uyarı");
        msg.setText("Önce stok kartına barkod giriniz.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}

void StokKartiForm::on_ResimSilpushButton_clicked()
{
    if(!guncellenecekKart.getResim().isNull()){
        uyariSesi.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Ürün resmini silmek istediğinize emin misiniz?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        // msg.setButtonText(QMessageBox::Yes, "Evet");
        // msg.setButtonText(QMessageBox::No, "Hayır");
        msg.setDefaultButton(QMessageBox::Yes);
        int cevap = msg.exec();
        if(cevap == QMessageBox::Yes){
            QFile resimDosya(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/urunler-image/" + ui->BarkodlineEdit->text() + ".png");
            if(resimDosya.remove()){
                uyariSesi.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Uyarı");
                msg.setIcon(QMessageBox::Information);
                msg.setText("Ürün resmi silindi.");
                msg.setStandardButtons(QMessageBox::Ok);
                // msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
                ui->UrunResimlabel->setPixmap(guncellenecekKart.getResim());
            }
            else{
                uyariSesi.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Uyarı");
                msg.setIcon(QMessageBox::Warning);
                msg.setText("Ürün resmi silinemedi.");
                msg.setStandardButtons(QMessageBox::Ok);
                // msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
            }
        }
    }
}

