#include "satisform.h"
#include "ui_satisform.h"
#include "sepet.h"
#include "stokfrom.h"
#include "stokkarti.h"
#include "satisyapform.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QMessageBox>

User *kullanici;
Sepet sepet[4];

QSqlDatabase db_satis;
QSqlQuery sorgu_satis;
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
    butonDurumlariniAyarla();
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
    sepetToplaminiYaz();
    butonDurumlariniAyarla();
}

void SatisForm::sepetToplaminiYaz()
{
    ui->ToplamTutarlcdNumber->display(sepet[ui->SepetlertabWidget->currentIndex()].sepetToplamTutari());
}

void SatisForm::barkodVarmi(QString bakilacakBarkod)
{
    sorgu_satis= QSqlQuery(db_satis);
    sorgu_satis.prepare("SELECT stokid, barkod, ad, birim, miktar, stokgrup, CAST(afiyat AS DECIMAL), CAST(sfiyat AS DECIMAL), kdv1, kdv2, kdv3, songuntarih, aciklama FROM stokkartlari WHERE barkod = ?");
    sorgu_satis.bindValue(0, bakilacakBarkod);
    sorgu_satis.exec();
    if(sorgu_satis.next()){
        if(sorgu_satis.value(4).toFloat() > 0){
            StokKarti stokkarti;
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

            //alt satırı if ile adet mi kilogrammı kontrol et ona göre urunEkle() çağır.
            sepet[ui->SepetlertabWidget->currentIndex()].urunEkle(stokkarti, 1);
            tableWidgetEkle(stokkarti);
            sepetToplaminiYaz();
        }
        else{
            QMessageBox::critical(this, "Dikkat", "ÜRÜN STOKTA YOK!", QMessageBox::Ok);
        }
    }
    else{
        QMessageBox::critical(this, "Uyarı", ui->barkodLineEdit->text() + "\n\nBARKODLU ÜRÜN BULUNAMADI!", QMessageBox::Ok);
    }
    ui->barkodLineEdit->clear();
    ui->barkodLineEdit->setFocus();
}

void SatisForm::tableWidgetEkle(StokKarti p_StokKarti)
{
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(urunSepetteVarmi(p_StokKarti.getBarkod())){
            float yeniMiktar = 1 + ui->sepet1TableWidget->model()->index(sepetMevcutUrunIndexi, 4).data().toFloat();
            ui->sepet1TableWidget->setItem(sepetMevcutUrunIndexi, 4, new QTableWidgetItem(QString::number(yeniMiktar)));
            ui->sepet1TableWidget->setItem(sepetMevcutUrunIndexi, 5, new QTableWidgetItem(QString::number(yeniMiktar * p_StokKarti.getSFiyat().toFloat())));
            ui->sepet1TableWidget->selectRow(sepetMevcutUrunIndexi);
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
            ui->sepet2TableWidget->selectRow(sepetMevcutUrunIndexi);
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
            ui->sepet3TableWidget->selectRow(sepetMevcutUrunIndexi);
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
            ui->sepet4TableWidget->selectRow(sepetMevcutUrunIndexi);
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
    butonDurumlariniAyarla();
}

void SatisForm::closeEvent(QCloseEvent *event)
{
    if(ui->sepet1TableWidget->rowCount() > 0 || ui->sepet2TableWidget->rowCount() > 0 || ui->sepet3TableWidget->rowCount() > 0 || ui->sepet4TableWidget->rowCount() > 0){
        QMessageBox closingMsgBox(QMessageBox::Question, tr("Dikkat"), tr("Satışı yapılmamış sepetiniz var!\n\nYine de çıkmak istediğinize emin misiniz?"), QMessageBox::Yes | QMessageBox::No, this);
        closingMsgBox.setButtonText(QMessageBox::Yes, "Evet");
        closingMsgBox.setButtonText(QMessageBox::No, "Hayır");
        closingMsgBox.setDefaultButton(QMessageBox::No);
        int cevap = closingMsgBox.exec();
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
    else if (event->key() == Qt::Key_Escape){
        this->close();
    }
}
/*!
    ürün sepette var ise true döndürür.
*/
bool SatisForm::urunSepetteVarmi(QString _Barkod)
{
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(ui->sepet1TableWidget->rowCount() >= 1){
            for (int i = 0; i < ui->sepet1TableWidget->rowCount(); ++i) {
                if(ui->sepet1TableWidget->item(i,0)->text() == _Barkod){
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
                if(ui->sepet2TableWidget->item(i,0)->text() == _Barkod){
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
                if(ui->sepet3TableWidget->item(i,0)->text() == _Barkod){
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
                if(ui->sepet4TableWidget->item(i,0)->text() == _Barkod){
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
        }
        guncelUrun = sepet[0].urunBilgileriniGetir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet1TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet1TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam)));
        break;
    case 1:
        if(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[1].urunArtir(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
        }
        guncelUrun = sepet[1].urunBilgileriniGetir(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet2TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet2TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam)));
        break;
    case 2:
        if(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[2].urunArtir(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
        }
        guncelUrun = sepet[2].urunBilgileriniGetir(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet3TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet3TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam)));
        break;
    case 3:
        if(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[3].urunArtir(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
        }
        guncelUrun = sepet[3].urunBilgileriniGetir(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet4TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet4TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam)));
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
        }
        guncelUrun = sepet[0].urunBilgileriniGetir(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet1TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet1TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam)));
        break;
    case 1:
        if(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[1].urunAzalt(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
        }
        guncelUrun = sepet[1].urunBilgileriniGetir(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet2TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet2TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam)));
        break;
    case 2:
        if(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[2].urunAzalt(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
        }
        guncelUrun = sepet[2].urunBilgileriniGetir(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet3TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet3TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam)));
        break;
    case 3:
        if(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "ADET"){
            sepet[3].urunAzalt(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString(), 1);
        }
        else if(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 3).data() == "KİLOGRAM"){
            // kilogram giriş formu gösterilecek alınan değer gönderilecek.
        }
        guncelUrun = sepet[3].urunBilgileriniGetir(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 0).data().toString());
        ui->sepet4TableWidget->setItem(getSeciliSatirIndexi(), 4, new QTableWidgetItem(QString::number(guncelUrun.miktar)));
        ui->sepet4TableWidget->setItem(getSeciliSatirIndexi(), 5, new QTableWidgetItem(QString::number(guncelUrun.toplam)));
        break;
    }
    sepetToplaminiYaz();
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_satirSilBtn_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setText("Dikkat");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, "Evet");
    msgBox.setButtonText(QMessageBox::No, "Hayır");
    msgBox.setModal(true);
    int cevap;
    switch (ui->SepetlertabWidget->currentIndex()) {
    case 0:
        if(ui->sepet1TableWidget->rowCount() > 0){
            msgBox.setInformativeText(QString("%1 \n\nürününü sepetten silmek istediğinize emin misiniz?").arg(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(), 1).data().toString()));
            cevap = msgBox.exec();
            switch (cevap) {
            case QMessageBox::Yes:
                sepet[0].urunSil(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(),0).data().toString());
                ui->sepet1TableWidget->removeRow(getSeciliSatirIndexi());
                ui->sepet1TableWidget->selectRow(ui->sepet1TableWidget->rowCount() - 1);
                break;
            }
        }
        break;
    case 1:
        if(ui->sepet2TableWidget->rowCount() > 0){
            msgBox.setInformativeText(QString("%1 \n\nürününü sepetten silmek istediğinize emin misiniz?").arg(ui->sepet2TableWidget->model()->index(getSeciliSatirIndexi(), 1).data().toString()));
            cevap = msgBox.exec();
            switch (cevap) {
            case QMessageBox::Yes:
                sepet[1].urunSil(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(),0).data().toString());
                ui->sepet2TableWidget->removeRow(getSeciliSatirIndexi());
                ui->sepet2TableWidget->selectRow(ui->sepet2TableWidget->rowCount() - 1);
                break;
            }
        }
        break;
    case 2:
        if(ui->sepet3TableWidget->rowCount() > 0){
            msgBox.setInformativeText(QString("%1 \n\nürününü sepetten silmek istediğinize emin misiniz?").arg(ui->sepet3TableWidget->model()->index(getSeciliSatirIndexi(), 1).data().toString()));
            cevap = msgBox.exec();
            switch (cevap) {
            case QMessageBox::Yes:
                sepet[2].urunSil(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(),0).data().toString());
                ui->sepet3TableWidget->removeRow(getSeciliSatirIndexi());
                ui->sepet3TableWidget->selectRow(ui->sepet3TableWidget->rowCount() - 1);
                break;
            }
        }
        break;
    case 3:
        if(ui->sepet4TableWidget->rowCount() > 0){
            msgBox.setInformativeText(QString("%1 \n\nürününü sepetten silmek istediğinize emin misiniz?").arg(ui->sepet4TableWidget->model()->index(getSeciliSatirIndexi(), 1).data().toString()));
            cevap = msgBox.exec();
            switch (cevap) {
            case QMessageBox::Yes:
                sepet[3].urunSil(ui->sepet1TableWidget->model()->index(getSeciliSatirIndexi(),0).data().toString());
                ui->sepet4TableWidget->removeRow(getSeciliSatirIndexi());
                ui->sepet4TableWidget->selectRow(ui->sepet4TableWidget->rowCount() - 1);
                break;
            }
        }
        break;
    }
    sepetToplaminiYaz();
    butonDurumlariniAyarla();
    ui->barkodLineEdit->setFocus();
}


void SatisForm::on_sepetSilBtn_clicked()
{
    QMessageBox sepetSilMsgBox(this);
    sepetSilMsgBox.setText("Dikkat");
    sepetSilMsgBox.setInformativeText("Seçili Sepeti silmek istediğinize emin misiniz?");
    sepetSilMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    sepetSilMsgBox.setDefaultButton(QMessageBox::No);
    sepetSilMsgBox.setButtonText(QMessageBox::Yes, "Evet");
    sepetSilMsgBox.setButtonText(QMessageBox::No, "Hayır");
    sepetSilMsgBox.setModal(true);
    int cevap = sepetSilMsgBox.exec();
    if(cevap == QMessageBox::Yes){
        switch (ui->SepetlertabWidget->currentIndex()) {
        case 0:
            sepet[0].sepetiSil();
            ui->sepet1TableWidget->model()->removeRows(0, ui->sepet1TableWidget->rowCount());
            ui->ToplamTutarlcdNumber->display(0);
            break;
        case 1:
            sepet[1].sepetiSil();
            ui->sepet2TableWidget->model()->removeRows(0, ui->sepet2TableWidget->rowCount());
            ui->ToplamTutarlcdNumber->display(0);
            break;
        case 2:
            sepet[2].sepetiSil();
            ui->sepet3TableWidget->model()->removeRows(0, ui->sepet3TableWidget->rowCount());
            ui->ToplamTutarlcdNumber->display(0);
            break;
        case 3:
            sepet[3].sepetiSil();
            ui->sepet4TableWidget->model()->removeRows(0, ui->sepet4TableWidget->rowCount());
            ui->ToplamTutarlcdNumber->display(0);
            break;
        }
    }
    sepetToplaminiYaz();
    butonDurumlariniAyarla();
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
        }
        else{
            ui->artirBtn->setEnabled(false);
            ui->azaltBtn->setEnabled(false);
            ui->satirSilBtn->setEnabled(false);
            ui->sepetSilBtn->setEnabled(false);
            ui->iadeAlBtn->setEnabled(false);
            ui->satisYapBtn->setEnabled(false);
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
        }
        else{
            ui->artirBtn->setEnabled(false);
            ui->azaltBtn->setEnabled(false);
            ui->satirSilBtn->setEnabled(false);
            ui->sepetSilBtn->setEnabled(false);
            ui->iadeAlBtn->setEnabled(false);
            ui->satisYapBtn->setEnabled(false);
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
        }
        else{
            ui->artirBtn->setEnabled(false);
            ui->azaltBtn->setEnabled(false);
            ui->satirSilBtn->setEnabled(false);
            ui->sepetSilBtn->setEnabled(false);
            ui->iadeAlBtn->setEnabled(false);
            ui->satisYapBtn->setEnabled(false);
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
        }
        else{
            ui->artirBtn->setEnabled(false);
            ui->azaltBtn->setEnabled(false);
            ui->satirSilBtn->setEnabled(false);
            ui->sepetSilBtn->setEnabled(false);
            ui->iadeAlBtn->setEnabled(false);
            ui->satisYapBtn->setEnabled(false);
        }
        break;
    }
}


void SatisForm::on_satisYapBtn_clicked()
{
    SatisYapForm *satisyapfrm = new SatisYapForm(this);
    satisyapfrm->satilacakSepet = sepet[ui->SepetlertabWidget->currentIndex()];
    satisyapfrm->exec();
}

