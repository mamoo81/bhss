#include "carikartlardialog.h"
#include "ui_carikartlardialog.h"
//**************************
#include <QCompleter>
#include <QLocale>
#include <QDateTime>

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


void CariKartlarDialog::formLoad()
{
    setVergiDaireleri(vt.getVergiDaireleri());

    QCompleter *tamamlayici = new QCompleter(vergiDaireleri, this);
    tamamlayici->setCompletionMode(QCompleter::PopupCompletion);
    tamamlayici->setCaseSensitivity(Qt::CaseInsensitive);
    ui->VergiDairelineEdit->setCompleter(tamamlayici);

    ui->CariKartlartableView->setModel(vt.getCariKartIsimleri());
    ui->CariTipcomboBox->addItems(vt.getCariTipleri());
    ui->ilcomboBox->addItems(vt.getIller());
    ui->ilcomboBox->setCurrentIndex(0);
    ui->ilcecomboBox->addItem("İlçe seçiniz.");
    ui->ilcecomboBox->setCurrentIndex(0);
}

void CariKartlarDialog::on_YenitoolButton_clicked()
{
    ui->SiltoolButton->setEnabled(false);
    alanlariTemizle();
}

void CariKartlarDialog::alanlariTemizle()
{
    ui->CariIDlineEdit->clear();
    ui->CariAdlineEdit->clear();
    ui->CariTipcomboBox->setCurrentIndex(0);
    ui->VergiNolineEdit->clear();
    ui->VergiDairelineEdit->clear();
    ui->ilcomboBox->setCurrentIndex(0);
    ui->ilcecomboBox->setCurrentIndex(0);
    ui->AdrestextEdit->clear();
    ui->MaillineEdit->clear();
    ui->TelefonlineEdit->clear();
    ui->AciklamatextEdit->clear();
}

void CariKartlarDialog::alanlariDoldur(QString _cariID)
{
    Cari gelenCari = vt.getCariKart(_cariID);
    ui->CariIDlineEdit->setText(QString(gelenCari.getId()));
    ui->CariAdlineEdit->setText(gelenCari.getAd());
    if(gelenCari.getTip() == 1){
        ui->CariTipcomboBox->setCurrentIndex(0);
    }
    else if(gelenCari.getTip() == 2){
        ui->CariTipcomboBox->setCurrentIndex(1);
    }
    ui->VergiNolineEdit->setText(gelenCari.getVerigino());
    ui->VergiDairelineEdit->setText(gelenCari.getVergiDaire());
    ui->ilcomboBox->setCurrentIndex(ui->ilcomboBox->findText(gelenCari.getIl()));
    ui->ilcecomboBox->setCurrentIndex(ui->ilcecomboBox->findText(gelenCari.getIlce()));
    ui->AdrestextEdit->setText(gelenCari.getAdres());
    ui->MaillineEdit->setText(gelenCari.getMail());
    ui->TelefonlineEdit->setText(gelenCari.getTelefon());
    ui->AciklamatextEdit->setText(gelenCari.getAciklama());
}

void CariKartlarDialog::setVergiDaireleri(const QStringList &newVergiDaireleri)
{
    vergiDaireleri = newVergiDaireleri;
}

void CariKartlarDialog::on_KaydettoolButton_clicked()
{
    if(ui->CariIDlineEdit->text().isEmpty()){// idlineedit boş ilse yeni kayıt
        if(!ui->CariAdlineEdit->text().isEmpty()){
            Cari yeniCariKart;
            yeniCariKart.setAd(turkce.toUpper(ui->CariAdlineEdit->text()));
            if(ui->CariTipcomboBox->currentText() == "MÜŞTERİ"){
                yeniCariKart.setTip(1);
            }
            else if(ui->CariTipcomboBox->currentText() == "TOPTANCI"){
                yeniCariKart.setTip(2);
            }
            yeniCariKart.setVerigino(ui->VergiNolineEdit->text());
            yeniCariKart.setVergiDaire(ui->VergiDairelineEdit->text());
            yeniCariKart.setIl(ui->ilcomboBox->currentText());
            yeniCariKart.setIlce(ui->ilcecomboBox->currentText());
            yeniCariKart.setAdres(turkce.toUpper(ui->AdrestextEdit->toPlainText()));
            yeniCariKart.setMail(ui->MaillineEdit->text());
            yeniCariKart.setTelefon(ui->TelefonlineEdit->text());
            yeniCariKart.setTarih(QDateTime::currentDateTime());
            yeniCariKart.setAciklama(ui->AciklamatextEdit->toPlainText());
            vt.yeniCariKart(yeniCariKart);
            ui->SiltoolButton->setEnabled(true);
        }
        else{
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Cari adını yazmalısınız.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
    else{// değilse mevcut kayıtı güncelleme

    }
}


void CariKartlarDialog::on_ilcomboBox_currentIndexChanged(int index)
{
    ui->ilcecomboBox->clear();
    ui->ilcecomboBox->addItems(vt.getIlceler(index));
}

void CariKartlarDialog::cariBilgileriGetir(QString _cariID)
{

}


void CariKartlarDialog::on_CariKartlartableView_clicked(const QModelIndex &index)
{
    alanlariDoldur(index.model()->index(ui->CariKartlartableView->currentIndex().row(), 0).data().toString());
}

