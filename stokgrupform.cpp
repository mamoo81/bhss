#include "stokgrupform.h"
#include "ui_stokgrupform.h"
#include "veritabani.h"
//*****************************
#include <QMessageBox>
#include <QDebug>

StokGrupForm::StokGrupForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StokGrupForm)
{
    ui->setupUi(this);

    formLoad();
}


StokGrupForm::~StokGrupForm()
{
    delete ui;
}

void StokGrupForm::formLoad()
{
    stokGruplariGetir();
    ui->YenilineEdit->setFocus();
}

void StokGrupForm::on_EkleBtn_clicked()
{
    if(!ui->YenilineEdit->text().isEmpty())
    {
        QString aranacak = ui->YenilineEdit->text().toUpper();
        QList<QListWidgetItem *> eslesme = ui->gruplarlistWidget->findItems(aranacak,Qt::MatchExactly);
        if(eslesme.size() > 0)
        {
            QMessageBox::critical(this,"Hata","Bu stok gurubu zaten mevcut.",QMessageBox::Ok);
        }
        else
        {
            QMessageBox::information(this,"Bilgi","Stok gurubu eklendi",QMessageBox::Ok);
            ui->YenilineEdit->clear();
            stokGruplariGetir();
            ui->YenilineEdit->setFocus();
        }
    }
}


void StokGrupForm::on_SilBtn_clicked()
{
    if(ui->gruplarlistWidget->selectedItems().size() != 0 )
    {
        QListWidgetItem *silinecek_item = ui->gruplarlistWidget->takeItem(ui->gruplarlistWidget->currentRow());
        delete silinecek_item;
        QMessageBox::information(this,"Uyarı","Stok grubu silindi.",QMessageBox::Ok);
        stokGruplariGetir();
    }
    else
    {
        QMessageBox::critical(this,"Uyarı","Listeden silinecek grubu seçin",QMessageBox::Ok);
    }
}

void StokGrupForm::on_KaydetBtn_clicked()
{

}

void StokGrupForm::stokGruplariGetir()
{
    Veritabani *vt_grup = new Veritabani();
    QStringList liste = vt_grup->stokGruplariGetir();
    foreach (auto item, liste) {
        ui->gruplarlistWidget->addItem(item);
    }
    delete vt_grup;
}

void StokGrupForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
}

