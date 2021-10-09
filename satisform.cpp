#include "satisform.h"
#include "ui_satisform.h"
#include "user.h"
#include "stokfrom.h"
#include "sepet.h"
#include "stokkarti.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QMessageBox>

User *kullanici;
Sepet sepet;
QSqlDatabase db_satis;
QSqlQuery sorgu_satis;
StokKarti stokkarti;
QTableWidgetItem *yeniSatir;
int sepetMevcutUrunIndexi = 0;


SatisForm::SatisForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SatisForm)
{
    ui->setupUi(this);

    formLoad();
}

SatisForm::~SatisForm()
{
    delete ui;
}

void SatisForm::setUser(User *user)
{
    kullanici = user;
    this->setWindowTitle("MHSS - " + kullanici->getAdi());
}

void SatisForm::formLoad()
{
    db_satis = QSqlDatabase::addDatabase("QPSQL", "db_satis");
    db_satis.setDatabaseName("mhss_data");
    db_satis.setHostName("localhost");
    db_satis.setUserName("admin");
    db_satis.setPassword("admin");
    if(!db_satis.isOpen())
    {
        db_satis.open();
    }

    initTableWidgets();

    ui->barkodLineEdit->installEventFilter(this);
    ui->barkodLineEdit->setFocus();
}

void SatisForm::on_StokKartlariBtn_clicked()
{
    StokFrom *stokKartiFormu = new StokFrom(this);
    stokKartiFormu->exec();
}

void SatisForm::initTableWidgets()
{
    ui->SepetlertabWidget->setCurrentIndex(0);
    ui->HizliUrunlertabWidget->setCurrentIndex(0);
    ui->sepet1TableWidget->setColumnWidth(0,135);
    ui->sepet1TableWidget->setColumnWidth(1,200);
    ui->sepet1TableWidget->setColumnWidth(2,95);
    ui->sepet1TableWidget->setColumnWidth(3,88);
    ui->sepet1TableWidget->setColumnWidth(4,45);
    ui->sepet1TableWidget->setColumnWidth(5,100);
    ui->sepet2TableWidget->setColumnWidth(0,135);
    ui->sepet2TableWidget->setColumnWidth(1,200);
    ui->sepet2TableWidget->setColumnWidth(2,95);
    ui->sepet2TableWidget->setColumnWidth(3,88);
    ui->sepet2TableWidget->setColumnWidth(4,45);
    ui->sepet2TableWidget->setColumnWidth(5,100);
    ui->sepet3TableWidget->setColumnWidth(0,135);
    ui->sepet3TableWidget->setColumnWidth(1,200);
    ui->sepet3TableWidget->setColumnWidth(2,95);
    ui->sepet3TableWidget->setColumnWidth(3,88);
    ui->sepet3TableWidget->setColumnWidth(4,45);
    ui->sepet3TableWidget->setColumnWidth(5,100);
    ui->sepet4TableWidget->setColumnWidth(0,135);
    ui->sepet4TableWidget->setColumnWidth(1,200);
    ui->sepet4TableWidget->setColumnWidth(2,95);
    ui->sepet4TableWidget->setColumnWidth(3,88);
    ui->sepet4TableWidget->setColumnWidth(4,45);
    ui->sepet4TableWidget->setColumnWidth(5,100);
}


void SatisForm::on_SepetlertabWidget_currentChanged(int index)
{
    sepetToplaminiYaz(index);
}

void SatisForm::sepetToplaminiYaz(int p_HangiSepet)
{
    double toplam = 0;
    switch (p_HangiSepet) {
    case 0:
        for (int i = 0; i < ui->sepet1TableWidget->rowCount(); ++i) {
            toplam += ui->sepet1TableWidget->item(i,5)->text().toDouble();
        }
        break;
    case 1:
        for (int i = 0; i < ui->sepet2TableWidget->rowCount(); ++i) {
            toplam += ui->sepet2TableWidget->item(i,5)->text().toDouble();
        }
        break;
    case 2:
        for (int i = 0; i < ui->sepet3TableWidget->rowCount(); ++i) {
            toplam += ui->sepet3TableWidget->item(i,5)->text().toDouble();
        }
        break;
    case 3:
        for (int i = 0; i < ui->sepet4TableWidget->rowCount(); ++i) {
            toplam += ui->sepet4TableWidget->item(i,5)->text().toDouble();
        }
        break;
    }
    ui->ToplamTutarlcdNumber->display(toplam);
}

void SatisForm::barkodVarmi(QString bakilacakBarkod)
{
    sorgu_satis= QSqlQuery(db_satis);
    sorgu_satis.prepare("SELECT stokid, barkod, ad, birim, miktar, stokgrup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv1, kdv2, kdv3, songuntarih, aciklama FROM stokkartlari WHERE barkod = ?");
    sorgu_satis.bindValue(0, bakilacakBarkod);
    sorgu_satis.exec();
    if(sorgu_satis.next()){
        stokkarti = StokKarti();
        stokkarti.setID(sorgu_satis.value(0).toString());
        stokkarti.setBarkod(sorgu_satis.value(1).toString());
        stokkarti.setAd(sorgu_satis.value(2).toString());
        stokkarti.setBirim(sorgu_satis.value(3).toString());
        stokkarti.setMiktar(sorgu_satis.value(4).toString());
        stokkarti.setGrup(sorgu_satis.value(5).toString());
        stokkarti.setAFiyat(sorgu_satis.value(6).toString());
        stokkarti.setSFiyat(sorgu_satis.value(7).toString());
        stokkarti.setKDV1(sorgu_satis.value(8).toInt());
        stokkarti.setKDV2(sorgu_satis.value(9).toInt());
        stokkarti.setKDV3(sorgu_satis.value(10).toInt());
        stokkarti.setSonGunTarih(sorgu_satis.value(11).toDateTime());
        stokkarti.setAciklama(sorgu_satis.value(12).toString());
        sepeteEkle(stokkarti);
    }
    else{
        QMessageBox::critical(this, "Uyarı", ui->barkodLineEdit->text() + " barkodlu ürün veritabanında bulunamadı.", QMessageBox::Ok);
    }
    ui->barkodLineEdit->clear();
    ui->barkodLineEdit->setFocus();
}

void SatisForm::sepeteEkle(StokKarti p_StokKarti)
{
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(urunSepetteVarmi(p_StokKarti.getBarkod())){
            float yeniMiktar = 1 + ui->sepet1TableWidget->model()->index(sepetMevcutUrunIndexi, 4).data().toFloat();
            ui->sepet1TableWidget->setItem(sepetMevcutUrunIndexi, 4, new QTableWidgetItem(QString::number(yeniMiktar)));
            ui->sepet1TableWidget->setItem(sepetMevcutUrunIndexi, 5, new QTableWidgetItem(QString::number(yeniMiktar * p_StokKarti.getSFiyat().toFloat())));
            ui->sepet1TableWidget->selectRow(ui->sepet1TableWidget->rowCount());
        }
        else{
            ui->sepet1TableWidget->insertRow(ui->sepet1TableWidget->rowCount());
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 0, new QTableWidgetItem(p_StokKarti.getBarkod()));
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 1, new QTableWidgetItem(p_StokKarti.getAd()));
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 2, new QTableWidgetItem(p_StokKarti.getSFiyat()));
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 3, new QTableWidgetItem(p_StokKarti.getBirim()));
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 4, new QTableWidgetItem("1"));
            ui->sepet1TableWidget->setItem(ui->sepet1TableWidget->rowCount() - 1, 5, new QTableWidgetItem(p_StokKarti.getSFiyat()));
            ui->sepet1TableWidget->selectRow(ui->sepet1TableWidget->rowCount() - 1);
        }
        break;
    case 1:
        if(urunSepetteVarmi(p_StokKarti.getBarkod())){
            float yeniMiktar = 1 + ui->sepet2TableWidget->model()->index(sepetMevcutUrunIndexi, 4).data().toFloat();
            ui->sepet2TableWidget->setItem(sepetMevcutUrunIndexi, 4, new QTableWidgetItem(QString::number(yeniMiktar)));
            ui->sepet2TableWidget->setItem(sepetMevcutUrunIndexi, 5, new QTableWidgetItem(QString::number(yeniMiktar * p_StokKarti.getSFiyat().toFloat())));
            ui->sepet2TableWidget->selectRow(ui->sepet2TableWidget->rowCount());
        }
        else{
            ui->sepet2TableWidget->insertRow(ui->sepet2TableWidget->rowCount());
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 0, new QTableWidgetItem(p_StokKarti.getBarkod()));
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 1, new QTableWidgetItem(p_StokKarti.getAd()));
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 2, new QTableWidgetItem(p_StokKarti.getSFiyat()));
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 3, new QTableWidgetItem(p_StokKarti.getBirim()));
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 4, new QTableWidgetItem("1"));
            ui->sepet2TableWidget->setItem(ui->sepet2TableWidget->rowCount() - 1, 5, new QTableWidgetItem(p_StokKarti.getSFiyat()));
            ui->sepet2TableWidget->selectRow(ui->sepet2TableWidget->rowCount() - 1);
        }
        break;
    case 2:
        if(urunSepetteVarmi(p_StokKarti.getBarkod())){
            float yeniMiktar = 1 + ui->sepet3TableWidget->model()->index(sepetMevcutUrunIndexi, 4).data().toFloat();
            ui->sepet3TableWidget->setItem(sepetMevcutUrunIndexi, 4, new QTableWidgetItem(QString::number(yeniMiktar)));
            ui->sepet3TableWidget->setItem(sepetMevcutUrunIndexi, 5, new QTableWidgetItem(QString::number(yeniMiktar * p_StokKarti.getSFiyat().toFloat())));
            ui->sepet3TableWidget->selectRow(ui->sepet3TableWidget->rowCount());
        }
        else{
            ui->sepet3TableWidget->insertRow(ui->sepet3TableWidget->rowCount());
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 0, new QTableWidgetItem(p_StokKarti.getBarkod()));
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 1, new QTableWidgetItem(p_StokKarti.getAd()));
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 2, new QTableWidgetItem(p_StokKarti.getSFiyat()));
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 3, new QTableWidgetItem(p_StokKarti.getBirim()));
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 4, new QTableWidgetItem("1"));
            ui->sepet3TableWidget->setItem(ui->sepet3TableWidget->rowCount() - 1, 5, new QTableWidgetItem(p_StokKarti.getSFiyat()));
            ui->sepet3TableWidget->selectRow(ui->sepet3TableWidget->rowCount() - 1);
        }
        break;
    case 3:
        if(urunSepetteVarmi(p_StokKarti.getBarkod())){
            float yeniMiktar = 1 + ui->sepet4TableWidget->model()->index(sepetMevcutUrunIndexi, 4).data().toFloat();
            ui->sepet4TableWidget->setItem(sepetMevcutUrunIndexi, 4, new QTableWidgetItem(QString::number(yeniMiktar)));
            ui->sepet4TableWidget->setItem(sepetMevcutUrunIndexi, 5, new QTableWidgetItem(QString::number(yeniMiktar * p_StokKarti.getSFiyat().toFloat())));
            ui->sepet4TableWidget->selectRow(ui->sepet4TableWidget->rowCount());
        }
        else{
            ui->sepet4TableWidget->insertRow(ui->sepet4TableWidget->rowCount());
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 0, new QTableWidgetItem(p_StokKarti.getBarkod()));
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 1, new QTableWidgetItem(p_StokKarti.getAd()));
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 2, new QTableWidgetItem(p_StokKarti.getSFiyat()));
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 3, new QTableWidgetItem(p_StokKarti.getBirim()));
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 4, new QTableWidgetItem("1"));
            ui->sepet4TableWidget->setItem(ui->sepet4TableWidget->rowCount() - 1, 5, new QTableWidgetItem(p_StokKarti.getSFiyat()));
            ui->sepet4TableWidget->selectRow(ui->sepet4TableWidget->rowCount() - 1);
        }
        break;
    }
    sepetToplaminiYaz(ui->SepetlertabWidget->currentIndex());
}

void SatisForm::closeEvent(QCloseEvent *event)
{
    if(ui->sepet1TableWidget->rowCount() > 0 || ui->sepet2TableWidget->rowCount() > 0 || ui->sepet3TableWidget->rowCount() > 0 || ui->sepet4TableWidget->rowCount() > 0){
        QMessageBox msgBox(QMessageBox::Question, tr("Dikkat"), tr("Satışı yapılmamış sepetiniz var!\n\nYinede çıkmak istediğinize emin misiniz?"), QMessageBox::Yes | QMessageBox::No, this);
        msgBox.setButtonText(QMessageBox::Yes,"Evet");
        msgBox.setButtonText(QMessageBox::No, "Hayır");
        msgBox.setDefaultButton(QMessageBox::No);
        int cevap = msgBox.exec();
        switch (cevap) {
        case QMessageBox::Yes:
            this->close();
            break;
        case QMessageBox::No:
            event->ignore();
            this->show();
            break;
        }
    }
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
        }
        return false;
    }
    return SatisForm::eventFilter(filtrelenecekObject, event);      //
}

void SatisForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        if(ui->barkodLineEdit->text().isEmpty()){
            //ödeme ekranı işlemleri.
        }
        else{
            SatisForm::barkodVarmi(ui->barkodLineEdit->text());
        }
    }
    else if(event->key() == Qt::DownArrow || event->key() == Qt::Key_Down){
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
    else if(event->key() == Qt::UpArrow || event->key() == Qt::Key_Up){
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
}

bool SatisForm::urunSepetteVarmi(QString p_Barkod)
{
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(ui->sepet1TableWidget->rowCount() >= 1){
            for (int i = 0; i < ui->sepet1TableWidget->rowCount(); ++i) {
                if(ui->sepet1TableWidget->item(i,0)->text() == p_Barkod){
                    sepetMevcutUrunIndexi = i;
                    return true;
                }
            }
        }
        else{
            return false;
        }
        break;
    case 1:
        if(ui->sepet2TableWidget->rowCount() >= 1){
            for (int i = 0; i < ui->sepet2TableWidget->rowCount(); ++i) {
                if(ui->sepet2TableWidget->item(i,0)->text() == p_Barkod){
                    sepetMevcutUrunIndexi = i;
                    return true;
                }
            }
        }
        else{
            return false;
        }
        break;
    case 2:
        if(ui->sepet3TableWidget->rowCount() >= 1){
            for (int i = 0; i < ui->sepet3TableWidget->rowCount(); ++i) {
                if(ui->sepet3TableWidget->item(i,0)->text() == p_Barkod){
                    sepetMevcutUrunIndexi = i;
                    return true;
                }
            }
        }
        else{
            return false;
        }
        break;
    case 3:
        if(ui->sepet4TableWidget->rowCount() >= 1){
            for (int i = 0; i < ui->sepet4TableWidget->rowCount(); ++i) {
                if(ui->sepet4TableWidget->item(i,0)->text() == p_Barkod){
                    sepetMevcutUrunIndexi = i;
                    return true;
                }
            }
        }
        else{
            return false;
        }
        break;
    }
}

