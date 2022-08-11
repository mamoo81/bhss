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
#include "rafetiketidialog.h"
#include "ui_rafetiketidialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QMenu>
#include <QAction>
#include <QSettings>

RafEtiketiDialog::RafEtiketiDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RafEtiketiDialog)
{
    ui->setupUi(this);

    stokKartlariniListele();
    ui->aralineEdit->setFocus();
    // barkodda ara aktif olduğu için ilk etapta textchanged sinyalini iptal ediyorum. adında ara seçeneğine basınca connect ediyorum.
    disconnect(ui->aralineEdit, SIGNAL(textChanged(QString)), this, SLOT(on_aralineEdit_textChanged(QString)));

    key_left = new QShortcut(this);
    key_left->setKey(Qt::Key_Left);
    connect(key_left, SIGNAL(activated()), this, SLOT(ListedenCikar()));

    key_right = new QShortcut(this);
    key_right->setKey(Qt::Key_Right);
    connect(key_right, SIGNAL(activated()), this, SLOT(ListeyeEkle()));

    ui->progressBar->setVisible(false);

    //genel ayarların okunması başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    // etiket yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("etiket-yazici");
    ui->VarsayilanKagitcomboBox->setCurrentIndex(genelAyarlar.value("kagit").toInt());
    genelAyarlar.endGroup();
}

RafEtiketiDialog::~RafEtiketiDialog()
{
    delete ui;
}

void RafEtiketiDialog::stokKartlaricustomContextMenuRequested(QPoint pos)
{
    QMenu *menuStokKartlari = new QMenu(this);

    QAction *ekleAction = new QAction(this);
    ekleAction->setIcon(QIcon(":/images/ui/plus.png"));
    ekleAction->setFont(QFont("Monospace", 14));
    ekleAction->setText("Ekle");
    connect(ekleAction, SIGNAL(triggered(bool)), this, SLOT(on_eklepushButton_clicked()));
    menuStokKartlari->addAction(ekleAction);

    menuStokKartlari->popup(ui->stokKartlaritableView->viewport()->mapToGlobal(pos));

    ui->aralineEdit->setFocus();
}

void RafEtiketiDialog::yazdirilacaklarcustomContextMenuRequested(QPoint pos)
{
    QMenu *menuYazidirilacaklar = new QMenu(this);

    QAction *cikarAction = new QAction(this);
    cikarAction->setIcon(QIcon(":/images/ui/negative.png"));
    cikarAction->setFont(QFont("Monospace", 14));
    cikarAction->setText("Çıkar");
    connect(cikarAction, SIGNAL(triggered(bool)), this, SLOT(on_cikarpushButton_clicked()));
    menuYazidirilacaklar->addAction(cikarAction);

    menuYazidirilacaklar->popup(ui->yazdirilacaklartableWidget->viewport()->mapToGlobal(pos));

    ui->aralineEdit->setFocus();
}

void RafEtiketiDialog::stokKartlariniListele()
{
    ui->stokKartlaritableView->setModel(vtEtiket->getStokKartlariEtiket());
    ui->stokKartlaritableView->resizeColumnsToContents();
    connect(ui->stokKartlaritableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(stokKartlaricustomContextMenuRequested(QPoint)));
    connect(ui->yazdirilacaklartableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(yazdirilacaklarcustomContextMenuRequested(QPoint)));
}

void RafEtiketiDialog::ListeyeEkle()
{
    QModelIndexList secilenEklenecekStoklar = ui->stokKartlaritableView->selectionModel()->selectedRows();

    foreach (QModelIndex satir, secilenEklenecekStoklar) {
        QString barkod = satir.model()->index(satir.row(), 0).data().toString();
        QString stokAdi = satir.model()->index(satir.row(), 1).data().toString();

        if(ui->yazdirilacaklartableWidget->rowCount() == 0){
            ui->yazdirilacaklartableWidget->insertRow(ui->yazdirilacaklartableWidget->rowCount());
            ui->yazdirilacaklartableWidget->setItem(ui->yazdirilacaklartableWidget->rowCount() - 1, 0, new QTableWidgetItem(barkod));
            ui->yazdirilacaklartableWidget->setItem(ui->yazdirilacaklartableWidget->rowCount() - 1, 1, new QTableWidgetItem(stokAdi));
        }
        else{
            bool varmi = false;
            for (int var = 0; var < ui->yazdirilacaklartableWidget->rowCount(); ++var) {

                if(ui->yazdirilacaklartableWidget->model()->index(var, 0).data().toString() == barkod){
                    varmi = true;
                    break;
                }
            }

            if(!varmi){
                ui->yazdirilacaklartableWidget->insertRow(ui->yazdirilacaklartableWidget->rowCount());
                ui->yazdirilacaklartableWidget->setItem(ui->yazdirilacaklartableWidget->rowCount() - 1, 0, new QTableWidgetItem(barkod));
                ui->yazdirilacaklartableWidget->setItem(ui->yazdirilacaklartableWidget->rowCount() - 1, 1, new QTableWidgetItem(stokAdi));
            }
        }
    }
    ui->stokKartlaritableView->clearSelection();
    ui->yazdirilacaklartableWidget->selectRow(ui->yazdirilacaklartableWidget->rowCount() -1);
    ui->yazdirilacakAdetlabel->setText(QString::number(ui->yazdirilacaklartableWidget->rowCount()));
    ui->yazdirilacaklartableWidget->resizeColumnsToContents();
    ui->yazdirilacaklartableWidget->sortByColumn(1, Qt::AscendingOrder);
    ButonDurumlariniAyarla();
}

void RafEtiketiDialog::ListedenCikar()
{
    if(ui->yazdirilacaklartableWidget->rowCount() != 0 && ui->yazdirilacaklartableWidget->selectionModel()->hasSelection()){

        if(ui->HepsiYazdirilacakcheckBox->isChecked()){
            ui->yazdirilacaklartableWidget->model()->removeRows(0, ui->yazdirilacaklartableWidget->rowCount());
            ui->HepsiYazdirilacakcheckBox->setChecked(false);
            ui->yazdirilacakAdetlabel->setText(QString::number(ui->yazdirilacaklartableWidget->rowCount()));
            return;
        }
        QModelIndexList secilenSilinecekStoklar = ui->yazdirilacaklartableWidget->selectionModel()->selectedRows();

        if((secilenSilinecekStoklar.first().row() - 1) != -1){
            ui->yazdirilacaklartableWidget->selectRow(secilenSilinecekStoklar.first().row() - 1);
        }
        else if(ui->yazdirilacaklartableWidget->rowCount() > 0){
            ui->yazdirilacaklartableWidget->selectRow(secilenSilinecekStoklar.last().row() + 1);
        }

        foreach (QModelIndex satir, secilenSilinecekStoklar) {

            ui->yazdirilacaklartableWidget->removeRow(satir.row());
        }
    }

    ui->yazdirilacaklartableWidget->resizeColumnsToContents();
    ui->yazdirilacakAdetlabel->setText(QString::number(ui->yazdirilacaklartableWidget->rowCount()));
    ButonDurumlariniAyarla();
}

void RafEtiketiDialog::ButonDurumlariniAyarla()
{
    if(ui->yazdirilacaklartableWidget->rowCount() != 0){
        ui->yazdirtoolButton->setEnabled(true);
    }
    else{
        ui->yazdirtoolButton->setEnabled(false);
    }
}

void RafEtiketiDialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        on_arapushButton_clicked();
    }
}

void RafEtiketiDialog::on_eklepushButton_clicked()
{
    ListeyeEkle();
}

void RafEtiketiDialog::on_cikarpushButton_clicked()
{
    ListedenCikar();
}

void RafEtiketiDialog::on_yazdirtoolButton_clicked()
{
    uyariSes->play();
    QMessageBox msg(this);
    msg.setWindowTitle("Dikkat");
    msg.setIcon(QMessageBox::Information);
    msg.setText("Listedeki stok kartları yazıcıya gönderilecek. Emin misiniz?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    msg.setButtonText(QMessageBox::Yes, "Evet");
    msg.setButtonText(QMessageBox::No, "Hayır");
    int cevap = msg.exec();
    if(cevap == QMessageBox::Yes){
        ui->progressBar->setVisible(true);
        ui->DurdurtoolButton->setEnabled(true);
        ui->eklepushButton->setEnabled(false);
        ui->cikarpushButton->setEnabled(false);
        ui->HepsiYazdirilacakcheckBox->setEnabled(false);
        ui->yazdirilacaklartableWidget->setEnabled(false);
        ui->progressBar->setMaximum(ui->yazdirilacaklartableWidget->rowCount());
        ui->progressBar->setFormat("Gönderiliyor %p%");
        for (int var = 0; var < ui->yazdirilacaklartableWidget->rowCount(); ++var) {
            kartlar.append(vtEtiket->getStokKarti(ui->yazdirilacaklartableWidget->model()->index(var, 0).data().toString()));
            ui->progressBar->setValue(ui->progressBar->value() + 1);
        }
        etiketTHRD = new EtiketThread();
        connect(etiketTHRD, SIGNAL(yazdirilinca(int, QString)), this, SLOT(yazdirmaSinyaliAlininca(int, QString)));
        connect(etiketTHRD, SIGNAL(yazdirmaBitince()), this, SLOT(yazdirmaBittiSinyaliAlininca()));

        // yazıdırılacak kartların set edilmesi
        etiketTHRD->setKartlar(kartlar);

        //genel ayarlar
        QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
        // etiket yazıcı ayarları okuma başlangıç
        genelAyarlar.beginGroup("etiket-yazici");
        if(genelAyarlar.value("kagit").isNull() || genelAyarlar.value("kagit").isValid()){
            uyariSes->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Varsayılan kağıt ayarlardan okunamadı. \n\nÖnceden ayarlamamış olabilirsiniz.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }
        ui->VarsayilanKagitcomboBox->setCurrentIndex(genelAyarlar.value("kagit").toInt());
        genelAyarlar.endGroup();

        // yazdırma
        etiketTHRD->setKagitTipi(ui->VarsayilanKagitcomboBox->currentIndex());

        etiketTHRD->start();

        kartlar.clear();
    }
}

void RafEtiketiDialog::on_hepsiStokkartlaricheckBox_clicked()
{
    if(ui->hepsiStokkartlaricheckBox->isChecked()){
        ui->stokKartlaritableView->selectAll();
    }
    else{
        ui->stokKartlaritableView->clearSelection();
    }
}

void RafEtiketiDialog::on_HepsiYazdirilacakcheckBox_clicked()
{
    if(ui->HepsiYazdirilacakcheckBox->isChecked()){
        ui->yazdirilacaklartableWidget->selectAll();
    }
    else{
        ui->yazdirilacaklartableWidget->clearSelection();
    }
}

void RafEtiketiDialog::on_DurdurtoolButton_clicked()
{
    //genel ayarların okunması başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    // etiket yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("etiket-yazici");
    QString etiketYazici = genelAyarlar.value("yazici").toString();
    genelAyarlar.endGroup();

    etiketTHRD->quit();
    etiketTHRD->stop = true;
    ui->progressBar->setVisible(false);
    ui->DurdurtoolButton->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat("Gönderiliyor %p%");
    disconnect(etiketTHRD, SIGNAL(yazdirilinca(int, QString)), this, SLOT(yazdirmaSinyaliAlininca(int, QString)));

    QString yazdirmaKomut;
    QProcess *processIslem = new QProcess();
    if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){
//        yazdirmaKomut = "lpr -P " + etiketYazici + " /tmp/mhss-fis.pdf";
    }
    else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
        yazdirmaKomut = "cancel -a " + etiketYazici;
    }
    else{
        qDebug() << "Yazdırmayı Durdurmak için İşletim Sistemi tespit edilemedi.";
    }
    processIslem->start(yazdirmaKomut);
}

void RafEtiketiDialog::stokKartiAra(QString aranacakMetin)
{
    bool varmi = false;
    if(ui->barkodradioButton->isChecked()){
        for (int i = 0; i < ui->stokKartlaritableView->model()->rowCount(); ++i) {
            if(ui->stokKartlaritableView->model()->index(i, 0).data().toString() == aranacakMetin){
                ui->stokKartlaritableView->selectRow(i);
                varmi = true;
                break;
            }
        }
    }

    if(!varmi){
        uyariSes->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Stok kartı bulunamadı");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
        ui->stokKartlaritableView->clearSelection();
        ui->aralineEdit->clear();
    }
}

void RafEtiketiDialog::on_arapushButton_clicked()
{
    if(!ui->aralineEdit->text().isEmpty()){
        stokKartiAra(ui->aralineEdit->text());
    }
    ui->aralineEdit->setFocus();
    ui->aralineEdit->selectAll();
}

void RafEtiketiDialog::on_aralineEdit_textChanged(const QString &arg1)
{
    if(ui->adindaradioButton->isChecked()){

//        gosterilenSatirlar.clear();
        ui->stokKartlaritableView->clearSelection();
        for (int i = 0; i < ui->stokKartlaritableView->model()->rowCount(); ++i) {
            if(!ui->stokKartlaritableView->model()->index(i, 1).data().toString().contains(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(arg1), Qt::CaseInsensitive)){
                ui->stokKartlaritableView->hideRow(i);
            }
            else{
                ui->stokKartlaritableView->showRow(i);
//                gosterilenSatirlar.append(i);// aşağı/yukarı yön tuşları ile seçim yaparken gizlenen satırları es geçmek için gösterilen satırların indexini atıyorum.
            }
        }

//        if(gosterilenSatirlar.count() > 0){
//            ui->StokKartlaritableView->selectRow(gosterilenSatirlar.first());// gösterilen satırlardan ilk indexi seçiyorum.
//        }

//        if(arg1.count() == 0){// arama metni uzunluğu 0 ise satır seçimini ve son gösterilenindexi 0 yap
//            ui->StokKartlaritableView->selectRow(0);
//            sonSecilenGosterilenSatirIndexi = 0;
//        }
    }
}

void RafEtiketiDialog::on_adindaradioButton_clicked()
{
    ui->aralineEdit->setFocus();
    ui->aralineEdit->selectAll();
    connect(ui->aralineEdit, SIGNAL(textChanged(QString)), this, SLOT(on_aralineEdit_textChanged(QString)));
}

void RafEtiketiDialog::on_barkodradioButton_clicked()
{
    ui->aralineEdit->setFocus();
    ui->aralineEdit->selectAll();
    disconnect(ui->aralineEdit, SIGNAL(textChanged(QString)), this, SLOT(on_aralineEdit_textChanged(QString)));
}

void RafEtiketiDialog::on_TemizletoolButton_clicked()
{
    ui->yazdirilacaklartableWidget->model()->removeRows(0, ui->yazdirilacaklartableWidget->rowCount());
    ui->yazdirilacakAdetlabel->setText(QString::number(ui->yazdirilacaklartableWidget->rowCount()));
}

void RafEtiketiDialog::yazdirmaSinyaliAlininca(int value, QString barkod)
{
    Q_UNUSED(barkod);
    ui->progressBar->setValue(ui->progressBar->value() + value);
    ui->progressBar->setFormat("Yazdiriliyor %p%");
}

void RafEtiketiDialog::yazdirmaBittiSinyaliAlininca()
{
    qDebug() << "etiket yazdırma bitti";
    ui->DurdurtoolButton->setEnabled(false);
    ui->progressBar->setVisible(false);
    ui->eklepushButton->setEnabled(true);
    ui->cikarpushButton->setEnabled(true);
    ui->HepsiYazdirilacakcheckBox->setEnabled(true);
    ui->yazdirilacaklartableWidget->setEnabled(true);
    disconnect(etiketTHRD, SIGNAL(yazdirmaBitince()), this, SLOT(yazdirmaBittiSinyaliAlininca()));
}
