/*####################################
MIT LICENCE                          #
######################################
Copyright 2021 Mehmet AKDEMİR        #
bilgi@basat.dev                      #
######################################
Permission is hereby granted, free of charge,
to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "loginform.h"
#include "ui_loginform.h"
#include "user.h"
#include "satisform.h"
//*******************************
#include <QKeyEvent>
#include <QDirIterator>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QFileInfo>
#include <QProcess>

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
    ui->CmBoxUserName->setFocus();

    ui->labelVersion->setText(QApplication::applicationVersion());
}

void LoginForm::on_GirisBtn_clicked()
{
    if(vt.loginControl(ui->CmBoxUserName->currentText(), ui->LeditPass->text())){
        SatisForm *satis = new SatisForm();
        satis->setUser(vt.GetUserInfos(ui->CmBoxUserName->currentText()));
        satis->show();
        this->close();
    }
    else{
        uyariSesi->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Hata");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Şifre hatalı!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
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
    QThread::msleep(100);
    QFile capslockFile(capsLockFilePath);
    if(!capslockFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Unbuffered | QIODevice::ExistingOnly)){
        qCritical("%s", qPrintable(capslockFile.errorString()));
    }
    QTextStream stream(&capslockFile);
    bool state = stream.readLine().toInt();
    capslockFile.close();
    return state;
}

void LoginForm::setCapsLockFilePath(const QString &newCapsLockFilePath)
{
    capsLockFilePath = newCapsLockFilePath;
}

void LoginForm::on_toolButton_clicked()
{
    QProcess *systemCommand = new QProcess();
    if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
        systemCommand->start("onboard", QIODevice::ReadWrite);
    }
}
