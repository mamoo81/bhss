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

