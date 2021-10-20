#include "loginform.h"
#include "ui_loginform.h"
#include "user.h"
#include "satisform.h"
#include "veritabani.h"

#include <QCloseEvent>

Veritabani vt;


LoginForm::LoginForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    getUsers();
}

LoginForm::~LoginForm()
{
    delete ui;
}


void LoginForm::on_GirisBtn_clicked()
{
    if(vt.loginControl(ui->CmBoxUserName->currentText(), ui->LeditPass->text())){
        SatisForm *satis = new SatisForm();
        User u = vt.GetUserInfos(ui->CmBoxUserName->currentText());
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
    vt.db.removeDatabase("mhss");
}

void LoginForm::getUsers()
{
    QList<QString> kullanicilar = vt.GetUsers();
    foreach (QString item, kullanicilar) {
        ui->CmBoxUserName->addItem(item);
    }
}

