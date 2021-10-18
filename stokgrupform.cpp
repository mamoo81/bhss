#include "stokgrupform.h"
#include "ui_stokgrupform.h"

#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

QSqlDatabase db_grup;
QSqlQuery sorgu_grup(db_grup);

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
    db_grup = QSqlDatabase::addDatabase("QPSQL");
    db_grup.setDatabaseName("mhss_data");
    db_grup.setHostName("localhost");
    db_grup.setUserName("admin");
    db_grup.setPassword("admin");
    if(!db_grup.isOpen()){
        db_grup.open();
        stokGruplariGetir();
    }
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
            sorgu_grup = QSqlQuery(db_grup);
            sorgu_grup.prepare("INSERT INTO stokgruplari (id,grup) VALUES (nextval('stokgruplari_id'), ?)");
            sorgu_grup.bindValue(0, aranacak);
            sorgu_grup.exec();
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
        sorgu_grup = QSqlQuery(db_grup);
        sorgu_grup.prepare("DELETE FROM stokgruplari WHERE grup = ?");
        sorgu_grup.bindValue(0, silinecek_item->text());
        sorgu_grup.exec();

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
    sorgu_grup = QSqlQuery(db_grup);
    sorgu_grup.exec("SELECT grup FROM stokgruplari");
    ui->gruplarlistWidget->clear();
    while (sorgu_grup.next()) {
        ui->gruplarlistWidget->addItem(sorgu_grup.value(0).toString());
    }
}

void StokGrupForm::closeEvent(QCloseEvent *)
{
    if(db_grup.isOpen()){
        db_grup.close();
    }
}

void StokGrupForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
}

