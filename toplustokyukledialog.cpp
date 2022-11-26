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
#include "toplustokyukledialog.h"
#include "ui_toplustokyukledialog.h"
#include "stokkartiform.h"
//*****************************************
#include <QStandardPaths>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QFileDialog>
#include <QStandardPaths>
#include <QProcess>
#include <QTcpSocket>

TopluStokYukleDialog::TopluStokYukleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopluStokYukleDialog)
{
    ui->setupUi(this);
}

TopluStokYukleDialog::~TopluStokYukleDialog()
{
    delete ui;
}

void TopluStokYukleDialog::on_SecpushButton_clicked()
{
    dosyaYolu = QFileDialog::getOpenFileName(this,
                                         "Stok Kartları Dosyasını Seçin",
                                         QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                         tr("JSON Dosyası (stok-kartlari.json);; CSV Dosyası (stok-kartlari.csv)"));

    stokKartiSayisiYaz(dosyaYolu);
}

void TopluStokYukleDialog::on_YuklepushButton_clicked()
{

    if(dosya.fileName() != ""){
        ui->Bilgilabel->setText("Aktarılıyor...");

        if(QFileInfo(dosya).suffix().contains("json", Qt::CaseInsensitive)){
            jsondanYukle();
        }
        else if(QFileInfo(dosya).suffix().contains("csv", Qt::CaseInsensitive)){
            csvdenYukle();
        }

        ui->Bilgilabel->setText("Aktarım tamamlandı.");

        uyariSesi->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Warning);
        msg.setText(tr("%1 stok kartı veritabanına girilmiştir.").arg(yuklenen) + tr("\n%1 adet stok kartı zaten mevcut.").arg(esGecilen) + tr("\n%1 adet stok kartı yüklemesi başarısız.").arg(basarisiz));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    else{
        ui->Bilgilabel->setText("Aktarılamadı !");

        uyariSesi->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Önce Stok kartları dosyası seçilmeli.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
}

void TopluStokYukleDialog::jsondanYukle()
{
    dosya.open(QIODevice::ReadOnly | QIODevice::Text);
    QString string = dosya.readAll();
    dosya.close();
    QJsonDocument document = QJsonDocument::fromJson(string.toUtf8());
    QJsonObject object =document.object();

    float ozelMiktar = ui->doubleSpinBoxMiktar->value();

    foreach (QString barkod, object.keys()) {
        if(!stokYonetimi.barkodVarmi(barkod)){
            QJsonValue value = object.value(barkod);

            if(ui->kontrolEderekcheckBox->isChecked()){
                StokKarti yKart;
                yKart.setBarkod(barkod);
                yKart.setKod(value["kod"].toString());
                yKart.setAd(value["ad"].toString());
                yKart.setBirim(value["birim"].toInt());
                if(ui->miktarcheckBox->isChecked()){
                    yKart.setMiktar(ozelMiktar);
                }
                else{
                    yKart.setMiktar(value["miktar"].toVariant().value<float>());
                }
                yKart.setGrup(value["birim"].toInt());
                yKart.setAFiyat(value["afiyat"].toDouble());
                yKart.setSFiyat(value["sfiyat"].toDouble());
                yKart.setKdv(value["kdv"].toInt());
                yKart.setKdvdahil(value["kdvdahil"].toBool());
                yKart.setOtv(value["otv"].toInt());
                yKart.setOtvdahil(value["otvdahil"].toBool());
                yKart.setTarih(value["tarih"].toVariant().value<QDateTime>());
                yKart.setUretici(stokYonetimi.getUreticiAD(value["uretici"].toInt()));
                yKart.setAciklama(value["aciklama"].toString());
                StokKartiForm *stokKartiForm =  new StokKartiForm(this);
                stokKartiForm->setKullanici(kullanici);
                stokKartiForm->setWindowTitle("Kontrollü Toplu Stok Kartı Girişi");
                stokKartiForm->toplustokkarti = true;
                stokKartiForm->FormLoad();
                stokKartiForm->setKart(yKart);
                stokKartiForm->exec();

                if(stokKartiForm->toplustokkartiIptal){
                    uyariSesi->play();
                    QMessageBox msg(this);
                    msg.setWindowTitle("Uyarı");
                    msg.setIcon(QMessageBox::Warning);
                    msg.setText("Toplu stok kartı aktarımı iptal edildi.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.setButtonText(QMessageBox::Ok, "Tamam");
                    msg.exec();
                    return;
                }
                if(stokKartiForm->kayitBasarilimi){
                    yuklenen++;
                }
                else{
                    basarisiz++;
                }
                delete stokKartiForm;
            }
            else{
                sorgu.prepare("INSERT INTO stokkartlari(id, barkod, kod, ad, birim, miktar, grup, afiyat, sfiyat, kdv, kdvdahil, otv, otvdahil, tarih, uretici, tedarikci, aciklama) "
                                "VALUES(nextval('stokkartlari_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                sorgu.bindValue(0, barkod);
                sorgu.bindValue(1, value["kod"].toString());
                sorgu.bindValue(2, value["ad"].toString());
                //sorgu.bindValue(3, value["birim"].toString());
                sorgu.bindValue(3, value["birim"].toInt());
                if(ui->miktarcheckBox->isChecked()){// seçili ise miktarları özelmiktar adet/kg/metre girer.
                    sorgu.bindValue(4, ozelMiktar);
                }
                else{
                    sorgu.bindValue(4, value["miktar"].toVariant().value<float>());
                }
                sorgu.bindValue(5, value["grup"].toInt());
                if(ui->fiyatcheckBox->isChecked()){// seçili ise fiyatları 0 TL girer.
                    sorgu.bindValue(6, 0);
                    sorgu.bindValue(7, 0);
                }
                else{
                    sorgu.bindValue(6, value["afiyat"].toDouble());
                    sorgu.bindValue(7, value["sfiyat"].toDouble());
                }
                sorgu.bindValue(8, value["kdv"].toInt());
                sorgu.bindValue(9, value["kdvdahil"].toBool());
                sorgu.bindValue(10, value["otv"].toInt());
                sorgu.bindValue(11, value["otvdahil"].toBool());
                QDateTime zaman = value["tarih"].toVariant().value<QDateTime>();
                sorgu.bindValue(12, value["tarih"].toVariant().value<QDateTime>());
                sorgu.bindValue(13, value["uretici"].toInt());
                sorgu.bindValue(14, QVariant(QString()));
                sorgu.bindValue(15, value["aciklama"].toString());
                sorgu.exec();
                if(sorgu.lastError().isValid()){
                    qDebug() << qPrintable(sorgu.lastError().text());
                    basarisiz++;
                }
                yuklenen++;
            }
        }
        else{
            esGecilen++;
        }
        ui->progressBar->setValue(ui->progressBar->value() + 1);
    }
}

void TopluStokYukleDialog::csvdenYukle()
{
    if(!dosya.open(QIODevice::ReadOnly)){
        qDebug() << qPrintable(dosya.errorString());

        uyariSesi->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Warning);
        msg.setText(QString("Dosya açılamadı.\n\nHata mesajı:\n\n%1").arg(dosya.errorString()));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        return;
    }

    QTextStream stream(&dosya);

    bool ilksatir = true;
    while(!stream.atEnd()){

        QString satir = stream.readLine();

        if(!ilksatir){
            QStringList csvKart = satir.split(QLatin1Char(','));

            StokKarti kart;
            kart.setBarkod(QString(csvKart.value(0)));
            kart.setKod(turkce.toUpper(QString(csvKart.value(1))));
            kart.setAd(turkce.toUpper(QString(csvKart.value(2))));
            kart.setBirim(QString(csvKart.at(3)).toInt());
            kart.setMiktar(QString(csvKart.value(4)).toFloat());
            kart.setGrup(QString(csvKart.value(5)).toInt());
            kart.setAFiyat(QString(csvKart.value(6)).toDouble());
            kart.setSFiyat(QString(csvKart.value(7)).toDouble());
            kart.setKdv(QString(csvKart.value(8)).toInt());
            if(QString(csvKart.value(9)).contains("evet", Qt::CaseInsensitive)){
                kart.setKdvdahil(true);
            }
            else{
                kart.setKdvdahil(false);
            }
            kart.setOtv(QString(csvKart.value(10)).toInt());
            if(QString(csvKart.value(11)).contains("evet", Qt::CaseInsensitive)){
                kart.setOtvdahil(true);
            }
            else{
                kart.setOtvdahil(false);
            }
            kart.setUretici(NULL);
            kart.setTedarikci(NULL);
            kartlar.append(kart);
        }
        ilksatir = false;
    }

    dosya.close();

    foreach (StokKarti kart, kartlar) {
        if(!stokYonetimi.barkodVarmi(kart.getBarkod())){
            sorgu.prepare("INSERT INTO stokkartlari(id, barkod, kod, ad, birim, miktar, grup, afiyat, sfiyat, kdv, kdvdahil, otv, otvdahil, tarih, uretici, tedarikci) "
                          "VALUES(nextval('stokkartlari_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            sorgu.bindValue(0, kart.getBarkod());
            sorgu.bindValue(1, kart.getKod());
            sorgu.bindValue(2, kart.getAd());
            sorgu.bindValue(3, kart.getBirim());
            sorgu.bindValue(4, kart.getMiktar());
            sorgu.bindValue(5, kart.getGrup());
            sorgu.bindValue(6, kart.getAFiyat());
            sorgu.bindValue(7, kart.getSFiyat());
            sorgu.bindValue(8, kart.getKdv());
            sorgu.bindValue(9, kart.getKdvdahil());
            sorgu.bindValue(10, kart.getOtv());
            sorgu.bindValue(11, kart.getOtvdahil());
            sorgu.bindValue(12, QDateTime::currentDateTime());
            sorgu.bindValue(13, kart.getUretici());
            sorgu.bindValue(14, kart.getTedarikci());
            sorgu.exec();
            if(sorgu.lastError().isValid()){
                basarisiz++;
            }
            else{
                yuklenen++;
            }
        }
        else{
            esGecilen++;
            mevcutBarkodlar.append(kart.getBarkod());
        }
        ui->progressBar->setValue(ui->progressBar->value() + 1);
    }
}

void TopluStokYukleDialog::stokKartiSayisiYaz(QString dosyaURL)
{
    dosya.setFileName(dosyaURL);
    QFileInfo dosyaBilgisi(dosya);
    ui->DosyaAdilabel->setText(dosyaBilgisi.absoluteFilePath());

    // dosya json ise içindeki stok kartı sayısını labele yazma.
    if(dosyaBilgisi.suffix().contains("json", Qt::CaseInsensitive)){
        dosya.open(QIODevice::ReadOnly | QIODevice::Text);
        QString string = dosya.readAll();
        dosya.close();

        QJsonDocument document = QJsonDocument::fromJson(string.toUtf8());
        QJsonObject object =document.object();
        ui->Bilgilabel->setText("Toplam: " + QString::number(object.keys().count()) + " adet stok kartı var");
        ui->progressBar->setMaximum(object.keys().count());

        ui->AyarlargroupBox->setEnabled(true);
    }
    // dosya csv ise içindeki stok kartı sayısını labele yazma.
    if(dosyaBilgisi.suffix().contains("csv", Qt::CaseInsensitive)){
        if(!dosya.open(QIODevice::ReadOnly)){
            uyariSesi->play();
            QMessageBox::information(this, "Hata", tr("seçilen csv dosyası açılamadı\n\nHata mesajı:\n").arg(dosya.errorString()), QMessageBox::Ok);
        }

        int stokkartiSayisi = 0;

        QTextStream stream(&dosya);
        while(!stream.atEnd()){
            // satırı okuyup sonrakine geçsin diye
            stream.readLine();

            stokkartiSayisi++;
        }
        dosya.close();

        ui->AyarlargroupBox->setEnabled(false);

        ui->Bilgilabel->setText("Toplam: " + QString::number(stokkartiSayisi - 1) + " adet stok kartı var");
        ui->progressBar->setMaximum(stokkartiSayisi - 1);
    }
}


void TopluStokYukleDialog::on_KapatpushButton_clicked()
{
    this->close();
}

void TopluStokYukleDialog::on_CsvDosyaCikartButton_clicked()
{
    QFileDialog dialog(this);
    QString dizin = dialog.getExistingDirectory(this, tr("Kayıt yeri seçin."), QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    // kullanıcı iptal derse
    if(dizin.isEmpty()){
        return;
    }
    QString yeniKopyalanacakKonum = dizin + "/stok-kartlari.csv";

    // dosya mevcutmu ve mevcutsa sor değilse direkt kopyala
    if(!QFileInfo(yeniKopyalanacakKonum).exists()){
        QFile(":/dosyalar/dosyalar/stok-kartlari.csv").copy(yeniKopyalanacakKonum);
        QFile yeniCSV(yeniKopyalanacakKonum);
        yeniCSV.setPermissions(QFileDevice::WriteUser | QFileDevice::ReadUser);

        uyariSesi->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Dosya oluşturuldu.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
    }
    else{
        uyariSesi->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Uyarı");
        msg.setIcon(QMessageBox::Question);
        msg.setText("Dosya zaten mevcut üzerine yazayım mı?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setButtonText(QMessageBox::Yes, "Evet");
        msg.setButtonText(QMessageBox::No, "Hayır");
        int cevap = msg.exec();
        if(cevap == QMessageBox::Yes){
            QFile(yeniKopyalanacakKonum).remove();
            QFile bosCSV(":/dosyalar/dosyalar/stok-kartlari.csv");
            bosCSV.copy(yeniKopyalanacakKonum);
            bosCSV.setPermissions(QFileDevice::WriteUser | QFileDevice::ReadUser);
        }
    }
}

void TopluStokYukleDialog::on_sunucudanpushButton_clicked()
{
    //internet bağlantı kontrolü
    if(check()){
        QProcess process;
        QDir kartlarDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/kartlar");
        if(!kartlarDir.exists()){
            ui->Bilgilabel->setText("Sunucudan klonlanıyor...");
            QDir().mkpath(kartlarDir.absolutePath());
            process.start("git", QStringList() << "clone" << "https://mls.akdeniz.edu.tr/git/mamoo/kartlar.git" << kartlarDir.absolutePath());
            process.waitForFinished();
            qDebug() << "git kartlar sunucudan klonlandı.\n" << process.readAll();
            stokKartiSayisiYaz(kartlarDir.absolutePath() + "/stok-kartlari.json");
            ui->Bilgilabel->setText("sunucudan klonlama bitti");
        }
        else{
            ui->Bilgilabel->setText("uzak sunucudan güncelleniyor...");
            process.start("git", QStringList() << "-C" << kartlarDir.absolutePath() << "fetch");
            process.waitForFinished();
            process.start("git", QStringList() << "-C" << kartlarDir.absolutePath() << "status");
            process.waitForFinished();
            QString str = process.readAll();
            if(str.contains("güncel.")){
                ui->Bilgilabel->setText("uzak sunucudan güncellendi");
                uyariSesi->play();
                QMessageBox msg(this);
                msg.setWindowTitle("Bilgi");
                msg.setIcon(QMessageBox::Information);
                msg.setText("Yereldeki kartlar deposu zaten sunucu ile aynı.");
                msg.setInformativeText(str);
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
            }
            else{
                uyariSesi->play();
                QMessageBox msg(this);
                msg.setWindowTitle("Bilgi");
                msg.setIcon(QMessageBox::Question);
                msg.setText("Sunucuya yeni stok kartları eklenmiş. Yerel depoyu sunucu ile eşitlemek istiyor musunuz?");
                msg.setInformativeText(str);
                msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msg.setDefaultButton(QMessageBox::Yes);
                int cvp = msg.exec();
                if(cvp == QMessageBox::Yes){
                    ui->Bilgilabel->setText("Yeni kartlar indiriliyor...");
                    process.start("git", QStringList() << "-C" << kartlarDir.absolutePath() << "pull");
                    process.waitForFinished();
                    uyariSesi->play();
                    QMessageBox msg(this);
                    msg.setWindowTitle("Bilgi");
                    msg.setIcon(QMessageBox::Information);
                    msg.setText("Yeni stok kartları yerel depoya eklendi.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    ui->Bilgilabel->setText("Yeni kartlar indirildi");
                    msg.exec();
                }
            }
            stokKartiSayisiYaz(kartlarDir.absolutePath() + "/stok-kartlari.json");
        }
    }
    else{// bağlantı yok ise
        uyariSesi->play();
        QMessageBox msg(this);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Bu işlem için internete bağlı olmalısınız.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}

bool TopluStokYukleDialog::check()
{
    QTcpSocket *sock = new QTcpSocket(this);
    sock->connectToHost("www.google.com", 80);
    bool connected = sock->waitForConnected(30000);//ms

    if (!connected)
    {
        sock->abort();
        return false;
    }
    sock->close();
    return true;
}

void TopluStokYukleDialog::on_kontrolEderekcheckBox_clicked()
{
    if(ui->kontrolEderekcheckBox->isChecked()){
        ui->miktarcheckBox->setChecked(false);
        ui->fiyatcheckBox->setChecked(false);
        ui->AyarlargroupBox->setEnabled(false);
    }
    else{
        ui->miktarcheckBox->setChecked(true);
        ui->fiyatcheckBox->setChecked(true);
        ui->AyarlargroupBox->setEnabled(true);
    }
}

void TopluStokYukleDialog::setKullanici(const User &newKullanici)
{
    kullanici = newKullanici;
}

