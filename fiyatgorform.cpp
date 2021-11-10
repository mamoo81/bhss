#include "fiyatgorform.h"
#include "ui_fiyatgorform.h"
#include "veritabani.h"
#include "stokkarti.h"
//*****************************
#include <QKeyEvent>
#include <QTimer>

FiyatGorForm::FiyatGorForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiyatGorForm)
{
    ui->setupUi(this);
}

FiyatGorForm::~FiyatGorForm()
{
    delete ui;
}


void FiyatGorForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        if(!ui->BarkodlineEdit->text().isEmpty()){
            Veritabani *vt = new Veritabani();
            StokKarti kart = vt->getStokKarti(ui->BarkodlineEdit->text());
            ui->urunAdiLabel->setText(kart.getAd());
            ui->fiyatLabel->setText(QString::number(kart.getSFiyat()));
            QTimer::singleShot(1500, this, &FiyatGorForm::slotTimer);
            delete vt;
        }
    }
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
}

void FiyatGorForm::slotTimer()
{
    this->close();
}
