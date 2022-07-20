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
#include "resimekledialog.h"
#include "ui_resimekledialog.h"
//**********************************
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QMessageBox>

ResimEkleDialog::ResimEkleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResimEkleDialog)
{
    ui->setupUi(this);
}

ResimEkleDialog::~ResimEkleDialog()
{
    delete ui;
}

void ResimEkleDialog::on_kaydetpushButton_clicked()
{
    resimlerDizini = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/urunler-image/";
    if(!QFileInfo::exists(resimlerDizini)){
        QDir().mkdir(resimlerDizini);
    }

    QFile yeniResim(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/urunler-image/" + urunBarkod + "." + QFileInfo(resim->fileName()).completeSuffix());
    resim->copy(resim->fileName(), yeniResim.fileName());

    if(QFileInfo(yeniResim).exists()){
        QMessageBox msg(this);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Resim başarıyla kaydedildi.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        this->close();
    }
    else{
        QMessageBox msg(this);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Resim kaydedilemedi.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}


void ResimEkleDialog::on_secpushButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Resim dosyasını seçiniz");
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Resim dosyası (*.png)"));
    dialog.setLabelText(QFileDialog::Accept, tr("Seç"));
    dialog.setLabelText(QFileDialog::Reject, tr("Vazgeç"));
    dialog.setLabelText(QFileDialog::FileName, tr("Dosya"));
    dialog.setLabelText(QFileDialog::FileType, tr("Dosya tipi"));
    dialog.exec();
    if(!dialog.selectedFiles().isEmpty()){
        resim = new QFile(dialog.selectedFiles().first());
        ui->label->setPixmap(dialog.selectedFiles().first());
    }
}

void ResimEkleDialog::setUrunBarkod(const QString &newUrunBarkod)
{
    urunBarkod = newUrunBarkod;
}


void ResimEkleDialog::on_iptalpushButton_clicked()
{
    this->close();
}

