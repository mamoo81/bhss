#include "carihareketiekleform.h"
#include "ui_carihareketiekleform.h"
//********************************************
#include <QDebug>
#include <QMessageBox>
#include <QMetaEnum>

CariHareketiEkleForm::CariHareketiEkleForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CariHareketiEkleForm)
{
    ui->setupUi(this);

    FormLoad();
}

CariHareketiEkleForm::~CariHareketiEkleForm()
{
    delete ui;
}

void CariHareketiEkleForm::setKullanici(const User &newKullanici)
{
    kullanici = newKullanici;
}

void CariHareketiEkleForm::setFaturaTip(FaturaTipi newFaturaTip)
{
    faturaTip = newFaturaTip;
}

void CariHareketiEkleForm::setCariID(const QString &newCariID)
{
    cariID = newCariID;
    cariKart = vt->getCariKart(cariID);
    ui->unvanlineEdit->setText(cariKart.getAd());
    ui->YetkililineEdit->setText(cariKart.getYetkili());
    ui->islemdateEdit->setDate(QDate::currentDate());
    ui->OdemeTipcomboBox->addItems(vt->getOdemeTipleri());
}


void CariHareketiEkleForm::FormLoad()
{


}

void CariHareketiEkleForm::on_KaydetpushButton_clicked()
{
    vt->caridenTahsilatYap(cariID,
                           ui->islemTutaridoubleSpinBox->value(),
                           ui->islemdateEdit->dateTime(),
                           faturaTip,
                           odemeTip,
                           kullanici,
                           QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->EvrakNolineEdit->text()),
                           QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->AciklamaplainTextEdit->toPlainText()));
    QMessageBox msg(this);
    msg.setWindowTitle("Bilgi");
    msg.setIcon(QMessageBox::Information);
    msg.setText(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(QVariant::fromValue(faturaTip).toString()) + " Kaydedildi.");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, "Tamam");
    msg.exec();
}

void CariHareketiEkleForm::on_OdemeTipcomboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    switch (ui->OdemeTipcomboBox->currentIndex()) {
    case 0:
        odemeTip = nakit;
        break;
    case 1:
        odemeTip = kredikarti;
        break;
    case 2:
        odemeTip = havale;
        break;
    case 3:
        odemeTip = cek;
        break;
    case 4:
        odemeTip = senet;
        break;
    }
}

