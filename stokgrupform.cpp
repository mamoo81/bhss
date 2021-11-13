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
            QMessageBox msg;
            msg.setWindowTitle("Hata");
            msg.setIcon(QMessageBox::Critical);
            msg.setText("Bu stok gurubu zaten mevcut.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        else
        {
            QMessageBox msg;
            msg.setWindowTitle("Bilgi");
            msg.setIcon(QMessageBox::Information);
            msg.setText("Stok gurubu başarıyla eklendi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
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
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Stok gurubu silindi.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        stokGruplariGetir();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Listeden silinecek gurubu seçin.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
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

