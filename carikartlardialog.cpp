#include "carikartlardialog.h"
#include "ui_carikartlardialog.h"
//**************************
#include <QLocale>

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
    ui->CaricomboBox->addItem("DİREKT");
    ui->CaricomboBox->addItems(vt.getCariKartIsimleri());
    ui->CariTipcomboBox->addItems(vt.getCariTipleri());
}

void CariKartlarDialog::on_YenitoolButton_clicked()
{
    ui->ileritoolButton->setEnabled(false);
    ui->GeritoolButton->setEnabled(false);
    ui->CaricomboBox->setEnabled(false);
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

void CariKartlarDialog::on_KaydettoolButton_clicked()
{
    if(ui->CariIDlineEdit->text().isEmpty()){// idlineedit boş ilse yeni kayıt
        Cari yeniCariKart;
        yeniCariKart.setAd(turkce.toUpper(ui->CariAdlineEdit->text()));
        yeniCariKart.setTip(ui->CariTipcomboBox->currentText());
        yeniCariKart.setVerigino(ui->VergiNolineEdit->text());
        yeniCariKart.setVergiDaire(ui->VergiDairelineEdit->text());
        yeniCariKart.setIl(ui->ilcomboBox->currentText());
        yeniCariKart.setIlce(ui->ilcecomboBox->currentText());
        yeniCariKart.setAdres(ui->AdrestextEdit->toPlainText());
        yeniCariKart.setMail(ui->MaillineEdit->text());
        yeniCariKart.setTelefon(ui->TelefonlineEdit->text());
        yeniCariKart.setAciklama(ui->AciklamatextEdit->toPlainText());
        vt.yeniCariKart(yeniCariKart);
        ui->ileritoolButton->setEnabled(true);
        ui->GeritoolButton->setEnabled(true);
        ui->CaricomboBox->setEnabled(true);
        ui->SiltoolButton->setEnabled(true);
    }
    else{// değilse mevcut kayıtı güncelleme

    }
}

