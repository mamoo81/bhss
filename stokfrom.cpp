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
#include "stokfrom.h"
#include "ui_stokfrom.h"
#include "stokgrupform.h"
#include "veritabani.h"
#include "user.h"
#include "birimekleform.h"
#include "stokgircikdialog.h"
#include "toplustokyukledialog.h"
#include "stokhareketleridialog.h"
#include "resimekledialog.h"
#include "stokkartlarimodel.h"
#include "yazici.h"
#include "rafetiketidialog.h"
//***************************
#include <QSqlQueryModel>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QDateTime>
#include <QModelIndex>
#include <QSettings>
#include <QStandardPaths>
#include <QRandomGenerator>
#include <QString>
#include <QMenu>

#include <QScroller>

//QItemSelectionModel *seciliSatirModel;
//int seciliSatirIndex;

StokFrom::StokFrom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StokFrom)
{
    ui->setupUi(this);

    formLoad();
}

StokFrom::~StokFrom()
{
    delete ui;
}

void StokFrom::on_StokGrupBtn_clicked()
{
    StokGrupForm *stkgrupform = new StokGrupForm(this);
    stkgrupform->setModal(true);
    stkgrupform->show();
}

void StokFrom::formLoad()
{
    grupComboboxDoldur();
    stokKartlariniListele();
    // stok birimlerini çekme
    ui->BirimiComboBox->clear();
    ui->BirimiComboBox->addItems(vt->getStokBirimleri());
    // üreticileri çekme
    ui->ureticicomboBox->addItems(vt->getUreticiler());
    // tedarikçileri getirme
    ui->tedarikcicomboBox->addItems(vt->getTedarikciler());

    // klavye kısayol tanımlamaları
    CTRL_F = new QShortcut(this);
    CTRL_F->setKey(Qt::CTRL + Qt::Key_F);
    connect(CTRL_F, SIGNAL(activated()), this, SLOT(CTRL_F_Slot()));

    key_Down = new QShortcut(this);
    key_Down->setKey(Qt::Key_Down);
    connect(key_Down, SIGNAL(activated()), this, SLOT(key_Down_Slot()));

    key_DownArrow = new QShortcut(this);
    key_DownArrow->setKey(Qt::DownArrow);
    connect(key_DownArrow, SIGNAL(activated()), this, SLOT(key_Down_Slot()));

    key_Up = new QShortcut(this);
    key_Up->setKey(Qt::Key_Up);
    connect(key_Up, SIGNAL(activated()), this, SLOT(key_UP_Slot()));

    key_UpArrow = new QShortcut(this);
    key_UpArrow->setKey(Qt::UpArrow);
    connect(key_UpArrow, SIGNAL(activated()), this, SLOT(key_UP_Slot()));

    key_F1 = new QShortcut(this);
    key_F1->setKey(Qt::Key_F1);
    connect(key_F1, SIGNAL(activated()), this, SLOT(key_F1_Slot()));

    key_F2 = new QShortcut(this);
    key_F2->setKey(Qt::Key_F2);
    connect(key_F2, SIGNAL(activated()), this, SLOT(key_F2_Slot()));

    key_F3 = new QShortcut(this);
    key_F3->setKey(Qt::Key_F3);
    connect(key_F3, SIGNAL(activated()), this, SLOT(key_F3_Slot()));

    key_F4 = new QShortcut(this);
    key_F4->setKey(Qt::Key_F4);
    connect(key_F4, SIGNAL(activated()), this, SLOT(key_F4_Slot()));

    key_F5 = new QShortcut(this);
    key_F5->setKey(Qt::Key_F5);
    connect(key_F5, SIGNAL(activated()), this, SLOT(key_F5_Slot()));

    key_F6 = new QShortcut(this);
    key_F6->setKey(Qt::Key_F6);
    connect(key_F6, SIGNAL(activated()), this, SLOT(key_F6_Slot()));

    QScroller::grabGesture(ui->StokKartlaritableView, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->ureticicomboBox, QScroller::LeftMouseButtonGesture);

    regEXPbarkod = QRegExp("[0-9]{8,13}");
    ui->BarkodLnEdit->setValidator(new QRegExpValidator(regEXPbarkod, this));

    regEXPstokKod = QRegExp("[a-zöçşiğüA-ZÖÇŞİĞÜ0-9]{3,}");
    ui->StokKoduLnEdit->setValidator(new QRegExpValidator(regEXPstokKod, this));

    regEXPstokAd = QRegExp("[a-zöçşiğü A-ZÖÇŞİĞÜ 0-9]{3,}");
    ui->StokAdiLnEdit->setValidator(new QRegExpValidator(regEXPstokAd, this));

    regEXPstokMiktar = new QDoubleValidator(0, 999999999, 10, this);
    regEXPstokMiktar->setNotation(QDoubleValidator::StandardNotation);
    regEXPstokMiktar->setLocale(QLocale::C);
    ui->MiktarLnEdit->setValidator(regEXPstokMiktar);

    TextColorPaletteDefault = ui->BarkodLnEdit->style()->standardPalette();

    TextColorPaletteRed = QPalette();
    TextColorPaletteRed.setColor(QPalette::Base, Qt::red);
    TextColorPaletteRed.setColor(QPalette::Text, Qt::black);

    ui->AraLineEdit->setFocus();
}

void StokFrom::setUser(User user)
{
    kullanici = user;
    this->setWindowTitle("STOK KARTLARI - " + kullanici.getUserName());
}

void StokFrom::customMenuRequested(QPoint pos)
{
    // satırı böyle bu void içinde de alabilirsin.
    //QModelIndex index = ui->StokKartlaritableView->indexAt(pos);

    QMenu *menu = new QMenu(this);

    QAction *etiketYazdirAction = new QAction(this);
    etiketYazdirAction->setIcon(QIcon(":/images/ui/label-printer.png"));
    etiketYazdirAction->setFont(QFont("Monospace", 12));
    etiketYazdirAction->setText("Raf etiketi yazdır");
    etiketYazdirAction->setShortcut(QKeySequence(Qt::Key_F6));
    connect(etiketYazdirAction, SIGNAL(triggered(bool)), this, SLOT(hizliRafEtiketiYazdir()));
    menu->addAction(etiketYazdirAction);

    QAction *duzenleAction = new QAction(this);
    duzenleAction->setIcon(QIcon(":/images/ui/edit-property-100.png"));
    duzenleAction->setFont(QFont("Monospace", 12));
    duzenleAction->setText("Düzenle");
    duzenleAction->setShortcut(QKeySequence(Qt::Key_F2));
    connect(duzenleAction, SIGNAL(triggered(bool)), this, SLOT(key_F2_Slot()));
    menu->addAction(duzenleAction);

    QAction *silAction = new QAction(this);
    silAction->setIcon(QIcon(":/images/ui/delete-document-100.png"));
    silAction->setFont(QFont("Monospace", 12));
    silAction->setText("Sil");
    silAction->setShortcut(QKeySequence(Qt::Key_F3));
    connect(silAction, SIGNAL(triggered(bool)), this, SLOT(key_F3_Slot()));
    menu->addAction(silAction);

    QAction *stokGirAction = new QAction(this);
    stokGirAction->setIcon(QIcon(":/images/ui/plus.png"));
    stokGirAction->setFont(QFont("Monospace", 12));
    stokGirAction->setText("Stok Gir");
    stokGirAction->setShortcut(QKeySequence(Qt::Key_F4));
    connect(stokGirAction, SIGNAL(triggered(bool)), this, SLOT(on_StokGirBtn_clicked()));
    menu->addAction(stokGirAction);

    QAction *stokCikAction = new QAction(this);
    stokCikAction->setIcon(QIcon(":/images/ui/negative.png"));
    stokCikAction->setFont(QFont("Monospace", 12));
    stokCikAction->setText("Stok Çık");
    stokCikAction->setShortcut(QKeySequence(Qt::Key_F5));
    connect(stokCikAction, SIGNAL(triggered(bool)), this, SLOT(on_StokCikBtn_clicked()));
    menu->addAction(stokCikAction);

    menu->popup(ui->StokKartlaritableView->viewport()->mapToGlobal(pos));
}

void StokFrom::hizliRafEtiketiYazdir()
{
    Yazici *yazici = new Yazici();
    yazici->rafEtiketiBas(vt->getStokKarti(ui->BarkodLnEdit->text()));
    delete yazici;
}

void StokFrom::grupComboboxDoldur()
{
    QStringList gruplar = vt->stokGruplariGetir();
    foreach (auto grup, gruplar) {
        ui->StokGrubuComboBox->addItem(grup);
    }
    ui->StokGrubuComboBox->setCurrentIndex(0);
}


void StokFrom::stokKartlariniListele()
{
    _stokKartlariTableModel = new StokKartlariModel();
    sortModel->setSourceModel(_stokKartlariTableModel);
    ui->StokKartlaritableView->setModel(sortModel);
    ui->StokKartlaritableView->sortByColumn(3, Qt::AscendingOrder);
    ui->StokKartlaritableView->resizeColumnsToContents();
    ui->StokKartlaritableView->setSortingEnabled(true);
    connect(ui->StokKartlaritableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),SLOT(alanlariDoldur()));
    connect(ui->StokKartlaritableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));
    ui->StokKartiAdetLabel->setText(QString::number(ui->StokKartlaritableView->model()->rowCount()));
}

void StokFrom::on_YeniBtn_clicked()
{
    if(!ui->BarkodLnEdit->isEnabled())
    {
        //butonlar
        ui->YeniBtn->setEnabled(false);
        ui->DuzenleBtn->setEnabled(false);
        ui->SilBtn->setEnabled(false);
        ui->StokKartlaritableView->setEnabled(false);
        ui->AraLineEdit->setEnabled(false);
        ui->araBtn->setEnabled(false);
        ui->StokGirBtn->setEnabled(false);
        ui->StokCikBtn->setEnabled(false);
        //textbox ve butonlar
        ui->BarkodLnEdit->setEnabled(true);
        ui->BarkodOlusturBtn->setEnabled(true);
        ui->StokKoduLnEdit->setEnabled(true);
        ui->StokAdiLnEdit->setEnabled(true);
        ui->BirimiComboBox->setEnabled(true);
        ui->MiktarLnEdit->setEnabled(true);
        ui->StokGrubuComboBox->setEnabled(true);
        ui->ureticicomboBox->setEnabled(true);
        ui->tedarikcicomboBox->setEnabled(true);
        ui->AFiyatdoubleSpinBox->setEnabled(true);
        ui->SFiyatdoubleSpinBox->setEnabled(true);
        ui->KDVspinBox->setEnabled(true);
        ui->OTVspinBox->setEnabled(true);
        ui->KDVcheckbox->setEnabled(true);
        ui->OTVcheckbox->setEnabled(true);
        ui->ResimEkleBtn->setEnabled(true);
        ui->ResimSilBtn->setEnabled(true);
        ui->UrunResimlabel->setPixmap(QPixmap(":/images/ui/box.png"));
        ui->SurekliYenicheckBox->setEnabled(true);
        ui->KaydetBtn->setEnabled(true);
        ui->IptalBtn->setEnabled(true);
        yeniKayit = true;
    }
    alanlariTemizle();
    ui->BarkodLnEdit->setFocus();
}

void StokFrom::on_DuzenleBtn_clicked()
{
    seciliIndex = ui->StokKartlaritableView->currentIndex().row();
    if(ui->StokKartlaritableView->currentIndex().row() > -1)
    {
        if(!ui->BarkodLnEdit->isEnabled())
        {
            //butonlar
            ui->YeniBtn->setEnabled(false);
            ui->DuzenleBtn->setEnabled(false);
            ui->SilBtn->setEnabled(false);
            ui->StokKartlaritableView->setEnabled(false);
            ui->AraLineEdit->setEnabled(false);
            ui->araBtn->setEnabled(false);
            ui->StokGirBtn->setEnabled(false);
            ui->StokCikBtn->setEnabled(false);
            ui->MiktarLnEdit->setEnabled(false);
            //textbox ve butonlar
            ui->BarkodLnEdit->setEnabled(true);
            ui->StokKoduLnEdit->setEnabled(true);
            ui->StokAdiLnEdit->setEnabled(true);
            ui->BirimiComboBox->setEnabled(true);
            ui->ureticicomboBox->setEnabled(true);
            ui->tedarikcicomboBox->setEnabled(true);
            ui->StokGrubuComboBox->setEnabled(true);
            ui->AFiyatdoubleSpinBox->setEnabled(true);
            ui->SFiyatdoubleSpinBox->setEnabled(true);
            ui->KDVspinBox->setEnabled(true);
            ui->OTVspinBox->setEnabled(true);
            ui->KDVcheckbox->setEnabled(true);
            ui->OTVcheckbox->setEnabled(true);
            ui->ResimEkleBtn->setEnabled(true);
            ui->ResimSilBtn->setEnabled(true);
            ui->KaydetBtn->setEnabled(true);
            ui->IptalBtn->setEnabled(true);
            ui->BarkodLnEdit->setFocus();
            yeniKayit = false;
        }
    }
    else
    {
        uyariSes->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Bir stok kartı seçiniz.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    //QItemSelectionModel *select = ui->StokKartlaritableView->selectionModel();
    //if(select.hasSelection()){
    //      böylede yapabilirsin.
    //}
}


void StokFrom::on_IptalBtn_clicked()
{
    alanlariTemizle();
    ui->YeniBtn->setEnabled(true);
    ui->DuzenleBtn->setEnabled(true);
    ui->SilBtn->setEnabled(true);
    ui->StokKartlaritableView->setEnabled(true);
    ui->AraLineEdit->setEnabled(true);
    ui->araBtn->setEnabled(true);
    ui->StokGirBtn->setEnabled(true);
    ui->StokCikBtn->setEnabled(true);

    ui->BarkodLnEdit->setEnabled(false);
    ui->BarkodOlusturBtn->setEnabled(false);
    ui->StokKoduLnEdit->setEnabled(false);
    ui->StokAdiLnEdit->setEnabled(false);
    ui->BirimiComboBox->setEnabled(false);
    ui->MiktarLnEdit->setEnabled(false);
    ui->StokGrubuComboBox->setEnabled(false);
    ui->ureticicomboBox->setEnabled(false);
    ui->tedarikcicomboBox->setEnabled(false);
    ui->AFiyatdoubleSpinBox->setEnabled(false);
    ui->SFiyatdoubleSpinBox->setEnabled(false);
    ui->KDVspinBox->setEnabled(false);
    ui->OTVspinBox->setEnabled(false);
    ui->KDVcheckbox->setEnabled(false);
    ui->OTVcheckbox->setEnabled(false);
    ui->ResimEkleBtn->setEnabled(false);
    ui->ResimSilBtn->setEnabled(false);
    ui->UrunResimlabel->setPixmap(QPixmap(":/images/ui/box.png"));
    ui->SurekliYenicheckBox->setEnabled(false);
    ui->KaydetBtn->setEnabled(false);
    ui->IptalBtn->setEnabled(false);
    ui->AraLineEdit->setFocus();
    alanlariDoldur();
}

void StokFrom::alanlariTemizle()
{
    ui->BarkodLnEdit->clear();
    ui->BarkodLnEdit->setPalette(TextColorPaletteDefault);
    ui->StokKoduLnEdit->clear();
    ui->StokKoduLnEdit->setPalette(TextColorPaletteDefault);
    ui->StokAdiLnEdit->clear();
    ui->StokAdiLnEdit->setPalette(TextColorPaletteDefault);
    ui->BirimiComboBox->setCurrentIndex(0);
    ui->MiktarLnEdit->clear();
    ui->MiktarLnEdit->setPalette(TextColorPaletteDefault);
    ui->StokGrubuComboBox->setCurrentIndex(0);
    ui->AFiyatdoubleSpinBox->setValue(0);
    ui->AFiyatdoubleSpinBox->setPalette(TextColorPaletteDefault);
    ui->SFiyatdoubleSpinBox->setValue(0);
    ui->SFiyatdoubleSpinBox->setPalette(TextColorPaletteDefault);
    ui->KDVspinBox->setValue(0);
    ui->AciklamaLnEdit->clear();
    ui->UrunResimlabel->setPixmap(QPixmap(":/images/ui/box.png"));
}


void StokFrom::alanlariDoldur()
{
    seciliSatirIndex = ui->StokKartlaritableView->currentIndex().row();
    seciliSatirModel = ui->StokKartlaritableView->selectionModel();
    ui->BarkodLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 1).data().toString());
    ui->StokKoduLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 2).data().toString());
    ui->StokAdiLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 3).data().toString());
    for (int i = 0; i <ui->BirimiComboBox->count(); ++i)
    {
        int result = QString::compare(ui->BirimiComboBox->itemText(i), seciliSatirModel->model()->index(seciliSatirIndex, 4).data().toString(), Qt::CaseInsensitive);
        if(result == 0)
        {
            ui->BirimiComboBox->setCurrentIndex(i);
            break;
        }
    }
    ui->MiktarLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 5).data().toString());
    for (int i = 0; i <ui->StokGrubuComboBox->count(); ++i)
    {
        int result = QString::compare(ui->StokGrubuComboBox->itemText(i), seciliSatirModel->model()->index(seciliSatirIndex, 6).data().toString(), Qt::CaseInsensitive);
        if(result == 0)
        {
            ui->StokGrubuComboBox->setCurrentIndex(i);
            break;
        }
    }
    ui->AFiyatdoubleSpinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 7).data().toDouble());
    ui->SFiyatdoubleSpinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 8).data().toDouble());
    ui->KDVspinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 9).data().toString().remove("%", Qt::CaseInsensitive).toInt());
    ui->OTVspinBox->setValue(seciliSatirModel->model()->index(seciliSatirIndex, 10).data().toString().remove("%", Qt::CaseInsensitive).toInt());
    if(seciliSatirModel->model()->index(seciliSatirIndex, 11).data().toString() == "Evet"){
        ui->KDVcheckbox->setChecked(true);
    }
    else{
        ui->KDVcheckbox->setChecked(false);
    }
    if(seciliSatirModel->model()->index(seciliSatirIndex, 12).data().toString() == "Evet"){
        ui->OTVcheckbox->setChecked(true);
    }
    else{
        ui->OTVcheckbox->setChecked(false);
    }
    ui->ureticicomboBox->setCurrentIndex(ui->ureticicomboBox->findText(seciliSatirModel->model()->index(seciliSatirIndex, 14).data().toString()));
    ui->tedarikcicomboBox->setCurrentIndex(ui->tedarikcicomboBox->findText(seciliSatirModel->model()->index(seciliSatirIndex, 15).data().toString()));
    ui->AciklamaLnEdit->setText(seciliSatirModel->model()->index(seciliSatirIndex, 16).data().toString());
    ui->UrunResimlabel->setPixmap(vt->getStokKarti(seciliSatirModel->model()->index(seciliSatirIndex, 1).data().toString()).getResim());
}

void StokFrom::closeEvent(QCloseEvent *)
{

}

void StokFrom::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        if(ui->KaydetBtn->isEnabled() && ui->IptalBtn->isEnabled()){
            uyariSes->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Question);
            msg.setText("Stok kartını düzenlemeyi bitirmediniz.\nYinede bitirmek istiyor musunuz?");
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setDefaultButton(QMessageBox::Yes);
            msg.setButtonText(QMessageBox::Yes, "Evet");
            msg.setButtonText(QMessageBox::No, "Hayır");
            int cevap = msg.exec();
            if(QMessageBox::Yes == cevap){
                on_IptalBtn_clicked();
            }
        }
        else{
            this->close();
        }
    }
    else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        if(ui->AraLineEdit->isEnabled()){
            on_araBtn_clicked();
        }
        else if(ui->KaydetBtn->isEnabled()){
            on_KaydetBtn_clicked();
        }
    }
}


void StokFrom::on_KaydetBtn_clicked()
{
    QMessageBox msg(this);
    msg.setWindowTitle("Uyarı");
    msg.setIcon(QMessageBox::Warning);
    msg.setText("Kırmızı renkli alanları kontrol ederek düzeltin/doldurun.");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, "Tamam");
    if(ui->SFiyatdoubleSpinBox->value() < ui->AFiyatdoubleSpinBox->value()){
        uyariSes->play();
        msg.setInformativeText("\n\nSatış fiyatı alış fiyatından düşük olamaz.");
        msg.exec();
        return;
    }
    if(!regEXPbarkod.exactMatch(ui->BarkodLnEdit->text())){
        uyariSes->play();
        msg.setInformativeText("\n\nBarkod numarası uygun formatta değil.");
        msg.exec();
        return;
    }
    if(!regEXPstokAd.exactMatch(ui->StokKoduLnEdit->text())){
        uyariSes->play();
        msg.setInformativeText("\n\nStok adı uygun formatta değil.");
        msg.exec();
        return;
    }
    if(!regEXPstokAd.exactMatch(ui->StokAdiLnEdit->text())){
        uyariSes->play();
        msg.setInformativeText("\n\nStok kodu uygun formatta değil.");
        msg.exec();
        return;
    }
    if(ui->BirimiComboBox->currentIndex() <= 0){
        uyariSes->play();
        msg.setInformativeText("\n\nStok birimini seçiniz.");
        msg.exec();
        return;
    }
    if(ui->StokGrubuComboBox->currentIndex() <= 0){
        uyariSes->play();
        msg.setInformativeText("\n\nStok gurubunu seçiniz.");
        msg.exec();
        return;
    }
    if(yeniKayit){// true ise yeni stok kartı kaydı oluşturur.
        if(!vt->barkodVarmi(ui->BarkodLnEdit->text())){
            StokKarti yeniStokKarti = StokKarti();
            yeniStokKarti.setBarkod(ui->BarkodLnEdit->text());
            yeniStokKarti.setKod(ui->StokKoduLnEdit->text());
            yeniStokKarti.setAd(QLocale().toUpper(ui->StokAdiLnEdit->text()));
            yeniStokKarti.setBirim(vt->getBirimID(ui->BirimiComboBox->currentText()));
            yeniStokKarti.setMiktar(ui->MiktarLnEdit->text().toFloat());
            yeniStokKarti.setGrup(vt->getGrupID(ui->StokGrubuComboBox->currentText()));
            yeniStokKarti.setAFiyat(ui->AFiyatdoubleSpinBox->value());
            yeniStokKarti.setSFiyat(ui->SFiyatdoubleSpinBox->value());
            yeniStokKarti.setKdv(ui->KDVspinBox->value());
            yeniStokKarti.setOtv(ui->OTVspinBox->value());
            yeniStokKarti.setKdvdahil(ui->KDVcheckbox->isChecked());
            yeniStokKarti.setOtvdahil(ui->OTVcheckbox->isChecked());
            yeniStokKarti.setTarih(QDateTime::currentDateTime());
            yeniStokKarti.setUretici(QString::number(vt->getUreticiID(ui->ureticicomboBox->currentText())));
            yeniStokKarti.setTedarikci(QString::number(vt->getTedarikciID(ui->tedarikcicomboBox->currentText())));
            yeniStokKarti.setAciklama(QLocale().toUpper("stok kartı oluşturuldu"));
            QSqlError hataMesajı = vt->yeniStokKartiOlustur(yeniStokKarti, &kullanici);
            if(!hataMesajı.isValid()){
                uyariSes->play();
                QMessageBox *msg = new QMessageBox(this);
                msg->setIcon(QMessageBox::Information);
                msg->setWindowTitle("Başarılı");
                msg->setText("Yeni stok kartı oluşturuldu.");
                msg->setStandardButtons(QMessageBox::Ok);
                msg->setDefaultButton(QMessageBox::Ok);
                msg->setButtonText(QMessageBox::Ok, "Tamam");
                msg->exec();
            }
            else{
                uyariSes->play();
                QMessageBox *msg = new QMessageBox(this);
                msg->setIcon(QMessageBox::Critical);
                msg->setWindowTitle("Hata");
                msg->setText("Yeni stok kartı oluşturulamadı.");
                msg->setInformativeText(qPrintable(hataMesajı.text()));
                msg->setStandardButtons(QMessageBox::Ok);
                msg->setDefaultButton(QMessageBox::Ok);
                msg->setButtonText(QMessageBox::Ok, "Tamam");
                msg->exec();
            }

            alanlariTemizle();
            stokKartlariniListele();
            alanlariDoldur();
            ui->StokKartlaritableView->selectRow(seciliIndex);
            ui->AraLineEdit->setFocus();
            ui->AraLineEdit->selectAll();
            emit on_IptalBtn_clicked();
        }
        else{
            uyariSes->play();
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Bu barkod veritabanında başka bir stok kartına tanımlı!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
    else if(!yeniKayit){// yenikayit false ise mevcut stok kartını günceller.

        seciliSatirIndex = ui->StokKartlaritableView->currentIndex().row();
        seciliSatirModel = ui->StokKartlaritableView->selectionModel();
        QString duzenlenecekStokKartiID(seciliSatirModel->model()->index(seciliSatirIndex, 0).data().toString());
        StokKarti yeniStokKarti = StokKarti();
        yeniStokKarti.setId(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 0).data().toString());
        yeniStokKarti.setBarkod(ui->BarkodLnEdit->text());
        yeniStokKarti.setAd(QLocale().toUpper(ui->StokAdiLnEdit->text()));
        yeniStokKarti.setBirim(vt->getBirimID(ui->BirimiComboBox->currentText()));
        yeniStokKarti.setMiktar(ui->MiktarLnEdit->text().toFloat());
        yeniStokKarti.setGrup(vt->getGrupID(ui->StokGrubuComboBox->currentText()));
        yeniStokKarti.setAFiyat(ui->AFiyatdoubleSpinBox->value());
        yeniStokKarti.setSFiyat(ui->SFiyatdoubleSpinBox->value());
        yeniStokKarti.setKdv(ui->KDVspinBox->value());
        yeniStokKarti.setOtv(ui->OTVspinBox->value());
        yeniStokKarti.setKdvdahil(ui->KDVcheckbox->isChecked());
        yeniStokKarti.setOtvdahil(ui->OTVcheckbox->isChecked());
        yeniStokKarti.setTarih(QDateTime::currentDateTime());
        yeniStokKarti.setUretici(QString::number(vt->getUreticiID(ui->ureticicomboBox->currentText())));
        yeniStokKarti.setTedarikci(QString::number(vt->getTedarikciID(ui->tedarikcicomboBox->currentText())));
        yeniStokKarti.setAciklama("stok kartı güncelleme");
        vt->stokKartiniGuncelle(duzenlenecekStokKartiID, yeniStokKarti, &kullanici);

        // düzenlenen stok kartı hızlı ürün butonlarına ekliyse ini dosyası ve buton bilgisini düzeltme başlangıcı
        vt->setHizliButon(yeniStokKarti);

        alanlariTemizle();
        stokKartlariniListele();
        alanlariDoldur();
        ui->StokKartlaritableView->selectRow(seciliIndex);
        ui->AraLineEdit->setFocus();
        ui->AraLineEdit->selectAll();
        emit on_IptalBtn_clicked();
    }
}


void StokFrom::on_SilBtn_clicked()
{
    if(ui->StokKartlaritableView->currentIndex().row() != -1){
        seciliSatirIndex = ui->StokKartlaritableView->currentIndex().row();
        seciliSatirModel = ui->StokKartlaritableView->selectionModel();
        uyariSes->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Question);
        msg.setText(QString("%1 barkod numaralı \n%2 isimli stok kartını silmek istediğinize emin misiniz?").arg(seciliSatirModel->model()->index(seciliSatirIndex, 1).data().toString()).arg(seciliSatirModel->model()->index(seciliSatirIndex, 3).data().toString()));
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        msg.setDefaultButton(QMessageBox::Yes);
        int cevap = msg.exec();
        if(cevap == QMessageBox::Yes){
            vt->stokKartiSil(seciliSatirModel->model()->index(seciliSatirIndex, 0).data().toString());
            stokKartlariniListele();
            emit on_IptalBtn_clicked();
        }
    }
    else{
        uyariSes->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Bir stok kartı seçiniz");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
    }
    ui->StokKartlaritableView->setFocus();
}

void StokFrom::stokKartiAra(QString aranacakMetin)
{
    bool varmi = false;
    if(ui->barkodRadioButton->isChecked()){
        for (int i = 0; i < ui->StokKartlaritableView->model()->rowCount(); ++i) {
            if(ui->StokKartlaritableView->model()->index(i, 1).data().toString() == aranacakMetin){
                ui->StokKartlaritableView->selectRow(i);
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
        ui->StokKartlaritableView->clearSelection();
        StokFrom::alanlariTemizle();
        ui->AraLineEdit->clear();
    }
}


void StokFrom::on_araBtn_clicked()
{
    stokKartiAra(ui->AraLineEdit->text());
    ui->AraLineEdit->setFocus();
    ui->AraLineEdit->selectAll();
}




void StokFrom::on_dosyadanToolButton_clicked()
{
    TopluStokYukleDialog *stokYukleForm = new TopluStokYukleDialog(this);
    stokYukleForm->exec();
    stokKartlariniListele();
    delete stokYukleForm;
}


void StokFrom::on_StokGirBtn_clicked()
{
    int seciliStokIndex = ui->StokKartlaritableView->currentIndex().row();

    if(ui->StokKartlaritableView->selectionModel()->hasSelection()){
        StokGirCikDialog *stokMiktarigirForm = new StokGirCikDialog(this);
        stokMiktarigirForm->setKullanici(kullanici);
        stokMiktarigirForm->setWindowTitle("Stok Gir");
        stokMiktarigirForm->setIslem("GİRİŞ");
        stokMiktarigirForm->setStokKartiID(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 0).data().toString());
        stokMiktarigirForm->exec();
        stokKartlariniListele();
        delete stokMiktarigirForm;
        ui->StokKartlaritableView->selectRow(seciliStokIndex);
    }
    else{
        uyariSes->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Stok kartı seçiniz!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Evet");
        msg.exec();
    }
}


void StokFrom::on_StokCikBtn_clicked()
{
    int seciliStokIndex = ui->StokKartlaritableView->currentIndex().row();

    if(ui->StokKartlaritableView->selectionModel()->hasSelection()){
        StokGirCikDialog *stokMiktarigirForm = new StokGirCikDialog(this);
        stokMiktarigirForm->setKullanici(kullanici);
        stokMiktarigirForm->setWindowTitle("Stok Çık");
        stokMiktarigirForm->setIslem("ÇIKIŞ");
        stokMiktarigirForm->setStokKartiID(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 0).data().toString());
        stokMiktarigirForm->exec();
        stokKartlariniListele();
        delete stokMiktarigirForm;
        ui->StokKartlaritableView->selectRow(seciliStokIndex);
    }
    else{
        uyariSes->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Stok kartı seçiniz!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Evet");
        msg.exec();
    }
}


void StokFrom::on_StokKartlaritableView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    StokHareketleriDialog *hareketlerForm = new StokHareketleriDialog(this);
    hareketlerForm->setStokBarkod(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString());
    hareketlerForm->exec();
    delete hareketlerForm;
}


void StokFrom::on_BarkodOlusturBtn_clicked()
{
    QString uretilenBarkod(QString::number(QRandomGenerator::global()->bounded(80000000, 89999999)));
    if(!vt->barkodVarmi(uretilenBarkod)){
        ui->BarkodLnEdit->setText(uretilenBarkod);
    }
}


void StokFrom::on_AraLineEdit_textChanged(const QString &arg1)
{
    if(ui->adRadioButton->isChecked()){

        gosterilenSatirlar.clear();
        ui->StokKartlaritableView->clearSelection();
        for (int i = 0; i < ui->StokKartlaritableView->model()->rowCount(); ++i) {
            if(!ui->StokKartlaritableView->model()->index(i, 3).data().toString().contains(QLocale(QLocale::Turkish, QLocale::Turkey).toUpper(arg1), Qt::CaseInsensitive)){
                ui->StokKartlaritableView->hideRow(i);
            }
            else{
                ui->StokKartlaritableView->showRow(i);
                gosterilenSatirlar.append(i);// aşağı/yukarı yön tuşları ile seçim yaparken gizlenen satırları es geçmek için gösterilen satırların indexini atıyorum.
            }
        }

        if(gosterilenSatirlar.count() > 0){
            ui->StokKartlaritableView->selectRow(gosterilenSatirlar.first());// gösterilen satırlardan ilk indexi seçiyorum.
        }

        if(arg1.count() == 0){// arama metni uzunluğu 0 ise satır seçimini ve son gösterilenindexi 0 yap
            ui->StokKartlaritableView->selectRow(0);
            sonSecilenGosterilenSatirIndexi = 0;
        }
    }
}


void StokFrom::on_ResimEkleBtn_clicked()
{
    ResimEkleDialog *resimEkleForm = new ResimEkleDialog(this);
    resimEkleForm->setUrunBarkod(ui->BarkodLnEdit->text());
    resimEkleForm->exec();
    if(QFile(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/urunler-image/" + ui->BarkodLnEdit->text() + ".png").exists()){
        ui->UrunResimlabel->setPixmap(QPixmap(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/urunler-image/" + ui->BarkodLnEdit->text() + ".png"));
    }
    else{
        ui->UrunResimlabel->setPixmap(QPixmap(":/images/ui/box.png"));
    }
}

void StokFrom::on_ResimSilBtn_clicked()
{
    if(!vt->getStokKarti(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString()).getResim().isNull()){
        uyariSes->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Ürün resmini silmek istediğinize emin misiniz?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        msg.setDefaultButton(QMessageBox::Yes);
        int cevap = msg.exec();
        if(cevap == QMessageBox::Yes){
            QFile resimDosya(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/urunler-image/" + ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString() + ".png");
            if(resimDosya.remove()){
                uyariSes->play();
                QMessageBox msg(this);
                msg.setWindowTitle("Uyarı");
                msg.setIcon(QMessageBox::Information);
                msg.setText("Ürün resmi silindi.");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
                ui->UrunResimlabel->setPixmap(vt->getStokKarti(ui->StokKartlaritableView->model()->index(ui->StokKartlaritableView->currentIndex().row(), 1).data().toString()).getResim());
            }
            else{
                uyariSes->play();
                QMessageBox msg(this);
                msg.setWindowTitle("Uyarı");
                msg.setIcon(QMessageBox::Warning);
                msg.setText("Ürün resmi silinemedi.");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
            }
        }
    }
}

void StokFrom::urunResmiKaydet(QPixmap urunResmi, QString urunBarkod)
{
    auto resimlerDizini = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/urunler-image/";
    if(!QFileInfo::exists(resimlerDizini)){
        QDir().mkdir(resimlerDizini);
    }
    // YENİ RESİM KAYDETME
    QFile yeniResim(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/urunler-image/" + urunBarkod + ".png");
    yeniResim.open(QIODevice::WriteOnly | QIODevice::Truncate);
    urunResmi.save(&yeniResim, "PNG");
    yeniResim.close();

    if(!QFileInfo(yeniResim).exists()){
        uyariSes->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Resim kaydedilemedi.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

void StokFrom::CTRL_F_Slot()
{
    if(!ui->AraLineEdit->isEnabled()){
        ui->AraLineEdit->selectAll();
        ui->AraLineEdit->setFocus();
    }
}

void StokFrom::key_Down_Slot()
{
    // gizlenen satırları seçim yapmasın diye satırı bu şekilde seçtiriyorum
    if(sonSecilenGosterilenSatirIndexi < (gosterilenSatirlar.count() - 1)){
        ui->StokKartlaritableView->selectRow(gosterilenSatirlar.at(sonSecilenGosterilenSatirIndexi + 1));
        sonSecilenGosterilenSatirIndexi++;
    }
    ui->AraLineEdit->setFocus();
}

void StokFrom::key_UP_Slot()
{
    if(sonSecilenGosterilenSatirIndexi > 0){
        ui->StokKartlaritableView->selectRow(gosterilenSatirlar.at(sonSecilenGosterilenSatirIndexi - 1));
        sonSecilenGosterilenSatirIndexi--;
    }
    ui->AraLineEdit->setFocus();
}

void StokFrom::key_F1_Slot()
{
    on_YeniBtn_clicked();;
}

void StokFrom::key_F2_Slot()
{
    on_DuzenleBtn_clicked();
}

void StokFrom::key_F3_Slot()
{
    on_SilBtn_clicked();
}

void StokFrom::key_F4_Slot()
{
    on_StokGirBtn_clicked();
}

void StokFrom::key_F5_Slot()
{
    on_StokCikBtn_clicked();
}

void StokFrom::key_F6_Slot()
{
    if(ui->StokKartlaritableView->selectionModel()->hasSelection()){
        hizliRafEtiketiYazdir();
    }
}

void StokFrom::on_barkodRadioButton_clicked()
{
    ui->AraLineEdit->setFocus();
    ui->AraLineEdit->selectAll();
}

void StokFrom::on_StokKartlaritableView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    if(ui->barkodRadioButton->isChecked()){
        ui->AraLineEdit->selectAll();
        ui->AraLineEdit->setFocus();
    }
    else if(ui->adRadioButton->isChecked()){
        ui->AraLineEdit->setFocus();
    }
}

void StokFrom::on_adRadioButton_clicked()
{
    ui->AraLineEdit->setFocus();
}

void StokFrom::on_kodRadioButton_clicked()
{
    ui->AraLineEdit->setFocus();
}

void StokFrom::on_BarkodLnEdit_textChanged(const QString &arg1)
{
    if(regEXPbarkod.exactMatch(arg1)){
        ui->BarkodLnEdit->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->BarkodLnEdit->setPalette(TextColorPaletteRed);
    }
}

void StokFrom::on_StokKoduLnEdit_textChanged(const QString &arg1)
{
    if(regEXPstokKod.exactMatch(arg1)){
        ui->StokKoduLnEdit->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->StokKoduLnEdit->setPalette(TextColorPaletteRed);
    }
}

void StokFrom::on_StokAdiLnEdit_textChanged(const QString &arg1)
{
    if(regEXPstokAd.exactMatch(arg1)){
        ui->StokAdiLnEdit->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->StokAdiLnEdit->setPalette(TextColorPaletteRed);
    }
}

void StokFrom::on_MiktarLnEdit_textChanged(const QString &arg1)
{
    QString str = arg1;
    int i = 0;
    QValidator::State st = regEXPstokMiktar->validate(str, i);

    if(st == QValidator::Acceptable){
        ui->MiktarLnEdit->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->MiktarLnEdit->setPalette(TextColorPaletteRed);
    }
}

void StokFrom::on_SFiyatdoubleSpinBox_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    if(ui->SFiyatdoubleSpinBox->value() > ui->AFiyatdoubleSpinBox->value()){
        ui->SFiyatdoubleSpinBox->setPalette(TextColorPaletteDefault);
        ui->AFiyatdoubleSpinBox->setPalette(TextColorPaletteDefault);
    }
    else{
        ui->SFiyatdoubleSpinBox->setPalette(TextColorPaletteRed);
    }
}

void StokFrom::on_AFiyatdoubleSpinBox_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    if(ui->AFiyatdoubleSpinBox->value() > ui->SFiyatdoubleSpinBox->value()){
        ui->AFiyatdoubleSpinBox->setPalette(TextColorPaletteRed);
    }
    else{
        ui->AFiyatdoubleSpinBox->setPalette(TextColorPaletteDefault);
    }
}

void StokFrom::on_TopluEtikettoolButton_clicked()
{
    //etiket basımını arkaplana atabilsin diye open() diyorum.
    RafEtiketiDialog *rafEtiketForm = new RafEtiketiDialog();
    rafEtiketForm->setWindowIcon(QIcon(":/images/ui/mhss.png"));
    rafEtiketForm->open();
//    delete rafEtiketForm;
}
