#include "birimekleform.h"
#include "ui_birimekleform.h"
//****************************
#include <QMessageBox>
#include <QList>
#include <QListWidgetItem>
#include <QDebug>

BirimekleForm::BirimekleForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BirimekleForm)
{
    ui->setupUi(this);

    FormLoad();
}

BirimekleForm::~BirimekleForm()
{
    delete ui;
}

void BirimekleForm::FormLoad()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(vt_birim->getStokBirimleri());
}

void BirimekleForm::on_eklepushButton_clicked()
{
    QList<QListWidgetItem *> aranacak = ui->listWidget->findItems(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->lineEdit->text()), Qt::MatchExactly);
    if(aranacak.size() > 0){
         QMessageBox msg(this);
         msg.setWindowTitle("Dikkat");
         msg.setIcon(QMessageBox::Warning);
         msg.setText("Girdiğiniz birim zaten mevcut");
         msg.setStandardButtons(QMessageBox::Ok);
         msg.setButtonText(QMessageBox::Ok, "Tamam");
         msg.exec();
    }
    else{
        vt_birim->stokBirimEkle(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->lineEdit->text()));
        ui->listWidget->clear();
        ui->listWidget->addItems(vt_birim->getStokBirimleri());
    }
}


void BirimekleForm::on_silpushButton_clicked()
{
    QMessageBox msg(this);
    msg.setWindowTitle("Dikkat");
    msg.setIcon(QMessageBox::Question);
    msg.setText("Silmek istediğinize emin misiniz?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setButtonText(QMessageBox::Yes, "Evet");
    msg.setButtonText(QMessageBox::No, "Hayır");
    msg.setDefaultButton(QMessageBox::Yes);
    msg.exec();
    if(QMessageBox::Yes == msg.result())
    {
        vt_birim->stokBirimSil(ui->listWidget->currentItem()->text());
        delete ui->listWidget->currentItem();
    }
}

