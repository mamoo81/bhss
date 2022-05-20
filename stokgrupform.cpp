#include "stokgrupform.h"
#include "ui_stokgrupform.h"
//*****************************
#include <QMessageBox>
#include <QDebug>
#include <QLocale>

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
            QMessageBox msg(this);
            msg.setWindowTitle("Hata");
            msg.setIcon(QMessageBox::Critical);
            msg.setText("Bu stok gurubu zaten mevcut.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
        else
        {
            vt_grup->stokGrupEkle(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->YenilineEdit->text()));
            QMessageBox msg(this);
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
        if(ui->gruplarlistWidget->currentRow() != 0){
            vt_grup->stokGrupSil(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(ui->gruplarlistWidget->currentItem()->text()));
            QListWidgetItem *silinecek_item = ui->gruplarlistWidget->takeItem(ui->gruplarlistWidget->currentRow());
            delete silinecek_item;
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Stok gurubu silindi.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
            stokGruplariGetir();
        }
    }
    else
    {
        QMessageBox msg(this);
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
    this->close();
}

void StokGrupForm::stokGruplariGetir()
{
    ui->gruplarlistWidget->model()->removeRows(0, ui->gruplarlistWidget->count());
    QStringList liste = vt_grup->stokGruplariGetir();
    foreach (auto item, liste) {
        ui->gruplarlistWidget->addItem(item);
    }
}

void StokGrupForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
}

