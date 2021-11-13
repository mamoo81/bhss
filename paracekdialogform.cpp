#include "paracekdialogform.h"
#include "ui_paracekdialogform.h"
#include "user.h"
#include "veritabani.h"

ParaCekDialogForm::ParaCekDialogForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParaCekDialogForm)
{
    ui->setupUi(this);
    ui->doubleSpinBox->setFocus();
}

ParaCekDialogForm::~ParaCekDialogForm()
{
    delete ui;
}

void ParaCekDialogForm::setKasadakiPara(double newKasadakiPara)
{
    ui->KasadakiParalabel->setText("₺" + QString::number(newKasadakiPara));
    kasadakiPara = newKasadakiPara;
}

void ParaCekDialogForm::on_IptalpushButton_clicked()
{
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle("Bilgi");
    msg.setText(QString("Kasadan para çekimi iptal edildi."));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, "Tamam");
    msg.setDefaultButton(QMessageBox::Ok);
    msg.exec();
    this->close();
}


void ParaCekDialogForm::on_CekpushButton_clicked()
{
    if(kasadakiPara > ui->doubleSpinBox->value()){
        Veritabani vt = Veritabani();
        vt.kasadanParaCek(ui->doubleSpinBox->value(), kull);
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setWindowTitle("Bilgi");
        msg.setText(QString("Kasadan ₺%1 lira çekildi.").arg(QString::number(ui->doubleSpinBox->value())));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
        this->close();
    }
    else{
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle("Bilgi");
        msg.setText(QString("Kasada ki miktardan yüksek olamaz.").arg(QString::number(ui->doubleSpinBox->value())));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
    }
}

void ParaCekDialogForm::setKull(const User &newKull)
{
    kull = newKull;
}


void ParaCekDialogForm::on_doubleSpinBox_valueChanged(double curValue)
{
    ui->kalanParaLabel->setText("₺" + QString::number(kasadakiPara - curValue));
}

