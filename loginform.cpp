#include "loginform.h"
#include "ui_loginform.h"
#include "user.h"
#include "satisform.h"

#include <QtSql>
#include <QMessageBox>
#include <QCloseEvent>


QSqlDatabase db;
QSqlQuery sorgu;

LoginForm::LoginForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QPSQL", "db_login");
    db.setHostName("localhost");
    db.setDatabaseName("mhss_data");
    db.setUserName("admin");
    db.setPassword("admin");
    if(!db.isOpen())
    {
        db.open();
        getUsers();
    }
    else
    {
        getUsers();
    }
}

LoginForm::~LoginForm()
{
    delete ui;
}


void LoginForm::on_GirisBtn_clicked()
{
    sorgu.exec("SELECT kulID, kulUserName, kulPassword, kulAdi, kulCep FROM kullanicilar WHERE kulUserName='" + ui->CmBoxUserName->currentText() + "'");
    if(sorgu.next())
    {
        if(ui->LeditPass->text() == sorgu.value(2))
        {
            SatisForm *satis = new SatisForm();
            User *u = new User();
            u->setUserInfos(sorgu.value(0).toString(),sorgu.value(1).toString(),sorgu.value(3).toString(), sorgu.value(4).toString());
            satis->setUser(u);
            satis->show();
            this->close();
        }
        else
        {
            QMessageBox::warning(this,"Hata","Şifre hatalı. \nYeniden deneyin.",QMessageBox::Ok);
            ui->LeditPass->clear();
            ui->LeditPass->setFocus();
        }
    }
}


void LoginForm::on_KapatBtn_clicked()
{
    this->close();
}

void LoginForm::closeEvent(QCloseEvent *)
{
    if(db.isOpen())
    {
        db.removeDatabase("mhss_data");
        db.close();
    }
}

void LoginForm::getUsers()
{
    sorgu = QSqlQuery("SELECT kulUserName FROM kullanicilar", db);
        while (sorgu.next()) {
            ui->CmBoxUserName->addItem(sorgu.value(0).toString());
        }
}

