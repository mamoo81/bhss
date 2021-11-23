#include "loginform.h"
#include "ui_loginform.h"
#include "user.h"
#include "satisform.h"
#include "veritabani.h"
//*******************************
#include <QCloseEvent>


LoginForm::LoginForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    formLoad();
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::formLoad()
{
    Veritabani vt;
    if(!vt.veritabaniVarmi())
    {
        QMessageBox msg(0);
        msg.setText("Veritabanı bulunamadı.\n\nSıfırdan oluşturmak ister misiniz?");
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Question);
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        int cevap = msg.exec();
        switch (cevap) {
        case QMessageBox::Yes:
            vt.veritabaniOlustur();
            break;
        }
    }
    getUsers();
}

void LoginForm::on_GirisBtn_clicked()
{
    Veritabani vt;
    if(vt.loginControl(ui->CmBoxUserName->currentText(), ui->LeditPass->text())){
        User u = vt.GetUserInfos(ui->CmBoxUserName->currentText());
        SatisForm *satis = new SatisForm();
        satis->setUser(u);
        satis->show();
        this->close();
    }
    else{
        ui->LeditPass->clear();
        ui->LeditPass->setFocus();
    }
}


void LoginForm::on_KapatBtn_clicked()
{
    this->close();
}

void LoginForm::closeEvent(QCloseEvent *)
{

}

void LoginForm::getUsers()
{
    Veritabani vt;
    QList<QString> kullanicilar = vt.GetUsers();
    foreach (QString item, kullanicilar) {
        ui->CmBoxUserName->addItem(item);
    }
}

