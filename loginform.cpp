#include "loginform.h"
#include "ui_loginform.h"
#include "user.h"
#include "satisform.h"
//*******************************
#include <QKeyEvent>
#include <QDirIterator>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

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
    // farklı pclerde capslock dosya yolu değişebiliyor o yüzden önce dosya yolunu buluyorum.
    QString directory = "/sys/class/leds/"; // Where to search
    QDirIterator it(directory, QDirIterator::Subdirectories);

    // Iterate through the directory using the QDirIterator
    while (it.hasNext()) {
        QString filename = it.next();
        QFileInfo file(filename);

        if (file.isDir()) { // Check if it's a dir
            // If the filename contains target string - put it in the hitlist
            if (file.fileName().contains("capslock", Qt::CaseInsensitive)) {
                setCapsLockFilePath(file.absoluteFilePath().append("/brightness"));
                break;
            }
        }
    }
    ui->CmBoxUserName->addItems(vt.GetUsers());
    ui->label_6->setVisible(getCapslockState());
}

void LoginForm::on_GirisBtn_clicked()
{
    if(vt.loginControl(ui->CmBoxUserName->currentText(), ui->LeditPass->text())){
//        User u = vt.GetUserInfos(ui->CmBoxUserName->currentText());
        SatisForm *satis = new SatisForm();
        satis->setUser(vt.GetUserInfos(ui->CmBoxUserName->currentText()));
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

void LoginForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_CapsLock){
        ui->label_6->setVisible(getCapslockState());
    }
}

bool LoginForm::getCapslockState()
{
    QFileInfo file(capsLockFilePath);
    if(file.exists())
    {
        QFile capslock(capsLockFilePath);
        capslock.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream stream(&capslock);
        qDebug() << capslock << stream.readLine().toInt();
        bool state = stream.readAll().toInt();
        capslock.close();
        return state;
    }
}

void LoginForm::setCapsLockFilePath(const QString &newCapsLockFilePath)
{
    capsLockFilePath = newCapsLockFilePath;
}
