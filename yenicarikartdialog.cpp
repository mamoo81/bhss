#include "yenicarikartdialog.h"
#include "ui_yenicarikartdialog.h"
#include "cari.h"
//************************************
#include <QCompleter>
#include <QMessageBox>

YeniCariKartDialog::YeniCariKartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YeniCariKartDialog)
{
    ui->setupUi(this);
    FormLoad();
}

YeniCariKartDialog::~YeniCariKartDialog()
{
    delete ui;
}

void YeniCariKartDialog::FormLoad()
{
    ui->CariTipicomboBox->addItems(vt->getCariTipleri());
    // vergi dairelerni veritabanından alıp ilgili objeye yazarken otomatik doldurma için.
    QCompleter *tamamlayici = new QCompleter(vt->getVergiDaireleri(), this);
    tamamlayici->setCompletionMode(QCompleter::PopupCompletion);
    tamamlayici->setCaseSensitivity(Qt::CaseInsensitive);
    ui->VergiDairesilineEdit->setCompleter(tamamlayici);
    //
    ui->ilcomboBox->addItems(vt->getIller());
    ui->CariAdilineEdit->setFocus();
}

void YeniCariKartDialog::on_iptalpushButton_clicked()
{
    this->close();
}


void YeniCariKartDialog::on_ilcomboBox_currentIndexChanged(int index)
{
    ui->ilcecomboBox->clear();
    ui->ilcecomboBox->addItems(vt->getIlceler(index));
}


void YeniCariKartDialog::on_KaydetpushButton_clicked()
{
    QLocale turkce(QLocale::Turkish, QLocale::Turkey);
    Cari yeniCari;
    yeniCari.setTarih(QDateTime::currentDateTime());
    yeniCari.setAd(turkce.toUpper(ui->CariAdilineEdit->text()));
    yeniCari.setYetkili(turkce.toUpper(ui->YetkililineEdit->text()));
    yeniCari.setTip(ui->CariTipicomboBox->currentIndex() + 1);// currentindex 'e +1 ekliyorum. veritabanındaki caritipleri id'sine denk gelsin diye
    yeniCari.setVerigino(ui->VergiNolineEdit->text());
    yeniCari.setVergiDaire(turkce.toUpper(ui->VergiDairesilineEdit->text()));
    yeniCari.setIl(turkce.toUpper(ui->ilcomboBox->currentText()));
    yeniCari.setIlce(turkce.toUpper(ui->ilcecomboBox->currentText()));
    yeniCari.setAdres(turkce.toUpper(ui->AdresplainTextEdit->placeholderText()));
    yeniCari.setMail(ui->MaillineEdit->text());
    yeniCari.setTelefon(ui->TelefonlineEdit->text());
    yeniCari.setAciklama(turkce.toUpper(ui->AciklamaplainTextEdit->placeholderText()));
    vt->yeniCariKart(yeniCari);

    this->close();
}

