#include "ayarlardialog.h"
#include "ui_ayarlardialog.h"
#include "kullanicidialogform.h"
//**************************************
#include <QList>

AyarlarDialog::AyarlarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AyarlarDialog)
{
    ui->setupUi(this);

    formLoad();
}

AyarlarDialog::~AyarlarDialog()
{
    delete ui;
}

void AyarlarDialog::formLoad()
{
    ui->tabWidget->setCurrentIndex(0);
}

void AyarlarDialog::setCurrentUser(const User &newCurrentUser)
{
    if(newCurrentUser.getUserName() == "admin")
    {
        ui->tabWidget->setTabVisible(1, true);
        getUsers();
    }
    else{
        ui->tabWidget->setTabVisible(1, false);
    }
    currentUser = newCurrentUser;
}

void AyarlarDialog::getUsers()
{
    ui->KullanicilarlistWidget->clear();
    QList<QString> k = vt.GetUsers();
    foreach (QString item, k) {
        ui->KullanicilarlistWidget->addItem(item);
    }
}

void AyarlarDialog::on_YeniPushButton_clicked()
{
    KullaniciDialogForm *yeniKullaniciform = new KullaniciDialogForm(this);
    yeniKullaniciform->setYeniMi(true);
    yeniKullaniciform->exec();
    getUsers();
}


void AyarlarDialog::on_KullanicilarlistWidget_itemSelectionChanged()
{
    User u = vt.GetUserInfos(ui->KullanicilarlistWidget->currentItem()->text());
    ui->KasacheckBox->setChecked(u.getKasaYetki());
    ui->IadecheckBox->setChecked(u.getIadeYetki());
    ui->StokcheckBox->setChecked(u.getStokYetki());
}


void AyarlarDialog::on_DuzenlePushButton_clicked()
{
    if(ui->KullanicilarlistWidget->currentIndex().row() != -1){
        KullaniciDialogForm *yeniKullaniciform = new KullaniciDialogForm(this);
        yeniKullaniciform->setYeniMi(false);
        yeniKullaniciform->setDuzenlenecekUser(ui->KullanicilarlistWidget->currentItem()->text());
        yeniKullaniciform->exec();
        getUsers();
    }
    else{
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Listeden bir kullanıcı seçin.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}


void AyarlarDialog::on_SilPushButton_clicked()
{
    if(ui->KullanicilarlistWidget->currentIndex().row() != -1 && ui->KullanicilarlistWidget->currentItem()->text() != "admin"){
        vt.deleteUser(ui->KullanicilarlistWidget->currentItem()->text());
        getUsers();
    }
    else{
        QMessageBox msg;
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Listeden bir kullanıcı seçin.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

