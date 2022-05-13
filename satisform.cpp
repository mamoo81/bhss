#include "satisform.h"
#include "ui_satisform.h"
#include "sepet.h"
#include "stokfrom.h"
#include "stokkarti.h"
#include "veritabani.h"
#include "kgform.h"
#include "hizliurunekleformdialog.h"
#include "fiyatgorform.h"
#include "loginform.h"
#include "adetdialogform.h"
#include "kasadialogform.h"
#include "yazici.h"
//*****************************
#include <QRegExp>
#include <QDebug>
#include <QSqlQuery>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QStandardPaths>
#include <QSound>

Sepet sepet[4];
QTableWidgetItem *yeniSatir;
int sepetMevcutUrunIndexi = 0;
QSound uyariSesi(":/sounds/sounds/warning-sound.wav");


SatisForm::SatisForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SatisForm)
{
    ui->setupUi(this);
    formLoad();

    setlocale(LC_ALL, "Turkish");
}

SatisForm::~SatisForm()
{
    delete ui;
}

void SatisForm::setUser(User user)
{
    kullanici = user;
    vt->setOturum(kullanici);
    this->setWindowTitle("MHSS - " + kullanici.getAd());
}

void SatisForm::formLoad()
{
    QWidget::showMaximized();
    initTableWidgets();
    hizliButonConnects();
    QRegExp rgx("(|\"|/|\\.|[0-9]){13}");// lineEdit'e sadece rakam girmesi için QRegExp tanımlaması.
    ui->barkodLineEdit->setValidator(new QRegExpValidator(rgx, this));// setValidator'üne QRegExpValidator'ü belirtme.
    ui->barkodLineEdit->installEventFilter(this);
    butonDurumlariniAyarla();
    getSonSatislar();
    hizliUrunButonlariAyarla();
    hizliUrunSayfaAyarla();
    ui->barkodLineEdit->setFocus();
    uyariSesi.setLoops(0);

    // klavye kısayol tanımlamaları
    CTRL_F = new QShortcut(this);
    CTRL_F->setKey(Qt::CTRL + Qt::Key_F);
    connect(CTRL_F, SIGNAL(activated()), this, SLOT(CTRL_F_Slot()));

    ENTER = new QShortcut(this);
    ENTER->setKey(Qt::Key_Enter);
    connect(ENTER, SIGNAL(activated()), this, SLOT(ENTER_Slot()));

    RETURN = new QShortcut(this);
    RETURN->setKey(Qt::Key_Return);
    connect(RETURN, SIGNAL(activated()), this, SLOT(ENTER_Slot()));

    ESC = new QShortcut(this);
    ESC->setKey(Qt::Key_Escape);
    connect(ESC, SIGNAL(activated()), this, SLOT(ESC_slot()));

    key_F = new QShortcut(this);
    key_F->setKey(Qt::Key_F);
    connect(key_F, SIGNAL(activated()), this, SLOT(F_Slot()));

    key_F3 = new QShortcut(this);
    key_F3->setKey(Qt::Key_F3);
    connect(key_F3, SIGNAL(activated()), this, SLOT(F3_Slot()));

    Key_F5 = new QShortcut(this);
    Key_F5->setKey(Qt::Key_F5);
    connect(Key_F5, SIGNAL(activated()), this, SLOT(F5_Slot()));

    key_F9 = new QShortcut(this);
    key_F9->setKey(Qt::Key_F9);
    connect(key_F9, SIGNAL(activated()), this, SLOT(F9_Slot()));

    key_F10 = new QShortcut(this);
    key_F10->setKey(Qt::Key_F10);
    connect(key_F10, SIGNAL(activated()), this, SLOT(F10_Slot()));

    key_F11 = new QShortcut(this);
    key_F11->setKey(Qt::Key_F11);
    connect(key_F11, SIGNAL(activated()), this, SLOT(F11_Slot()));

    key_F12 = new QShortcut(this);
    key_F12->setKey(Qt::Key_F12);
    connect(key_F12, SIGNAL(activated()), this, SLOT(F12_Slot()));

    key_Down = new QShortcut(this);
    key_Down->setKey(Qt::Key_Down);
    connect(key_Down, SIGNAL(activated()), this, SLOT(key_Down_Slot()));

    key_DownArrow = new QShortcut(this);
    key_DownArrow->setKey(Qt::DownArrow);
    connect(key_DownArrow, SIGNAL(activated()), this, SLOT(key_Down_Slot()));

}

void SatisForm::on_StokKartlariBtn_clicked()
{
    if(sepet[0].sepetBosmu() && sepet[1].sepetBosmu() && sepet[2].sepetBosmu() &&  sepet[3].sepetBosmu()){
        if(kullanici.getStokYetki()){
            StokFrom *stokKartiFormu = new StokFrom(this);
            stokKartiFormu->setUser(kullanici);
            stokKartiFormu->exec();
            delete stokKartiFormu;
            hizliUrunButonlariAyarla();
        }
        else{
            uyariSesi.play();
            QMessageBox msg(this);
            msg.setWindowTitle("Uyarı");
            msg.setIcon(QMessageBox::Warning);
            msg.setText("Stok işlemleri yapmaya yetkiniz yoktur.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setButtonText(QMessageBox::Ok, "Tamam");
            msg.exec();
        }
    }
    else{
        uyariSesi.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Satışı yapılmamış sepetiniz var stok kartları sayfasını açamazsınız.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    ui->barkodLineEdit->setFocus();
}

void SatisForm::initTableWidgets()
{
    ui->SepetlertabWidget->setCurrentIndex(0);
    ui->HizliUrunlertabWidget->setCurrentIndex(0);
    ui->sepet1TableWidget->setColumnWidth(0,135);
    ui->sepet1TableWidget->setColumnWidth(1,200);
    ui->sepet1TableWidget->setColumnWidth(2,80);
    ui->sepet1TableWidget->setColumnWidth(3,88);
    ui->sepet1TableWidget->setColumnWidth(4,60);
    ui->sepet1TableWidget->setColumnWidth(5,100);
    ui->sepet2TableWidget->setColumnWidth(0,135);
    ui->sepet2TableWidget->setColumnWidth(1,200);
    ui->sepet2TableWidget->setColumnWidth(2,80);
    ui->sepet2TableWidget->setColumnWidth(3,88);
    ui->sepet2TableWidget->setColumnWidth(4,60);
    ui->sepet2TableWidget->setColumnWidth(5,100);
    ui->sepet3TableWidget->setColumnWidth(0,135);
    ui->sepet3TableWidget->setColumnWidth(1,200);
    ui->sepet3TableWidget->setColumnWidth(2,80);
    ui->sepet3TableWidget->setColumnWidth(3,88);
    ui->sepet3TableWidget->setColumnWidth(4,60);
    ui->sepet3TableWidget->setColumnWidth(5,100);
    ui->sepet4TableWidget->setColumnWidth(0,135);
    ui->sepet4TableWidget->setColumnWidth(1,200);
    ui->sepet4TableWidget->setColumnWidth(2,80);
    ui->sepet4TableWidget->setColumnWidth(3,88);
    ui->sepet4TableWidget->setColumnWidth(4,60);
    ui->sepet4TableWidget->setColumnWidth(5,100);
}

void SatisForm::hizliButonConnects()
{
    for (int i = 1; i < 151; ++i) {
        QToolButton *btn = this->findChild<QToolButton*>("hizlitoolButton" + QString::number(i));
        connect(btn, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenuRequested(QPoint)));
    }
    ui->hizlitoolButton1->installEventFilter(this);
    ui->hizlitoolButton2->installEventFilter(this);
    ui->hizlitoolButton3->installEventFilter(this);
    ui->hizlitoolButton4->installEventFilter(this);
    ui->hizlitoolButton5->installEventFilter(this);
    ui->hizlitoolButton6->installEventFilter(this);
    ui->hizlitoolButton7->installEventFilter(this);
    ui->hizlitoolButton8->installEventFilter(this);
    ui->hizlitoolButton9->installEventFilter(this);
    ui->hizlitoolButton10->installEventFilter(this);
    ui->hizlitoolButton11->installEventFilter(this);
    ui->hizlitoolButton12->installEventFilter(this);
    ui->hizlitoolButton13->installEventFilter(this);
    ui->hizlitoolButton14->installEventFilter(this);
    ui->hizlitoolButton15->installEventFilter(this);
    ui->hizlitoolButton16->installEventFilter(this);
    ui->hizlitoolButton17->installEventFilter(this);
    ui->hizlitoolButton18->installEventFilter(this);
    ui->hizlitoolButton19->installEventFilter(this);
    ui->hizlitoolButton20->installEventFilter(this);
    ui->hizlitoolButton21->installEventFilter(this);
    ui->hizlitoolButton22->installEventFilter(this);
    ui->hizlitoolButton23->installEventFilter(this);
    ui->hizlitoolButton24->installEventFilter(this);
    ui->hizlitoolButton25->installEventFilter(this);
    ui->hizlitoolButton26->installEventFilter(this);
    ui->hizlitoolButton27->installEventFilter(this);
    ui->hizlitoolButton28->installEventFilter(this);
    ui->hizlitoolButton29->installEventFilter(this);
    ui->hizlitoolButton30->installEventFilter(this);
    ui->hizlitoolButton31->installEventFilter(this);
    ui->hizlitoolButton32->installEventFilter(this);
    ui->hizlitoolButton33->installEventFilter(this);
    ui->hizlitoolButton34->installEventFilter(this);
    ui->hizlitoolButton35->installEventFilter(this);
    ui->hizlitoolButton36->installEventFilter(this);
    ui->hizlitoolButton37->installEventFilter(this);
    ui->hizlitoolButton38->installEventFilter(this);
    ui->hizlitoolButton39->installEventFilter(this);
    ui->hizlitoolButton40->installEventFilter(this);
    ui->hizlitoolButton41->installEventFilter(this);
    ui->hizlitoolButton42->installEventFilter(this);
    ui->hizlitoolButton43->installEventFilter(this);
    ui->hizlitoolButton44->installEventFilter(this);
    ui->hizlitoolButton45->installEventFilter(this);
    ui->hizlitoolButton46->installEventFilter(this);
    ui->hizlitoolButton47->installEventFilter(this);
    ui->hizlitoolButton48->installEventFilter(this);
    ui->hizlitoolButton49->installEventFilter(this);
    ui->hizlitoolButton50->installEventFilter(this);
    ui->hizlitoolButton51->installEventFilter(this);
    ui->hizlitoolButton52->installEventFilter(this);
    ui->hizlitoolButton53->installEventFilter(this);
    ui->hizlitoolButton54->installEventFilter(this);
    ui->hizlitoolButton55->installEventFilter(this);
    ui->hizlitoolButton56->installEventFilter(this);
    ui->hizlitoolButton57->installEventFilter(this);
    ui->hizlitoolButton58->installEventFilter(this);
    ui->hizlitoolButton59->installEventFilter(this);
    ui->hizlitoolButton60->installEventFilter(this);
    ui->hizlitoolButton61->installEventFilter(this);
    ui->hizlitoolButton62->installEventFilter(this);
    ui->hizlitoolButton63->installEventFilter(this);
    ui->hizlitoolButton64->installEventFilter(this);
    ui->hizlitoolButton65->installEventFilter(this);
    ui->hizlitoolButton66->installEventFilter(this);
    ui->hizlitoolButton67->installEventFilter(this);
    ui->hizlitoolButton68->installEventFilter(this);
    ui->hizlitoolButton69->installEventFilter(this);
    ui->hizlitoolButton70->installEventFilter(this);
    ui->hizlitoolButton71->installEventFilter(this);
    ui->hizlitoolButton72->installEventFilter(this);
    ui->hizlitoolButton73->installEventFilter(this);
    ui->hizlitoolButton74->installEventFilter(this);
    ui->hizlitoolButton75->installEventFilter(this);
    ui->hizlitoolButton76->installEventFilter(this);
    ui->hizlitoolButton77->installEventFilter(this);
    ui->hizlitoolButton78->installEventFilter(this);
    ui->hizlitoolButton79->installEventFilter(this);
    ui->hizlitoolButton80->installEventFilter(this);
    ui->hizlitoolButton81->installEventFilter(this);
    ui->hizlitoolButton82->installEventFilter(this);
    ui->hizlitoolButton83->installEventFilter(this);
    ui->hizlitoolButton84->installEventFilter(this);
    ui->hizlitoolButton85->installEventFilter(this);
    ui->hizlitoolButton86->installEventFilter(this);
    ui->hizlitoolButton87->installEventFilter(this);
    ui->hizlitoolButton88->installEventFilter(this);
    ui->hizlitoolButton89->installEventFilter(this);
    ui->hizlitoolButton90->installEventFilter(this);
    ui->hizlitoolButton91->installEventFilter(this);
    ui->hizlitoolButton92->installEventFilter(this);
    ui->hizlitoolButton93->installEventFilter(this);
    ui->hizlitoolButton94->installEventFilter(this);
    ui->hizlitoolButton95->installEventFilter(this);
    ui->hizlitoolButton96->installEventFilter(this);
    ui->hizlitoolButton97->installEventFilter(this);
    ui->hizlitoolButton98->installEventFilter(this);
    ui->hizlitoolButton99->installEventFilter(this);
    ui->hizlitoolButton100->installEventFilter(this);
    ui->hizlitoolButton101->installEventFilter(this);
    ui->hizlitoolButton102->installEventFilter(this);
    ui->hizlitoolButton103->installEventFilter(this);
    ui->hizlitoolButton104->installEventFilter(this);
    ui->hizlitoolButton105->installEventFilter(this);
    ui->hizlitoolButton106->installEventFilter(this);
    ui->hizlitoolButton107->installEventFilter(this);
    ui->hizlitoolButton108->installEventFilter(this);
    ui->hizlitoolButton109->installEventFilter(this);
    ui->hizlitoolButton110->installEventFilter(this);
    ui->hizlitoolButton111->installEventFilter(this);
    ui->hizlitoolButton112->installEventFilter(this);
    ui->hizlitoolButton113->installEventFilter(this);
    ui->hizlitoolButton114->installEventFilter(this);
    ui->hizlitoolButton115->installEventFilter(this);
    ui->hizlitoolButton116->installEventFilter(this);
    ui->hizlitoolButton117->installEventFilter(this);
    ui->hizlitoolButton118->installEventFilter(this);
    ui->hizlitoolButton119->installEventFilter(this);
    ui->hizlitoolButton120->installEventFilter(this);
    ui->hizlitoolButton121->installEventFilter(this);
    ui->hizlitoolButton122->installEventFilter(this);
    ui->hizlitoolButton123->installEventFilter(this);
    ui->hizlitoolButton124->installEventFilter(this);
    ui->hizlitoolButton125->installEventFilter(this);
    ui->hizlitoolButton126->installEventFilter(this);
    ui->hizlitoolButton127->installEventFilter(this);
    ui->hizlitoolButton128->installEventFilter(this);
    ui->hizlitoolButton129->installEventFilter(this);
    ui->hizlitoolButton130->installEventFilter(this);
    ui->hizlitoolButton131->installEventFilter(this);
    ui->hizlitoolButton132->installEventFilter(this);
    ui->hizlitoolButton133->installEventFilter(this);
    ui->hizlitoolButton134->installEventFilter(this);
    ui->hizlitoolButton135->installEventFilter(this);
    ui->hizlitoolButton136->installEventFilter(this);
    ui->hizlitoolButton137->installEventFilter(this);
    ui->hizlitoolButton138->installEventFilter(this);
    ui->hizlitoolButton139->installEventFilter(this);
    ui->hizlitoolButton140->installEventFilter(this);
    ui->hizlitoolButton141->installEventFilter(this);
    ui->hizlitoolButton142->installEventFilter(this);
    ui->hizlitoolButton143->installEventFilter(this);
    ui->hizlitoolButton144->installEventFilter(this);
    ui->hizlitoolButton145->installEventFilter(this);
    ui->hizlitoolButton146->installEventFilter(this);
    ui->hizlitoolButton147->installEventFilter(this);
    ui->hizlitoolButton148->installEventFilter(this);
    ui->hizlitoolButton149->installEventFilter(this);
    ui->hizlitoolButton150->installEventFilter(this);
}

void SatisForm::sepeteEkle()
{
    if(vt->barkodVarmi(ui->barkodLineEdit->text())){
        stokKarti = vt->getStokKarti(ui->barkodLineEdit->text());
        if(stokKarti.getMiktar() > 0){
            if(stokKarti.getBirim() == 1){
                tableWidgetEkle(stokKarti, 1);
                sepet[ui->SepetlertabWidget->currentIndex()].urunEkle(stokKarti, 1);
            }
            else if(stokKarti.getBirim() == 2){
                KgForm *kgformu = new KgForm(this);
                kgformu->setModal(true);
                kgformu->exec();
                if(kgformu->getGirilenKg() != 0){
                    tableWidgetEkle(stokKarti, kgformu->getGirilenKg());
                    sepet[ui->SepetlertabWidget->currentIndex()].urunEkle(stokKarti, kgformu->getGirilenKg());
                }
                delete kgformu;
            }
            else if(stokKarti.getBirim() == 6){
                KgForm *kgformu = new KgForm(this);
                kgformu->setWindowTitle("Metre Girişi");
                kgformu->setBirimi("METRE");
                kgformu->setModal(true);
                kgformu->exec();
                if(kgformu->getGirilenKg() != 0){
                    tableWidgetEkle(stokKarti, kgformu->getGirilenKg());
                    sepet[ui->SepetlertabWidget->currentIndex()].urunEkle(stokKarti, kgformu->getGirilenKg());
                }
                delete kgformu;
            }
            ui->barkodLineEdit->clear();
            sepetToplaminiYaz();
            butonDurumlariniAyarla();
            sepetTabIconlariAyarla();
        }
        else{
            uyariSesi.play();
            QMessageBox MsgBox(QMessageBox::Warning, tr("Uyarı"), ui->barkodLineEdit->text() + tr("\n\nBarkodlu ürün stoğu tükenmiş!"), QMessageBox::Ok, this);
            MsgBox.setButtonText(QMessageBox::Ok, "Tamam");
            MsgBox.exec();
            ui->barkodLineEdit->clear();
        }
    }
    else{
        uyariSesi.play();
        QMessageBox MsgBox(QMessageBox::Warning, tr("Uyarı"), ui->barkodLineEdit->text() + tr("\n\nBarkodlu ürün bulunamadı!"), QMessageBox::Ok, this);
        MsgBox.setButtonText(QMessageBox::Ok, "Tamam");
        MsgBox.exec();
        ui->barkodLineEdit->clear();
    }
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_SepetlertabWidget_currentChanged(int index)
{
    Q_UNUSED(index);// warning uyarısını kapatmak için
    sepetToplaminiYaz();
    butonDurumlariniAyarla();
    ui->barkodLineEdit->setFocus();
}

void SatisForm::sepetToplaminiYaz()
{
    ui->ToplamTutarlcdNumber->display(QString::number(sepet[ui->SepetlertabWidget->currentIndex()].sepetToplamTutari(), 'f', 2));
}

void SatisForm::tableWidgetEkle(StokKarti p_StokKarti, float _miktar)
{
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(sepet[0].urunler.contains(p_StokKarti.getBarkod())){
            for (int i = 0; i < ui->sepet1TableWidget->model()->rowCount(); ++i) {
                if(ui->sepet1TableWidget->model()->index(i, 0).data().toString() == p_StokKarti.getBarkod()){
                    sepetMevcutUrunIndexi = i;
                    break;
                }
            }
            float yeniMiktar = _miktar + ui->sepet1TableWidget->model()->index(sepetMevcutUrunIndexi, 4).data().toFloat();
            ui->sepet1TableWidget->setItem(sepetMevcutUrunIndexi, 4, new QTableWidgetItem(QString::number(yeniMiktar)));
            ui->sepet1TableWidget->setItem(sepetMevcutUrunIndexi, 5, new QTableWidgetItem(QString::number(yeniMiktar * p_StokKarti.getSFiyat(), 'f', 2)));
            ui->sepet1TableWidget->selectRow(sepetMevcutUrunIndexi);
        }
        else{
            ui->sepet1TableWidget->insertRow(ui->sepet1TableWidget->rowCount());
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 0, new QTableWidgetItem(p_StokKarti.getBarkod()));
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 1, new QTableWidgetItem(p_StokKarti.getAd()));
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(p_StokKarti.getSFiyat(), 'f', 2)));
//            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem(p_StokKarti.getBirim()));
            switch (p_StokKarti.getBirim()) {
            case 1:
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem("ADET"));
                break;
            case 2:
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem("KİLOGRAM"));
                break;
            case 3:
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem("KOLİ"));
                break;
            case 4:
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem("PALET"));
                break;
            case 5:
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem("PAKET"));
                break;
            case 6:
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METRE"));
                break;
            case 7:
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METREKARE"));
                break;
            case 8:
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METREKÜP"));
                break;
            default:
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem("BİLİNMİYOR"));
                break;
            }
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(_miktar)));
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(p_StokKarti.getSFiyat() * _miktar, 'f', 2)));
            ui->sepet1TableWidget->selectRow(ui->sepet1TableWidget->rowCount() - 1);
        }
        break;
    case 1:
        if(sepet[1].urunler.contains(p_StokKarti.getBarkod())){
            for (int i = 0; i < ui->sepet2TableWidget->model()->rowCount(); ++i) {
                if(ui->sepet2TableWidget->model()->index(i, 0).data().toString() == p_StokKarti.getBarkod()){
                    sepetMevcutUrunIndexi = i;
                    break;
                }
            }
            float yeniMiktar = _miktar + ui->sepet2TableWidget->model()->index(sepetMevcutUrunIndexi, 4).data().toFloat();
            ui->sepet2TableWidget->setItem(sepetMevcutUrunIndexi, 4, new QTableWidgetItem(QString::number(yeniMiktar)));
            ui->sepet2TableWidget->setItem(sepetMevcutUrunIndexi, 5, new QTableWidgetItem(QString::number(yeniMiktar * p_StokKarti.getSFiyat(), 'f', 2)));
            ui->sepet2TableWidget->selectRow(sepetMevcutUrunIndexi);
        }
        else{
            ui->sepet2TableWidget->insertRow(ui->sepet2TableWidget->rowCount());
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 0, new QTableWidgetItem(p_StokKarti.getBarkod()));
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 1, new QTableWidgetItem(p_StokKarti.getAd()));
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(p_StokKarti.getSFiyat(), 'f', 2)));
//            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem(p_StokKarti.getBirim()));
            switch (p_StokKarti.getBirim()) {
            case 1:
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem("ADET"));
                break;
            case 2:
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem("KİLOGRAM"));
                break;
            case 3:
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem("KOLİ"));
                break;
            case 4:
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem("PALET"));
                break;
            case 5:
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem("PAKET"));
                break;
            case 6:
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METRE"));
                break;
            case 7:
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METREKARE"));
                break;
            case 8:
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METREKÜP"));
                break;
            default:
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem("BİLİNMİYOR"));
                break;
            }
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(_miktar)));
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(p_StokKarti.getSFiyat() * _miktar, 'f', 2)));
            ui->sepet2TableWidget->selectRow(ui->sepet2TableWidget->rowCount() - 1);
        }
        break;
    case 2:
        if(sepet[2].urunler.contains(p_StokKarti.getBarkod())){
            for (int i = 0; i < ui->sepet3TableWidget->model()->rowCount(); ++i) {
                if(ui->sepet3TableWidget->model()->index(i, 0).data().toString() == p_StokKarti.getBarkod()){
                    sepetMevcutUrunIndexi = i;
                    break;
                }
            }
            float yeniMiktar = _miktar + ui->sepet3TableWidget->model()->index(sepetMevcutUrunIndexi, 4).data().toFloat();
            ui->sepet3TableWidget->setItem(sepetMevcutUrunIndexi, 4, new QTableWidgetItem(QString::number(yeniMiktar)));
            ui->sepet3TableWidget->setItem(sepetMevcutUrunIndexi, 5, new QTableWidgetItem(QString::number(yeniMiktar * p_StokKarti.getSFiyat(), 'f', 2)));
            ui->sepet3TableWidget->selectRow(sepetMevcutUrunIndexi);
        }
        else{
            ui->sepet3TableWidget->insertRow(ui->sepet3TableWidget->rowCount());
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 0, new QTableWidgetItem(p_StokKarti.getBarkod()));
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 1, new QTableWidgetItem(p_StokKarti.getAd()));
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(p_StokKarti.getSFiyat(), 'f', 2)));
//            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem(p_StokKarti.getBirim()));
            switch (p_StokKarti.getBirim()) {
            case 1:
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem("ADET"));
                break;
            case 2:
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem("KİLOGRAM"));
                break;
            case 3:
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem("KOLİ"));
                break;
            case 4:
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem("PALET"));
                break;
            case 5:
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem("PAKET"));
                break;
            case 6:
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METRE"));
                break;
            case 7:
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METREKARE"));
                break;
            case 8:
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METREKÜP"));
                break;
            default:
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem("BİLİNMİYOR"));
                break;
            }
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(_miktar)));
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(p_StokKarti.getSFiyat() * _miktar, 'f', 2)));
            ui->sepet3TableWidget->selectRow(ui->sepet3TableWidget->rowCount() - 1);
        }
        break;
    case 3:
        if(sepet[3].urunler.contains(p_StokKarti.getBarkod())){
            for (int i = 0; i < ui->sepet4TableWidget->model()->rowCount(); ++i) {
                if(ui->sepet4TableWidget->model()->index(i, 0).data().toString() == p_StokKarti.getBarkod()){
                    sepetMevcutUrunIndexi = i;
                    break;
                }
            }
            float yeniMiktar = _miktar + ui->sepet4TableWidget->model()->index(sepetMevcutUrunIndexi, 4).data().toFloat();
            ui->sepet4TableWidget->setItem(sepetMevcutUrunIndexi, 4, new QTableWidgetItem(QString::number(yeniMiktar)));
            ui->sepet4TableWidget->setItem(sepetMevcutUrunIndexi, 5, new QTableWidgetItem(QString::number(yeniMiktar * p_StokKarti.getSFiyat(), 'f', 2)));
            ui->sepet4TableWidget->selectRow(sepetMevcutUrunIndexi);
        }
        else{
            ui->sepet4TableWidget->insertRow(ui->sepet4TableWidget->rowCount());
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 0, new QTableWidgetItem(p_StokKarti.getBarkod()));
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 1, new QTableWidgetItem(p_StokKarti.getAd()));
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(p_StokKarti.getSFiyat(), 'f', 2)));
//            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem(p_StokKarti.getBirim()));
            switch (p_StokKarti.getBirim()) {
            case 1:
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem("ADET"));
                break;
            case 2:
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem("KİLOGRAM"));
                break;
            case 3:
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem("KOLİ"));
                break;
            case 4:
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem("PALET"));
                break;
            case 5:
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem("PAKET"));
                break;
            case 6:
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METRE"));
                break;
            case 7:
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METREKARE"));
                break;
            case 8:
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem("METREKÜP"));
                break;
            default:
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem("BİLİNMİYOR"));
                break;
            }
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(_miktar)));
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(p_StokKarti.getSFiyat() * _miktar, 'f', 2)));
            ui->sepet4TableWidget->selectRow(ui->sepet4TableWidget->rowCount() - 1);
        }
        break;
    }
    butonDurumlariniAyarla();
}

void SatisForm::closeEvent(QCloseEvent *event)
{
    if(ui->sepet1TableWidget->rowCount() > 0 || ui->sepet2TableWidget->rowCount() > 0 || ui->sepet3TableWidget->rowCount() > 0 || ui->sepet4TableWidget->rowCount() > 0){
        uyariSesi.play();
        QMessageBox closingMsgBox(QMessageBox::Question, tr("Dikkat"), tr("Satışı yapılmamış sepetiniz var!\n\nYine de çıkmak istediğinize emin misiniz?"), QMessageBox::Yes | QMessageBox::No, this);
        closingMsgBox.setButtonText(QMessageBox::Yes, "Evet");
        closingMsgBox.setButtonText(QMessageBox::No, "Hayır");
        closingMsgBox.setDefaultButton(QMessageBox::No);
        int cevap = closingMsgBox.exec();
        switch (cevap) {
        case QMessageBox::Yes: {
                uyariSesi.play();
                QMessageBox msg(this);
                msg.setWindowTitle("Kasa uyarısı");
                msg.setIcon(QMessageBox::Question);
                msg.setText("Kasadan para çekimi yapılsın mı?");
                msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                msg.setDefaultButton(QMessageBox::No);
                msg.setButtonText(QMessageBox::Yes, "Evet");
                msg.setButtonText(QMessageBox::No, "Hayır");
                msg.setButtonText(QMessageBox::Cancel, "İptal");
                int cevapKasa = msg.exec();
                if(QMessageBox::Yes == cevapKasa){
                    SatisForm::on_KasaBtn_clicked();
                    Yazici *yazdir = new Yazici();
                    yazdir->cikisRaporuBas(kullanici);
                    vt->oturumSonlandir();
                    this->close();
                    LoginForm *lg = new LoginForm();
                    lg->show();
                }
                else if(QMessageBox::Cancel == cevapKasa){
                    event->ignore();
                    return;
                }
                else if(QMessageBox::No == cevapKasa){
                    Yazici *yazdir = new Yazici();
                    yazdir->cikisRaporuBas(kullanici);
                    vt->oturumSonlandir();
                    this->close();
                    LoginForm *lg = new LoginForm();
                    lg->show();
                }
            }
            break;
        case QMessageBox::No: {
                event->ignore();
                this->show();
            }
            break;
        }
    }
    else{
        uyariSesi.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Kasa uyarısı");
        msg.setIcon(QMessageBox::Question);
        msg.setText("Kasadan para çekimi yapılsın mı?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        msg.setButtonText(QMessageBox::Cancel, "İptal");
        int cevap = msg.exec();
        if(QMessageBox::Yes == cevap){
            SatisForm::on_KasaBtn_clicked();
            Yazici *yazdir = new Yazici();
            yazdir->cikisRaporuBas(kullanici);
            vt->oturumSonlandir();
            this->close();
            LoginForm *lg = new LoginForm();
            lg->show();
        }
        else if(QMessageBox::Cancel == cevap){
            event->ignore();
            return;
        }
        else if(QMessageBox::No == cevap){
            Yazici *yazdir = new Yazici();
            yazdir->cikisRaporuBas(kullanici);
            vt->oturumSonlandir();
            this->close();
            LoginForm *lg = new LoginForm();
            lg->show();
        }
    }
}

void SatisForm::adetCarp()
{
    QMessageBox msg;
    msg.setWindowTitle("Uyarı");
    msg.setIcon(QMessageBox::Warning);
    msg.setText("Adet stok miktarından fazla olamaz.");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, "Tamam");
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(ui->sepet1TableWidget->model()->index(ui->sepet1TableWidget->currentIndex().row(), 3).data().toString() == "ADET"
            || ui->sepet1TableWidget->model()->index(ui->sepet1TableWidget->currentIndex().row(), 3).data().toString() == "METRE"){
            //adet girme formu açılması
            adetForm->carpilacakAdet = sepet[ui->SepetlertabWidget->currentIndex()].urunler[ui->sepet1TableWidget->model()->index(ui->sepet1TableWidget->currentIndex().row(), 0).data().toString()].miktar;
            adetForm->exec();
            // çarpılacak adet stok miktarına eşitse veya düşükse
            if(sepet[0].urunler[ui->sepet1TableWidget->model()->index(ui->sepet1TableWidget->currentIndex().row(), 0).data().toString()].stokMiktari >= adetForm->carpilacakAdet){
                sepet[0].urunAdetCarp(ui->sepet1TableWidget->model()->index(ui->sepet1TableWidget->currentIndex().row(), 0).data().toString(), adetForm->carpilacakAdet);
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->currentIndex().row(), 4, new QTableWidgetItem(QString::number(adetForm->carpilacakAdet)));
                ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->currentIndex().row(), 5, new QTableWidgetItem(QString::number(sepet[0].urunler[ui->sepet1TableWidget->model()->index(ui->sepet1TableWidget->currentIndex().row(), 0).data().toString()].toplam, 'f', 2)));
            }
            else{
                uyariSesi.play();
                msg.exec();
            }
        }
        break;
    case 1:
        if(ui->sepet2TableWidget->model()->index(ui->sepet2TableWidget->currentIndex().row(), 3).data().toString() == "ADET"
            || ui->sepet1TableWidget->model()->index(ui->sepet1TableWidget->currentIndex().row(), 3).data().toString() == "METRE"){
            // adet girme formu açılması
            adetForm->carpilacakAdet = sepet[ui->SepetlertabWidget->currentIndex()].urunler[ui->sepet2TableWidget->model()->index(ui->sepet2TableWidget->currentIndex().row(), 0).data().toString()].miktar;
            adetForm->exec();
            if(sepet[1].urunler[ui->sepet2TableWidget->model()->index(ui->sepet2TableWidget->currentIndex().row(), 0).data().toString()].stokMiktari >= adetForm->carpilacakAdet){
                sepet[1].urunAdetCarp(ui->sepet2TableWidget->model()->index(ui->sepet2TableWidget->currentIndex().row(), 0).data().toString(), adetForm->carpilacakAdet);
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->currentIndex().row(), 4, new QTableWidgetItem(QString::number(adetForm->carpilacakAdet)));
                ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->currentIndex().row(), 5, new QTableWidgetItem(QString::number(sepet[1].urunler[ui->sepet2TableWidget->model()->index(ui->sepet2TableWidget->currentIndex().row(), 0).data().toString()].toplam, 'f', 2)));
            }
            else{
                uyariSesi.play();
                msg.exec();
            }
        }
        break;
    case 2:
        if(ui->sepet3TableWidget->model()->index(ui->sepet3TableWidget->currentIndex().row(), 3).data().toString() == "ADET"
            || ui->sepet1TableWidget->model()->index(ui->sepet1TableWidget->currentIndex().row(), 3).data().toString() == "METRE"){
            // adet girme formu açılması
            adetForm->carpilacakAdet = sepet[ui->SepetlertabWidget->currentIndex()].urunler[ui->sepet3TableWidget->model()->index(ui->sepet3TableWidget->currentIndex().row(), 0).data().toString()].miktar;
            adetForm->exec();
            if(sepet[2].urunler[ui->sepet3TableWidget->model()->index(ui->sepet3TableWidget->currentIndex().row(), 0).data().toString()].stokMiktari >= adetForm->carpilacakAdet){
                sepet[2].urunAdetCarp(ui->sepet3TableWidget->model()->index(ui->sepet3TableWidget->currentIndex().row(), 0).data().toString(), adetForm->carpilacakAdet);
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->currentIndex().row(), 4, new QTableWidgetItem(QString::number(adetForm->carpilacakAdet)));
                ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->currentIndex().row(), 5, new QTableWidgetItem(QString::number(sepet[2].urunler[ui->sepet3TableWidget->model()->index(ui->sepet3TableWidget->currentIndex().row(), 0).data().toString()].toplam, 'f', 2)));
            }
            else{
                uyariSesi.play();
                msg.exec();
            }
        }
        break;
    case 3:
        if(ui->sepet4TableWidget->model()->index(ui->sepet4TableWidget->currentIndex().row(), 3).data().toString() == "ADET"
            || ui->sepet1TableWidget->model()->index(ui->sepet1TableWidget->currentIndex().row(), 3).data().toString() == "METRE"){
            // adet girme formu açılması
            adetForm->carpilacakAdet = sepet[ui->SepetlertabWidget->currentIndex()].urunler[ui->sepet4TableWidget->model()->index(ui->sepet4TableWidget->currentIndex().row(), 0).data().toString()].miktar;
            adetForm->exec();
            if(sepet[3].urunler[ui->sepet4TableWidget->model()->index(ui->sepet4TableWidget->currentIndex().row(), 0).data().toString()].stokMiktari >= adetForm->carpilacakAdet){
                sepet[3].urunAdetCarp(ui->sepet4TableWidget->model()->index(ui->sepet4TableWidget->currentIndex().row(), 0).data().toString(), adetForm->carpilacakAdet);
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->currentIndex().row(), 4, new QTableWidgetItem(QString::number(adetForm->carpilacakAdet)));
                ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->currentIndex().row(), 5, new QTableWidgetItem(QString::number(sepet[3].urunler[ui->sepet4TableWidget->model()->index(ui->sepet4TableWidget->currentIndex().row(), 0).data().toString()].toplam, 'f', 2)));
            }
            else{
                uyariSesi.play();
                msg.exec();
            }
        }
        break;
    }
    sepetToplaminiYaz();
}

// focus QlineEdit deyken sağ-sol tuş eventlerini yakalamak için. QlineEdit sağ-sol tuşları imlecin yerini değiştirdiği için evente göndermiyor bu şekilde eventi yakalıyorum.
bool SatisForm::eventFilter(QObject *filtrelenecekObject, QEvent *event){
    if(ui->barkodLineEdit == filtrelenecekObject){      // filtrelenecek obje ui->barkodLineEdit ise
        if(event->type() == QEvent::KeyPress){      // event tipi QEvent::KeyPress ise
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);       // QKeyEvent eventi oluşturup,  key press event tipini QKeyEvent'e cast (kopyalıyorum) ediyorum.
            if(keyEvent->key() == Qt::LeftArrow || keyEvent->key() == Qt::Key_Left){
                if(ui->SepetlertabWidget->currentIndex() > 0){
                    int currentIndex = ui->SepetlertabWidget->currentIndex();
                    ui->SepetlertabWidget->setCurrentIndex(currentIndex - 1);
                }
                return true;
            }
            if(keyEvent->key() == Qt::RightArrow || keyEvent->key() == Qt::Key_Right){
                if(ui->SepetlertabWidget->currentIndex() < 3){
                    int currentIndex = ui->SepetlertabWidget->currentIndex();
                    ui->SepetlertabWidget->setCurrentIndex(currentIndex + 1);
                }
                return true;
            }
            if(keyEvent->key() == Qt::Key_F){
                FiyatGorForm *fiyatForm = new FiyatGorForm(this);
                fiyatForm->exec();
                delete fiyatForm;
                return true;
            }
            if(keyEvent->key() == Qt::Key_Plus){// sepette ki seçili ürün adetini artirma
                SatisForm::on_artirBtn_clicked();
            }
            if(keyEvent->key() == Qt::Key_Minus){// sepette ki seçili ürün adetini azaltma
                SatisForm::on_azaltBtn_clicked();
            }
            if(keyEvent->key() == Qt::Key_Asterisk){// klavyeden '*' tuşuna bastığında sepetteki seçili ürün adetini çarpma.
                SatisForm::adetCarp();
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton1 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton2 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton3 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton4 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton5 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton6 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton7 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton8 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton9 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton10 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton11 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton12 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton13 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton14 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton15 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton16 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton17 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton18 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton19 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton20 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton21 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton22 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton23 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton24 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton25 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton26 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton27 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton28 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton29 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton30 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton31 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton32 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton33 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton34 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton35 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton36 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton37 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton38 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton39 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton40 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton41 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton42 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton43 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton44 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton45 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton46 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton47 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton48 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton49 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton50 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton51 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton52 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton53 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton54 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton55 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton56 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton57 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton58 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton59 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton60 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton61 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton62 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton63 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton64 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton65 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton66 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton67 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton68 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton69 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton70 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton71 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton72 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton73 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton74 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton75 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton76 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton77 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton78 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton79 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton80 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton81 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton82 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton83 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton84 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton85 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton86 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton87 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton88 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton89 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton90 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton91 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton92 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton93 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton94 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton95 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton96 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton97 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton98 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton99 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton100 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton101 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton102 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton103 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton104 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton105 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton106 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton107 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton108 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton109 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton110 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton111 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton112 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton113 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton114 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton115 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton116 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton117 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton118 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton119 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton120 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton121 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton122 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton123 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton124 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton125 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton126 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton127 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton128 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton129 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton130 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton131 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton132 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton133 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton134 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton135 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton136 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton137 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton138 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton139 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton140 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton141 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton142 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton143 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton144 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton145 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton146 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton147 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton148 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton149 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    else if(ui->hizlitoolButton150 == filtrelenecekObject){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
            if(mEvent->button() == Qt::RightButton){
                hizliEklenecekButon = static_cast<QToolButton*>(filtrelenecekObject);
                return true;
            }
        }
        return false;
    }
    return SatisForm::eventFilter(filtrelenecekObject, event);
}

void SatisForm::getSonSatislar()
{
//    Veritabani *vt_satis = new Veritabani();
    ui->SonSatislarlistWidget->clear();
    ui->SonSatislarlistWidget->addItems(vt->getSonIslemler());
    ui->IslemSayisilabel->setText(QString::number(ui->SonSatislarlistWidget->count()));
}

void SatisForm::slotCustomContextMenuRequested(QPoint position)
{
    position = QCursor::pos();
    QMenu * hizliButonMenu = new QMenu(this);
    QAction *ekle = new QAction("ÜRÜN EKLE", this);
    ekle->setIcon(QIcon(":/images/ui/plus.png"));
    hizliButonMenu->addAction(ekle);
    connect(ekle, SIGNAL(triggered()), this, SLOT(ekleAction()));
    if(hizliEklenecekButon->whatsThis().isEmpty()){
        ekle->setEnabled(true);
    }
    else{
        ekle->setEnabled(false);
    }
    QAction *sil = new QAction("SİL", this);
    sil->setIcon(QIcon(":/images/ui/negative.png"));
    hizliButonMenu->addSeparator();
    hizliButonMenu->addAction(sil);
    connect(sil, SIGNAL(triggered()), this, SLOT(silAction()));
    if(!hizliEklenecekButon->whatsThis().isEmpty()){
        sil->setEnabled(true);
    }
    else{
        sil->setEnabled(false);
    }
    hizliButonMenu->popup(position);
    ui->barkodLineEdit->setFocus();
}

void SatisForm::hizliUrunButonlariAyarla()
{
    QSettings hizliButonBarkodlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/hizlibutonlar.ini", QSettings::IniFormat);
    if(!hizliButonBarkodlar.childGroups().empty()){
        foreach (QString butonName, hizliButonBarkodlar.childGroups()) {
            QToolButton *btn = this->findChild<QToolButton*>(butonName);
            hizliButonBarkodlar.beginGroup(butonName);
            btn->setText(hizliButonBarkodlar.value("ad").toString());
            btn->setWhatsThis(hizliButonBarkodlar.value("barkod").toString());
            if(QFileInfo(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/mhss/urunler-image/" + hizliButonBarkodlar.value("barkod").toString() + ".jpg").exists()){
                btn->setIcon(QIcon(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/mhss/urunler-image/" + hizliButonBarkodlar.value("barkod").toString() + ".jpg"));
            }
            else{
                btn->setIcon(QIcon(":/images/ui/box.png"));
            }
            hizliButonBarkodlar.endGroup();
        }
    }
    else{
        for (int i = 1; i < 151; ++i) {
            QToolButton *btn = this->findChild<QToolButton*>("hizlitoolButton" + QString::number(i));
            btn->setText("");
            btn->setWhatsThis("");
            btn->setIcon(QIcon(":/images/ui/box.png"));
        }
    }
}

void SatisForm::hizliUrunSayfaAyarla()
{
    QSettings hizliButonSayfaAdlari(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    hizliButonSayfaAdlari.beginGroup("hizlisayfa");
    ui->HizliUrunlertabWidget->setTabText(0, hizliButonSayfaAdlari.value("0").toString());
    ui->HizliUrunlertabWidget->setTabText(1, hizliButonSayfaAdlari.value("1").toString());
    ui->HizliUrunlertabWidget->setTabText(2, hizliButonSayfaAdlari.value("2").toString());
    ui->HizliUrunlertabWidget->setTabText(3, hizliButonSayfaAdlari.value("3").toString());
    ui->HizliUrunlertabWidget->setTabText(4, hizliButonSayfaAdlari.value("4").toString());
    hizliButonSayfaAdlari.endGroup();
}

void SatisForm::ekleAction()
{
    HizliUrunEkleFormDialog *hizliDialog = new HizliUrunEkleFormDialog(this);
    hizliDialog->exec();
    if(hizliDialog->ok){
        // hizlibutonlar.ini dosyasına kayıt etme başlangıcı
        QSettings hizliButonBarkodlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/hizlibutonlar.ini", QSettings::IniFormat);
        hizliButonBarkodlar.beginGroup(hizliEklenecekButon->objectName());
        hizliButonBarkodlar.setValue("stokid", hizliDialog->hizliUrunStokID);
        hizliButonBarkodlar.setValue("ad", hizliDialog->hizliUrunAd);
        hizliButonBarkodlar.setValue("barkod", hizliDialog->hizliUrunBarkod);
        // static_cast edilmiş butonun textini ve whatthis değiştirme başlangıcı.
        hizliEklenecekButon->setText(hizliDialog->hizliUrunAd);
        hizliEklenecekButon->setWhatsThis(hizliDialog->hizliUrunBarkod);
        if(QFileInfo(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/mhss/urunler-image/" + hizliButonBarkodlar.value("barkod").toString() + ".jpg").exists()){
            hizliEklenecekButon->setIcon(QIcon(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/mhss/urunler-image/" + hizliButonBarkodlar.value("barkod").toString() + ".jpg"));
        }
        else{
            hizliEklenecekButon->setIcon(QIcon(":/images/ui/box.png"));
        }
        hizliButonBarkodlar.endGroup();
        // hizlibutonlar.ini dosyasına kayıt etme bitiş.
    }
    ui->barkodLineEdit->setFocus();
}

void SatisForm::silAction()
{
    QSettings hizliButonBarkodlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/hizlibutonlar.ini", QSettings::IniFormat);
    hizliButonBarkodlar.remove(hizliEklenecekButon->objectName());
    hizliEklenecekButon->setText("");
    hizliEklenecekButon->setWhatsThis("");
    QIcon icon(":/images/ui/box.png");
    hizliEklenecekButon->setIcon(icon);
    ui->barkodLineEdit->setFocus();
}

int SatisForm::getSeciliSatirIndexi()
{
    int index = -1;
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(!(ui->sepet1TableWidget->currentIndex().row() < 0)){
            index = ui->sepet1TableWidget->currentIndex().row();
        }
        break;
    case 1:
        if(!(ui->sepet2TableWidget->currentIndex().row() < 0)){
            index = ui->sepet2TableWidget->currentIndex().row();
        }
        break;
    case 2:
        if(!(ui->sepet3TableWidget->currentIndex().row() < 0)){
            index = ui->sepet3TableWidget->currentIndex().row();
        }
        break;
    case 3:
        if(!(ui->sepet4TableWidget->currentIndex().row() < 0)){
            index = ui->sepet4TableWidget->currentIndex().row();
        }
        break;
    }
    return index;
}

void SatisForm::on_artirBtn_clicked()
{
    Urun guncelUrun;
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[0].urunArtir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setModal(true);
            kgformu->exec();
            sepet[0].urunArtir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        else if(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "METRE"){
            // metre giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setWindowTitle("Metre Girişi");
            kgformu->setBirimi("METRE");
            kgformu->setModal(true);
            kgformu->exec();
            sepet[0].urunArtir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        guncelUrun = sepet[0].urunBilgileriniGetir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet1TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet1TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam, 'f', 2)));
        break;
    case 1:
        if(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[1].urunArtir(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setModal(true);
            kgformu->exec();
            sepet[1].urunArtir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        else if(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "METRE"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setWindowTitle("Metre Girişi");
            kgformu->setBirimi("METRE");
            kgformu->setModal(true);
            kgformu->exec();
            sepet[1].urunArtir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        guncelUrun = sepet[1].urunBilgileriniGetir(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet2TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet2TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam, 'f', 2)));
        break;
    case 2:
        if(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[2].urunArtir(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setModal(true);
            kgformu->exec();
            sepet[2].urunArtir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        else if(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "METRE"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setWindowTitle("Metre Girişi");
            kgformu->setBirimi("METRE");
            kgformu->setModal(true);
            kgformu->exec();
            sepet[2].urunArtir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        guncelUrun = sepet[2].urunBilgileriniGetir(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet3TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet3TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam, 'f', 2)));
        break;
    case 3:
        if(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[3].urunArtir(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setModal(true);
            kgformu->exec();
            sepet[3].urunArtir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        else if(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "METRE"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setWindowTitle("Metre Girişi");
            kgformu->setBirimi("METRE");
            kgformu->setModal(true);
            kgformu->exec();
            sepet[3].urunArtir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        guncelUrun = sepet[3].urunBilgileriniGetir(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet4TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet4TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam, 'f', 2)));
        break;
    }
    sepetToplaminiYaz();
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_azaltBtn_clicked()
{
    Urun guncelUrun;
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[0].urunAzalt(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setModal(true);
            kgformu->exec();
            sepet[0].urunAzalt(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        else if(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "METRE"){
            // metre giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setWindowTitle("Metre Girişi");
            kgformu->setBirimi("METRE");
            kgformu->setModal(true);
            kgformu->exec();
            sepet[0].urunAzalt(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        guncelUrun = sepet[0].urunBilgileriniGetir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet1TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet1TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam, 'f', 2)));
        break;
    case 1:
        if(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[1].urunAzalt(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setModal(true);
            kgformu->exec();
            sepet[1].urunAzalt(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        else if(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "METRE"){
            // metre giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setWindowTitle("Metre Girişi");
            kgformu->setBirimi("METRE");
            kgformu->setModal(true);
            kgformu->exec();
            sepet[1].urunAzalt(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        guncelUrun = sepet[1].urunBilgileriniGetir(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet2TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet2TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam, 'f', 2)));
        break;
    case 2:
        if(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[2].urunAzalt(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setModal(true);
            kgformu->exec();
            sepet[2].urunAzalt(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        else if(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "METRE"){
            // metre giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setWindowTitle("Metre Girişi");
            kgformu->setBirimi("METRE");
            kgformu->setModal(true);
            kgformu->exec();
            sepet[2].urunAzalt(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        guncelUrun = sepet[2].urunBilgileriniGetir(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet3TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet3TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam, 'f', 2)));
        break;
    case 3:
        if(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[3].urunAzalt(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setModal(true);
            kgformu->exec();
            sepet[3].urunAzalt(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        else if(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "METRE"){
            // metre giriş formu gösterilecek alınan değer gönderilecek.
            KgForm *kgformu = new KgForm(this);
            kgformu->setWindowTitle("Metre Girişi");
            kgformu->setBirimi("METRE");
            kgformu->setModal(true);
            kgformu->exec();
            sepet[3].urunAzalt(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), kgformu->getGirilenKg());
            delete kgformu;
        }
        guncelUrun = sepet[3].urunBilgileriniGetir(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet4TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet4TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam, 'f', 2)));
        break;
    }
    sepetToplaminiYaz();
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_satirSilBtn_clicked()
{
    uyariSesi.play();
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Dikkat");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, "Evet");
    msgBox.setButtonText(QMessageBox::No, "Hayır");
    int cevap;
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(ui->sepet1TableWidget->rowCount() > 0){
            msgBox.setText(QString("%1 \n\nürününü sepetten silmek istediğinize emin misiniz?").arg(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 1).data().toString()));
            cevap = msgBox.exec();
            switch (cevap) {
            case QMessageBox::Yes:
                sepet[0].urunSil(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(),0).data().toString());
                ui->sepet1TableWidget->removeRow(getSeciliSatirIndexi());
                if(ui->sepet1TableWidget->rowCount() != 0){
                    ui->sepet1TableWidget->selectRow(ui->sepet1TableWidget->rowCount() - 1);
                }
                break;
            }
        }
        break;
    case 1:
        if(ui->sepet2TableWidget->rowCount() > 0){
            msgBox.setText(QString("%1 \n\nürününü sepetten silmek istediğinize emin misiniz?").arg(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 1).data().toString()));
            cevap = msgBox.exec();
            switch (cevap) {
            case QMessageBox::Yes:
                sepet[1].urunSil(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(),0).data().toString());
                ui->sepet2TableWidget->removeRow(getSeciliSatirIndexi());
                if(ui->sepet2TableWidget->rowCount() != 0){
                    ui->sepet2TableWidget->selectRow(ui->sepet2TableWidget->rowCount() - 1);
                }
                break;
            }
        }
        break;
    case 2:
        if(ui->sepet3TableWidget->rowCount() > 0){
            msgBox.setText(QString("%1 \n\nürününü sepetten silmek istediğinize emin misiniz?").arg(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 1).data().toString()));
            cevap = msgBox.exec();
            switch (cevap) {
            case QMessageBox::Yes:
                sepet[2].urunSil(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(),0).data().toString());
                ui->sepet3TableWidget->removeRow(getSeciliSatirIndexi());
                if(ui->sepet3TableWidget->rowCount() != 0){
                    ui->sepet3TableWidget->selectRow(ui->sepet3TableWidget->rowCount() - 1);
                }
                break;
            }
        }
        break;
    case 3:
        if(ui->sepet4TableWidget->rowCount() > 0){
            msgBox.setText(QString("%1 \n\nürününü sepetten silmek istediğinize emin misiniz?").arg(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 1).data().toString()));
            cevap = msgBox.exec();
            switch (cevap) {
            case QMessageBox::Yes:
                sepet[3].urunSil(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(),0).data().toString());
                ui->sepet4TableWidget->removeRow(getSeciliSatirIndexi());
                if(ui->sepet4TableWidget->rowCount() != 0){
                    ui->sepet4TableWidget->selectRow(ui->sepet4TableWidget->rowCount() - 1);
                }
                break;
            }
        }
        break;
    }
    sepetToplaminiYaz();
    butonDurumlariniAyarla();
    sepetTabIconlariAyarla();
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_sepetSilBtn_clicked()
{
    uyariSesi.play();
    QMessageBox sepetSilMsgBox(this);
    sepetSilMsgBox.setWindowTitle("Dikkat");
    sepetSilMsgBox.setIcon(QMessageBox::Question);
    sepetSilMsgBox.setText("Aktif Sepeti silmek istediğinize emin misiniz?");
    sepetSilMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    sepetSilMsgBox.setDefaultButton(QMessageBox::Yes);
    sepetSilMsgBox.setButtonText(QMessageBox::Yes, "Evet");
    sepetSilMsgBox.setButtonText(QMessageBox::No, "Hayır");
    sepetSilMsgBox.setModal(true);
    int cevap = sepetSilMsgBox.exec();
    if(cevap == QMessageBox::Yes){
        switch (ui->SepetlertabWidget->currentIndex()) {
        case 0:
            sepet[0].sepetiSil();
            ui->sepet1TableWidget->model()->removeRows(0, ui->sepet1TableWidget->rowCount());
            break;
        case 1:
            sepet[1].sepetiSil();
            ui->sepet2TableWidget->model()->removeRows(0, ui->sepet2TableWidget->rowCount());
            break;
        case 2:
            sepet[2].sepetiSil();
            ui->sepet3TableWidget->model()->removeRows(0, ui->sepet3TableWidget->rowCount());
            break;
        case 3:
            sepet[3].sepetiSil();
            ui->sepet4TableWidget->model()->removeRows(0, ui->sepet4TableWidget->rowCount());
            break;
        }
        ui->ToplamTutarlcdNumber->display(0);
    }
    sepetToplaminiYaz();
    butonDurumlariniAyarla();
    sepetTabIconlariAyarla();
    ui->barkodLineEdit->setFocus();
}

void SatisForm::butonDurumlariniAyarla()
{
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(ui->sepet1TableWidget->rowCount() > 0){
            ui->artirBtn->setEnabled(true);
            ui->azaltBtn->setEnabled(true);
            ui->satirSilBtn->setEnabled(true);
            ui->sepetSilBtn->setEnabled(true);
            ui->iadeAlBtn->setEnabled(true);
            ui->satisYapBtn->setEnabled(true);
            ui->CarpBtn->setEnabled(true);
        }
        else{
            ui->artirBtn->setEnabled(false);
            ui->azaltBtn->setEnabled(false);
            ui->satirSilBtn->setEnabled(false);
            ui->sepetSilBtn->setEnabled(false);
            ui->iadeAlBtn->setEnabled(false);
            ui->satisYapBtn->setEnabled(false);
            ui->CarpBtn->setEnabled(false);
        }
        break;
    case 1:
        if(ui->sepet2TableWidget->rowCount() > 0){
            ui->artirBtn->setEnabled(true);
            ui->azaltBtn->setEnabled(true);
            ui->satirSilBtn->setEnabled(true);
            ui->sepetSilBtn->setEnabled(true);
            ui->iadeAlBtn->setEnabled(true);
            ui->satisYapBtn->setEnabled(true);
            ui->CarpBtn->setEnabled(true);
        }
        else{
            ui->artirBtn->setEnabled(false);
            ui->azaltBtn->setEnabled(false);
            ui->satirSilBtn->setEnabled(false);
            ui->sepetSilBtn->setEnabled(false);
            ui->iadeAlBtn->setEnabled(false);
            ui->satisYapBtn->setEnabled(false);
            ui->CarpBtn->setEnabled(false);
        }
        break;
    case 2:
        if(ui->sepet3TableWidget->rowCount() > 0){
            ui->artirBtn->setEnabled(true);
            ui->azaltBtn->setEnabled(true);
            ui->satirSilBtn->setEnabled(true);
            ui->sepetSilBtn->setEnabled(true);
            ui->iadeAlBtn->setEnabled(true);
            ui->satisYapBtn->setEnabled(true);
            ui->CarpBtn->setEnabled(true);
        }
        else{
            ui->artirBtn->setEnabled(false);
            ui->azaltBtn->setEnabled(false);
            ui->satirSilBtn->setEnabled(false);
            ui->sepetSilBtn->setEnabled(false);
            ui->iadeAlBtn->setEnabled(false);
            ui->satisYapBtn->setEnabled(false);
            ui->CarpBtn->setEnabled(false);
        }
        break;
    case 3:
        if(ui->sepet4TableWidget->rowCount() > 0){
            ui->artirBtn->setEnabled(true);
            ui->azaltBtn->setEnabled(true);
            ui->satirSilBtn->setEnabled(true);
            ui->sepetSilBtn->setEnabled(true);
            ui->iadeAlBtn->setEnabled(true);
            ui->satisYapBtn->setEnabled(true);
            ui->CarpBtn->setEnabled(true);
        }
        else{
            ui->artirBtn->setEnabled(false);
            ui->azaltBtn->setEnabled(false);
            ui->satirSilBtn->setEnabled(false);
            ui->sepetSilBtn->setEnabled(false);
            ui->iadeAlBtn->setEnabled(false);
            ui->satisYapBtn->setEnabled(false);
            ui->CarpBtn->setEnabled(false);
        }
        break;
    }
}


void SatisForm::on_satisYapBtn_clicked()
{
    SatisYapForm *satisyapfrm = new SatisYapForm(this);
    satisyapfrm->setSatilacakSepet(sepet[ui->SepetlertabWidget->currentIndex()]);
    satisyapfrm->setKullanici(kullanici);
    satisyapfrm->exec();
    //satış sonrası sepet ve tablo silme, son işlemlere ekleme başlangıcı
    if(satisyapfrm->satisYapildimi){
        switch (ui->SepetlertabWidget->currentIndex()) {
        case 0:
            sepet[0].sepetiSil();
            ui->sepet1TableWidget->model()->removeRows(0, ui->sepet1TableWidget->rowCount());
            break;
        case 1:
            sepet[1].sepetiSil();
            ui->sepet2TableWidget->model()->removeRows(0, ui->sepet2TableWidget->rowCount());
            break;
        case 2:
            sepet[2].sepetiSil();
            ui->sepet3TableWidget->model()->removeRows(0, ui->sepet3TableWidget->rowCount());
            break;
        case 3:
            sepet[3].sepetiSil();
            ui->sepet4TableWidget->model()->removeRows(0, ui->sepet4TableWidget->rowCount());
            break;
        }
        butonDurumlariniAyarla();
        sepetToplaminiYaz();
        getSonSatislar();
        sepetTabIconlariAyarla();
        ui->barkodLineEdit->setFocus();
    }
    delete satisyapfrm;
}

void SatisForm::CTRL_F_Slot()
{
    emit on_KartAratoolButton_clicked();
}

void SatisForm::ENTER_Slot()
{
    if(ui->barkodLineEdit->text().isEmpty()){// barkod lineedit boş ise ödeme ekranını göster.
        //ödeme ekranı işlemleri.
        if(!sepet[ui->SepetlertabWidget->currentIndex()].sepetBosmu()){
            SatisForm::on_satisYapBtn_clicked();
        }
    }
    else{
        //sepete ekleme işleri başlangıcı.
        sepeteEkle();
        ui->barkodLineEdit->clear();
        ui->barkodLineEdit->setFocus();
    }
}

void SatisForm::ESC_slot()
{
    this->close();
}

void SatisForm::F_Slot()
{
    FiyatGorForm *fiyatForm = new FiyatGorForm(this);
    fiyatForm->exec();
    ui->barkodLineEdit->setFocus();;
    delete fiyatForm;
}

void SatisForm::F3_Slot()
{
    if(ui->satirSilBtn->isEnabled()){
        SatisForm::on_satirSilBtn_clicked();
    }
}

void SatisForm::F5_Slot()
{
    if(ui->sepetSilBtn->isEnabled()){
        SatisForm::on_sepetSilBtn_clicked();
    }
}

void SatisForm::F9_Slot()
{
    SatisForm::on_StokKartlariBtn_clicked();
}

void SatisForm::F10_Slot()
{
    SatisForm::on_CariKartlarBtn_clicked();
}

void SatisForm::F11_Slot()
{
    // kasa formu açılacak
    SatisForm::on_KasaBtn_clicked();
}

void SatisForm::F12_Slot()
{
    SatisForm::on_AyarlarBtn_clicked();
}

void SatisForm::key_Down_Slot()
{
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        ui->sepet1TableWidget->selectRow(ui->sepet1TableWidget->currentIndex().row() + 1);
        break;
    case 1:
        ui->sepet2TableWidget->selectRow(ui->sepet2TableWidget->currentIndex().row() + 1);
        break;
    case 2:
        ui->sepet3TableWidget->selectRow(ui->sepet3TableWidget->currentIndex().row() + 1);
        break;
    case 3:
        ui->sepet4TableWidget->selectRow(ui->sepet4TableWidget->currentIndex().row() + 1);
        break;
    }
}

void SatisForm::key_UP_Slot()
{
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        ui->sepet1TableWidget->selectRow(ui->sepet1TableWidget->currentIndex().row() - 1);
        break;
    case 1:
        ui->sepet2TableWidget->selectRow(ui->sepet2TableWidget->currentIndex().row() - 1);
        break;
    case 2:
        ui->sepet3TableWidget->selectRow(ui->sepet3TableWidget->currentIndex().row() - 1);
        break;
    case 3:
        ui->sepet4TableWidget->selectRow(ui->sepet4TableWidget->currentIndex().row() - 1);
        break;
    }
}

#pragma region hizliurunbutonları click metodları {


void SatisForm::on_hizlitoolButton1_clicked()
{
    if(!ui->hizlitoolButton1->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton1->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton2_clicked()
{
    if(!ui->hizlitoolButton2->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton2->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton3_clicked()
{
    if(!ui->hizlitoolButton3->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton3->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton4_clicked()
{
    if(!ui->hizlitoolButton4->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton4->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton5_clicked()
{
    if(!ui->hizlitoolButton5->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton5->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton6_clicked()
{
    if(!ui->hizlitoolButton6->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton6->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton7_clicked()
{
    if(!ui->hizlitoolButton7->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton7->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton8_clicked()
{
    if(!ui->hizlitoolButton8->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton8->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton9_clicked()
{
    if(!ui->hizlitoolButton9->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton9->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton10_clicked()
{
    if(!ui->hizlitoolButton10->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton10->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton11_clicked()
{
    if(!ui->hizlitoolButton11->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton11->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton12_clicked()
{
    if(!ui->hizlitoolButton12->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton12->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton13_clicked()
{
    if(!ui->hizlitoolButton13->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton13->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton14_clicked()
{
    if(!ui->hizlitoolButton14->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton14->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton15_clicked()
{
    if(!ui->hizlitoolButton15->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton15->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton16_clicked()
{
    if(!ui->hizlitoolButton16->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton16->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton17_clicked()
{
    if(!ui->hizlitoolButton17->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton17->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton18_clicked()
{
    if(!ui->hizlitoolButton18->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton18->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton19_clicked()
{
    if(!ui->hizlitoolButton19->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton19->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton20_clicked()
{
    if(!ui->hizlitoolButton20->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton20->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton21_clicked()
{
    if(!ui->hizlitoolButton21->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton21->whatsThis());
        sepeteEkle();
    }
}
void SatisForm::on_hizlitoolButton22_clicked()
{
    if(!ui->hizlitoolButton22->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton22->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton23_clicked()
{
    if(!ui->hizlitoolButton23->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton23->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton24_clicked()
{
    if(!ui->hizlitoolButton24->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton24->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton25_clicked()
{
    if(!ui->hizlitoolButton25->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton25->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton26_clicked()
{
    if(!ui->hizlitoolButton26->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton26->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton27_clicked()
{
    if(!ui->hizlitoolButton27->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton27->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton28_clicked()
{
    if(!ui->hizlitoolButton28->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton28->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton29_clicked()
{
    if(!ui->hizlitoolButton29->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton29->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton30_clicked()
{
    if(!ui->hizlitoolButton30->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton30->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton31_clicked()
{
    if(!ui->hizlitoolButton31->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton31->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton32_clicked()
{
    if(!ui->hizlitoolButton32->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton32->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton33_clicked()
{
    if(!ui->hizlitoolButton33->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton33->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton34_clicked()
{
    if(!ui->hizlitoolButton34->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton34->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton35_clicked()
{
    if(!ui->hizlitoolButton35->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton35->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton36_clicked()
{
    if(!ui->hizlitoolButton36->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton36->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton37_clicked()
{
    if(!ui->hizlitoolButton37->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton37->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton38_clicked()
{
    if(!ui->hizlitoolButton38->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton38->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton39_clicked()
{
    if(!ui->hizlitoolButton39->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton39->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton40_clicked()
{
    if(!ui->hizlitoolButton40->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton40->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton41_clicked()
{
    if(!ui->hizlitoolButton41->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton41->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton42_clicked()
{
    if(!ui->hizlitoolButton42->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton42->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton43_clicked()
{
    if(!ui->hizlitoolButton43->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton43->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton44_clicked()
{
    if(!ui->hizlitoolButton44->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton44->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton45_clicked()
{
    if(!ui->hizlitoolButton45->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton45->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton46_clicked()
{
    if(!ui->hizlitoolButton46->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton46->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton47_clicked()
{
    if(!ui->hizlitoolButton47->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton47->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton48_clicked()
{
    if(!ui->hizlitoolButton48->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton48->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton49_clicked()
{
    if(!ui->hizlitoolButton49->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton49->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton50_clicked()
{
    if(!ui->hizlitoolButton50->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton50->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton51_clicked()
{
    if(!ui->hizlitoolButton51->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton51->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton52_clicked()
{
    if(!ui->hizlitoolButton52->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton52->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton53_clicked()
{
    if(!ui->hizlitoolButton53->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton53->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton54_clicked()
{
    if(!ui->hizlitoolButton54->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton54->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton55_clicked()
{
    if(!ui->hizlitoolButton55->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton55->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton56_clicked()
{
    if(!ui->hizlitoolButton56->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton56->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton57_clicked()
{
    if(!ui->hizlitoolButton57->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton57->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton58_clicked()
{
    if(!ui->hizlitoolButton58->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton58->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton59_clicked()
{
    if(!ui->hizlitoolButton59->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton59->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton60_clicked()
{
    if(!ui->hizlitoolButton60->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton60->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton61_clicked()
{
    if(!ui->hizlitoolButton61->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton61->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton62_clicked()
{
    if(!ui->hizlitoolButton62->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton62->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton63_clicked()
{
    if(!ui->hizlitoolButton63->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton63->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton64_clicked()
{
    if(!ui->hizlitoolButton64->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton64->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton65_clicked()
{
    if(!ui->hizlitoolButton65->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton65->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton66_clicked()
{
    if(!ui->hizlitoolButton66->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton66->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton67_clicked()
{
    if(!ui->hizlitoolButton67->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton67->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton68_clicked()
{
    if(!ui->hizlitoolButton68->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton68->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton69_clicked()
{
    if(!ui->hizlitoolButton69->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton69->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton70_clicked()
{
    if(!ui->hizlitoolButton70->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton70->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton71_clicked()
{
    if(!ui->hizlitoolButton71->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton71->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton72_clicked()
{
    if(!ui->hizlitoolButton72->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton72->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton73_clicked()
{
    if(!ui->hizlitoolButton73->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton73->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton74_clicked()
{
    if(!ui->hizlitoolButton74->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton74->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton75_clicked()
{
    if(!ui->hizlitoolButton75->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton75->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton76_clicked()
{
    if(!ui->hizlitoolButton76->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton76->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton77_clicked()
{
    if(!ui->hizlitoolButton77->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton77->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton78_clicked()
{
    if(!ui->hizlitoolButton78->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton78->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton79_clicked()
{
    if(!ui->hizlitoolButton79->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton79->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton80_clicked()
{
    if(!ui->hizlitoolButton80->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton80->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton81_clicked()
{
    if(!ui->hizlitoolButton81->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton81->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton82_clicked()
{
    if(!ui->hizlitoolButton82->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton82->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton83_clicked()
{
    if(!ui->hizlitoolButton83->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton83->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton84_clicked()
{
    if(!ui->hizlitoolButton84->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton84->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton85_clicked()
{
    if(!ui->hizlitoolButton85->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton85->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton86_clicked()
{
    if(!ui->hizlitoolButton86->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton86->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton87_clicked()
{
    if(!ui->hizlitoolButton87->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton87->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton88_clicked()
{
    if(!ui->hizlitoolButton88->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton88->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton89_clicked()
{
    if(!ui->hizlitoolButton89->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton89->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton90_clicked()
{
    if(!ui->hizlitoolButton90->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton90->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton91_clicked()
{
    if(!ui->hizlitoolButton91->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton91->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton92_clicked()
{
    if(!ui->hizlitoolButton92->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton92->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton93_clicked()
{
    if(!ui->hizlitoolButton93->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton93->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton94_clicked()
{
    if(!ui->hizlitoolButton94->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton94->whatsThis());
        sepeteEkle();
    }
}
void SatisForm::on_hizlitoolButton95_clicked()
{
    if(!ui->hizlitoolButton95->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton95->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton96_clicked()
{
    if(!ui->hizlitoolButton96->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton96->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton97_clicked()
{
    if(!ui->hizlitoolButton97->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton97->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton98_clicked()
{
    if(!ui->hizlitoolButton98->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton98->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton99_clicked()
{
    if(!ui->hizlitoolButton99->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton99->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton100_clicked()
{
    if(!ui->hizlitoolButton100->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton100->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton101_clicked()
{
    if(!ui->hizlitoolButton101->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton101->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton102_clicked()
{
    if(!ui->hizlitoolButton102->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton102->whatsThis());
        sepeteEkle();
    }
}
void SatisForm::on_hizlitoolButton103_clicked()
{
    if(!ui->hizlitoolButton103->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton103->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton104_clicked()
{
    if(!ui->hizlitoolButton104->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton104->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton105_clicked()
{
    if(!ui->hizlitoolButton105->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton105->whatsThis());
        sepeteEkle();
    }
}
void SatisForm::on_hizlitoolButton106_clicked()
{
    if(!ui->hizlitoolButton106->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton106->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton107_clicked()
{
    if(!ui->hizlitoolButton107->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton107->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton108_clicked()
{
    if(!ui->hizlitoolButton108->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton108->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton109_clicked()
{
    if(!ui->hizlitoolButton109->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton109->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton110_clicked()
{
    if(!ui->hizlitoolButton110->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton110->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton111_clicked()
{
    if(!ui->hizlitoolButton111->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton111->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton112_clicked()
{
    if(!ui->hizlitoolButton112->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton112->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton113_clicked()
{
    if(!ui->hizlitoolButton113->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton113->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton114_clicked()
{
    if(!ui->hizlitoolButton114->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton114->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton115_clicked()
{
    if(!ui->hizlitoolButton115->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton115->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton116_clicked()
{
    if(!ui->hizlitoolButton116->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton116->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton117_clicked()
{
    if(!ui->hizlitoolButton117->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton117->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton118_clicked()
{
    if(!ui->hizlitoolButton118->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton118->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton119_clicked()
{
    if(!ui->hizlitoolButton119->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton119->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton120_clicked()
{
    if(!ui->hizlitoolButton120->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton120->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton121_clicked()
{
    if(!ui->hizlitoolButton121->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton121->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton122_clicked()
{
    if(!ui->hizlitoolButton122->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton122->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton123_clicked()
{
    if(!ui->hizlitoolButton123->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton123->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton124_clicked()
{
    if(!ui->hizlitoolButton124->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton124->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton125_clicked()
{
    if(!ui->hizlitoolButton125->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton125->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton126_clicked()
{
    if(!ui->hizlitoolButton126->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton126->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton127_clicked()
{
    if(!ui->hizlitoolButton127->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton127->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton128_clicked()
{
    if(!ui->hizlitoolButton128->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton128->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton129_clicked()
{
    if(!ui->hizlitoolButton129->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton129->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton130_clicked()
{
    if(!ui->hizlitoolButton130->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton130->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton131_clicked()
{
    if(!ui->hizlitoolButton131->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton131->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton132_clicked()
{
    if(!ui->hizlitoolButton132->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton132->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton133_clicked()
{
    if(!ui->hizlitoolButton133->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton133->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton134_clicked()
{
    if(!ui->hizlitoolButton134->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton134->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton135_clicked()
{
    if(!ui->hizlitoolButton135->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton135->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton136_clicked()
{
    if(!ui->hizlitoolButton136->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton136->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton137_clicked()
{
    if(!ui->hizlitoolButton137->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton137->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton138_clicked()
{
    if(!ui->hizlitoolButton138->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton138->whatsThis());
        sepeteEkle();
    }
}
void SatisForm::on_hizlitoolButton139_clicked()
{
    if(!ui->hizlitoolButton139->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton139->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton140_clicked()
{
    if(!ui->hizlitoolButton140->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton140->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton141_clicked()
{
    if(!ui->hizlitoolButton141->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton141->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton142_clicked()
{
    if(!ui->hizlitoolButton142->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton142->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton143_clicked()
{
    if(!ui->hizlitoolButton143->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton143->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton144_clicked()
{
    if(!ui->hizlitoolButton144->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton144->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton145_clicked()
{
    if(!ui->hizlitoolButton145->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton145->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton146_clicked()
{
    if(!ui->hizlitoolButton146->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton146->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton147_clicked()
{
    if(!ui->hizlitoolButton147->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton147->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton148_clicked()
{
    if(!ui->hizlitoolButton148->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton148->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton149_clicked()
{
    if(!ui->hizlitoolButton149->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton149->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
void SatisForm::on_hizlitoolButton150_clicked()
{
    if(!ui->hizlitoolButton150->whatsThis().isEmpty()){
        ui->barkodLineEdit->setText(ui->hizlitoolButton150->whatsThis());
        sepeteEkle();
    }
    else{
        ui->barkodLineEdit->setFocus();
    }
}
#pragma endregion}

void SatisForm::on_HizliUrunlertabWidget_currentChanged(int index)
{
    Q_UNUSED(index);// warning uyarısını kapatmak için.
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_SonSatislarlistWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString gosterilecekSatis = item->text();
    satisGosterForm->setSatisFaturaNo(gosterilecekSatis);
    satisGosterForm->sepetiCek();
    satisGosterForm->exec();
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_CikisToolBtn_clicked()
{   
    this->close();
}


void SatisForm::on_sepet1TableWidget_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);//warning uyarısını kapatmak için
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_KasaBtn_clicked()
{
    KasaDialogForm *kasaForm = new KasaDialogForm(this);
    kasaForm->setKullanici(kullanici);
    kasaForm->exec();
    ui->barkodLineEdit->setFocus();
}

void SatisForm::sepetTabIconlariAyarla()
{
    if(!sepet[ui->SepetlertabWidget->currentIndex()].sepetBosmu()){
        ui->SepetlertabWidget->setTabIcon(ui->SepetlertabWidget->currentIndex(), QIcon(":/images/ui/shopping-cart-buying.png"));
    }
    else{
        ui->SepetlertabWidget->setTabIcon(ui->SepetlertabWidget->currentIndex(), QIcon(":/images/ui/shopping-cart.png"));
    }
}

void SatisForm::on_iadeAlBtn_clicked()
{
    if(kullanici.getIadeYetki()){
        if(!sepet[ui->SepetlertabWidget->currentIndex()].sepetBosmu()){
            uyariSesi.play();
            QMessageBox msg(this);
            msg.setWindowTitle("Dikkat");
            msg.setIcon(QMessageBox::Question);
            msg.setText(QString("Sepetteki %1 kalem ürün DİREKT carisine iade alınacak.\n\nİade almak istediğinize emin misiniz?").arg(sepet[ui->SepetlertabWidget->currentIndex()].urunler.count()));
            msg.setInformativeText(QString("İade edilecek tutar: ₺%1").arg(sepet[ui->SepetlertabWidget->currentIndex()].sepetToplamTutari()));
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setDefaultButton(QMessageBox::Yes);
            msg.setButtonText(QMessageBox::Yes, "Evet");
            msg.setButtonText(QMessageBox::No, "Hayır");
            int cvp = msg.exec();
            if(cvp == QMessageBox::Yes){
                if(sepet[ui->SepetlertabWidget->currentIndex()].sepetToplamTutari() < vt->getKasadakiPara()){
                    // iade işlemleri başlangıcı.
                    vt->iadeAl(sepet[ui->SepetlertabWidget->currentIndex()], kullanici);
                    // sepet silme başlangıcı
                    sepet[ui->SepetlertabWidget->currentIndex()].sepetiSil();
                    switch (ui->SepetlertabWidget->currentIndex()) {
                    case 0:
                        sepet[0].sepetiSil();
                        ui->sepet1TableWidget->model()->removeRows(0, ui->sepet1TableWidget->rowCount());
                        break;
                    case 1:
                        sepet[1].sepetiSil();
                        ui->sepet2TableWidget->model()->removeRows(0, ui->sepet2TableWidget->rowCount());
                        break;
                    case 2:
                        sepet[2].sepetiSil();
                        ui->sepet3TableWidget->model()->removeRows(0, ui->sepet3TableWidget->rowCount());
                        break;
                    case 3:
                        sepet[3].sepetiSil();
                        ui->sepet4TableWidget->model()->removeRows(0, ui->sepet4TableWidget->rowCount());
                        break;
                    }
                    butonDurumlariniAyarla();
                    sepetToplaminiYaz();
                    getSonSatislar();
                    sepetTabIconlariAyarla();
                }
                else{
                    uyariSesi.play();
                    QMessageBox msg(this);
                    msg.setWindowTitle("Dikkat");
                    msg.setIcon(QMessageBox::Warning);
                    msg.setText("Kasada yeterli miktarda para yok!\n\nİade alamassınız.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.setButtonText(QMessageBox::Ok, "Tamam");
                    msg.exec();
                }
            }
        }
    }
    else{
        uyariSesi.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Dikkat");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("İade almaya yetkiniz yok.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_CarpBtn_clicked()
{
    SatisForm::adetCarp();
    ui->barkodLineEdit->setFocus();
}

void SatisForm::on_AyarlarBtn_clicked()
{
    if(kullanici.getAyaryetki()){
        AyarlarDialog *ayarForm = new AyarlarDialog(this);
        ayarForm->setCurrentUser(kullanici);
        ayarForm->exec();
        delete ayarForm;
        // veritabanı sıfırlandıysa hızlı butonları sıfırlasın/ayarlasın.
        hizliUrunButonlariAyarla();
        hizliUrunSayfaAyarla();
        getSonSatislar();
    }
    else{
        uyariSesi.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Ayarlara erişim ve düzenleme yetkiniz yoktur.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_CariKartlarBtn_clicked()
{
    if(kullanici.getCariyetki()){
        CariKartlarDialog *cariForm = new CariKartlarDialog(this);
        cariForm->setKullanici(kullanici);
        cariForm->exec();
    }
    else{
        uyariSesi.play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Cari işlemleri yapmaya yetkiniz yoktur.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    ui->barkodLineEdit->setFocus();
}

void SatisForm::KasadanParaCek()
{

}


void SatisForm::on_KartAratoolButton_clicked()
{
    HizliUrunEkleFormDialog *hizliDialog = new HizliUrunEkleFormDialog(this);
    hizliDialog->exec();
    if(hizliDialog->ok){
        if(!hizliDialog->hizliUrunBarkod.isNull() || !hizliDialog->hizliUrunBarkod.isEmpty()){
            ui->barkodLineEdit->setText(hizliDialog->hizliUrunBarkod);
            sepeteEkle();
        }
    }
    ui->barkodLineEdit->setFocus();
}

