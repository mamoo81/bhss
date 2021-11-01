#include "loginform.h"
#include "ui_loginform.h"
#include "user.h"
#include "satisform.h"

#include <QCloseEvent>

Veritabani vt;


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
    if(vt.db.isDriverAvailable("QPSQL")){
        if(vt.veritabaniVarmi()){
            getUsers();
        }
    }
    else{
        QMessageBox msg(this);
        msg.setText("Uyarı");
        msg.setInformativeText("Postgresql servisini kontrol ediniz.");
        msg.setIcon(QMessageBox::Warning);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.setModal(true);
        msg.exec();
    }
}

void LoginForm::on_GirisBtn_clicked()
{
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
    vt.db.removeDatabase("mhss");
}

void LoginForm::getUsers()
{
    QList<QString> kullanicilar = vt.GetUsers();
    foreach (QString item, kullanicilar) {
        ui->CmBoxUserName->addItem(item);
    }
}

