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
#include "veritabani.h"
#include "user.h"
#include "stokkarti.h"
//**************************
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVariant>
#include <QDateTime>
#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QHash>

Veritabani::Veritabani()
{

}

Veritabani::~Veritabani()
{

}

bool Veritabani::loginControl(QString _UserName, QString _Password)
{
    sorgu.prepare("SELECT username, password FROM kullanicilar WHERE username = ?");
    sorgu.bindValue(0, _UserName);
    sorgu.exec();
    if(sorgu.next()){
        if(sorgu.value(0) == _Password){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

QStringList Veritabani::getSonIslemler()
{
    QStringList islemler;
    sorgu.exec("SELECT fatura_no, tarih FROM faturalar WHERE tarih::date = now()::date AND tipi = 2 ORDER BY tarih DESC");
    while(sorgu.next()){
        islemler.append(sorgu.value(0).toString() + " " + sorgu.value(1).toTime().toString("hh:mm:ss"));
    }
    return islemler;
}

bool Veritabani::veritabaniYedekle(QString _dirNameAndFileName)
{
    QString yedeklemeKomutu = "pg_dump -Fc -U postgres mhss_data > ";
    yedeklemeKomutu += _dirNameAndFileName + ".sql";
    int exitCode = system(qPrintable(yedeklemeKomutu));
    if(exitCode == QProcess::NormalExit && exitCode == QProcess::NormalExit){// system() ile gönderdiğim komut normal olarak bittiyse
        // komut başarılı
        return true;
    } else {
        // komut başarısız.
        return false;
    }
}

bool Veritabani::veritabaniYedektenGeriYukle(QString _dosyaYolu)
{
    db.close();
    db.setDatabaseName("postgres");
    //veritabanını silme
    db.open();

    // başka bir yerde bağlantı açıksa veya açık kaldıysa mhss_data veritabanına açık tüm bağlantıları sonlandırma.
    QSqlQuery vtTumBaglantiKesmeSorgu = QSqlQuery(db);
    vtTumBaglantiKesmeSorgu.exec("SELECT pg_terminate_backend(pg_stat_activity.pid) "
                                 "FROM pg_stat_activity "
                                 "WHERE pg_stat_activity.datname = 'mhss_data'"
                                 "AND pid <> pg_backend_pid()");
    qDebug() << qPrintable(vtTumBaglantiKesmeSorgu.lastError().text());

    QSqlQuery yedekSorgu = QSqlQuery(db);
    yedekSorgu.exec("DROP DATABASE mhss_data");
    if(!QString(yedekSorgu.lastError().text()).isEmpty()){
        qDebug() << yedekSorgu.lastError().text();
    }
    //veritabanını oluşturma
    yedekSorgu.exec("CREATE DATABASE mhss_data OWNER postgres");
    if(!QString(yedekSorgu.lastError().text()).isEmpty()){
        qDebug() << yedekSorgu.lastError().text();
    }
    db.close();
    db.setDatabaseName("mhss_data");
    db.open();
    QString geriYuklemeKomutu = "pg_restore -U postgres -d mhss_data < " + _dosyaYolu;
    int exitCode = system(qPrintable(geriYuklemeKomutu));
    QFile(_dosyaYolu).remove();
    if(exitCode == QProcess::NormalExit){// system() ile gönderdiğim komut süreci normal olarak bittiyse
        // komut başarılı
        return true;
    } else {
        // komut başarısız.
        return false;
    }
}

bool Veritabani::veritabaniSifirla()
{
    // başka bir yerde bağlantı açıksa veya açık kaldıysa mhss_data veritabanına açık tüm bağlantıları sonlandırma.
    QSqlQuery vtTumBaglantiKesmeSorgu = QSqlQuery(db);
    vtTumBaglantiKesmeSorgu.exec("SELECT pg_terminate_backend(pg_stat_activity.pid) "
                                 "FROM pg_stat_activity "
                                 "WHERE pg_stat_activity.datname = 'mhss_data'"
                                 "AND pid <> pg_backend_pid()");
    qDebug() << qPrintable(vtTumBaglantiKesmeSorgu.lastError().text());

    // veritabani sıfırlandığı için hizli butonları da sıfırla
    QSettings hizlibutonlariINI(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/hizlibutonlar.ini", QSettings::IniFormat);
    hizlibutonlariINI.clear();
    QFile mhss_data_sifir(":/dosyalar/dosyalar/mhss_data.sql");
    if(!QFileInfo().exists("/tmp/mhss_data.sql")){
        mhss_data_sifir.copy("/tmp/mhss_data.sql");
    }
    QFile dosya("/tmp/mhss_data.sql");
//    qDebug() << QFileInfo(dosya).absoluteFilePath();
    if(veritabaniYedektenGeriYukle(QFileInfo(dosya).absoluteFilePath())){
        return true;
    }
    else{
        return false;
    }
}

void Veritabani::setHizliButon(StokKarti _stokKarti)
{
    QSettings hizliButonBarkodlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/hizlibutonlar.ini", QSettings::IniFormat);
    foreach (QString buttonName, hizliButonBarkodlar.childGroups()) {
        hizliButonBarkodlar.beginGroup(buttonName);
        QString duzenlenecekStokBarkod = hizliButonBarkodlar.value("barkod").toString();
        if(duzenlenecekStokBarkod == _stokKarti.getBarkod()){
            hizliButonBarkodlar.setValue("barkod", _stokKarti.getBarkod());
            hizliButonBarkodlar.setValue("ad", _stokKarti.getAd());
            hizliButonBarkodlar.endGroup();
            return;
        }
        hizliButonBarkodlar.endGroup();
    }
}

void Veritabani::setOturum(User _user)
{
    sorgu.exec("SELECT * FROM oturum");
    if(sorgu.next()){
        if(sorgu.value(1).toString() != _user.getUserName()){
            sorgu.prepare("UPDATE oturum SET username = ?, giristarihi = ? WHERE id = 1");
            sorgu.bindValue(0, _user.getUserName());
            sorgu.bindValue(1, QDateTime::currentDateTime());
            sorgu.exec();
            if(sorgu.lastError().isValid()){
                qDebug() << qPrintable(sorgu.lastError().text());
            }
        }
    }
    else{
        sorgu.prepare("INSERT INTO oturum(id, username, giristarihi) VALUES('1', ?,?)");
        sorgu.bindValue(0, _user.getUserName());
        sorgu.bindValue(1, QDateTime::currentDateTime());
        sorgu.exec();
        if(sorgu.lastError().isValid()){
            qDebug() << qPrintable(sorgu.lastError().text());
        }
    }
}

QSqlQuery Veritabani::getOturum()
{
    sorgu.exec("SELECT * FROM oturum");
    if(!sorgu.next()){
        return QSqlQuery();
    }
    return sorgu;
}

void Veritabani::oturumSonlandir()
{
    sorgu.exec("DELETE FROM oturum WHERE id = 1");
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
}

bool Veritabani::veritabaniVarmi()
{
    //mhss_data veritabanı varmı kontrol
    sorgu.exec("SELECT datname FROM pg_database WHERE datname = 'mhss_data'");
    if(sorgu.next()){
        db.setDatabaseName("mhss_data");
        db.open();
        return true;
    }
    else{
        return false;
    }
}


User Veritabani::GetUserInfos(QString _UserName)
{
    User u;
    sorgu.prepare("SELECT * FROM kullanicilar WHERE username = ?");
    sorgu.bindValue(0, _UserName);
    sorgu.exec();
    if(sorgu.next()){
        u.setUserID(sorgu.value(0).toString());
        u.setUserName(sorgu.value(1).toString());
        u.setPassWord(sorgu.value(2).toString());
        u.setAd(sorgu.value(3).toString());
        u.setSoyad(sorgu.value(4).toString());
        u.setCepNo(sorgu.value(5).toString());
        u.setTarih(sorgu.value(6).toDateTime());
        u.setKasaYetki(sorgu.value(7).toBool());
        u.setIadeYetki(sorgu.value(8).toBool());
        u.setStokYetki(sorgu.value(9).toBool());
        u.setAyaryetki(sorgu.value(10).toBool());
        u.setCariyetki(sorgu.value(11).toBool());
    }
    return u;
}

bool Veritabani::updateUser(User _NewUserInfos)
{
    sorgu.prepare("UPDATE kullanicilar "
                    "SET password = ?, ad = ?, soyad = ?, cepno = ?, tarih = ?, kasayetki = ?, iadeyetki = ?, stokyetki = ?, ayaryetki = ?, cariyetki = ? "
                    "WHERE id = ?");
    sorgu.bindValue(0, _NewUserInfos.getPassWord());
    sorgu.bindValue(1, _NewUserInfos.getAd());
    sorgu.bindValue(2, _NewUserInfos.getSoyad());
    sorgu.bindValue(3, _NewUserInfos.getCepNo());
    sorgu.bindValue(4, _NewUserInfos.getTarih());
    sorgu.bindValue(5, _NewUserInfos.getKasaYetki());
    sorgu.bindValue(6, _NewUserInfos.getIadeYetki());
    sorgu.bindValue(7, _NewUserInfos.getStokYetki());
    sorgu.bindValue(8, _NewUserInfos.getAyaryetki());
    sorgu.bindValue(9, _NewUserInfos.getCariyetki());
    sorgu.bindValue(10, _NewUserInfos.getUserID());
    sorgu.exec();
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
        return false;
    }
    return true;
}

bool Veritabani::CreateNewUser(User _NewUser)
{
    sorgu.prepare("INSERT INTO kullanicilar(id, username, password, ad, soyad, cepno, tarih, kasayetki, iadeyetki, stokyetki, ayaryetki, cariyetki)"
                    " VALUES(nextval('kullanicilar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    sorgu.bindValue(0, _NewUser.getUserName());
    sorgu.bindValue(1, _NewUser.getPassWord());
    sorgu.bindValue(2, _NewUser.getAd());
    sorgu.bindValue(3, _NewUser.getSoyad());
    sorgu.bindValue(4, _NewUser.getCepNo());
    sorgu.bindValue(5, _NewUser.getTarih());
    sorgu.bindValue(6, _NewUser.getKasaYetki());
    sorgu.bindValue(7, _NewUser.getIadeYetki());
    sorgu.bindValue(8, _NewUser.getStokYetki());
    sorgu.bindValue(9, _NewUser.getAyaryetki());
    sorgu.bindValue(10, _NewUser.getCariyetki());
    if(sorgu.exec()){
        return true;
    }
    else{
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    return false;
}

void Veritabani::deleteUser(QString _DeletedUserName)
{
    sorgu.prepare("DELETE FROM kullanicilar WHERE username = ?");
    sorgu.bindValue(0, _DeletedUserName);
    if(sorgu.exec()){
        QMessageBox *msg = new QMessageBox(0);
        msg->setIcon(QMessageBox::Information);
        msg->setWindowTitle("Başarılı");
        msg->setText(_DeletedUserName + " kullanıcısı silindi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
        // msg->setButtonText(QMessageBox::Ok, "Tamam");
        msg->exec();
    }
    else{
        if(sorgu.lastError().isValid()){
            QMessageBox *msg = new QMessageBox(0);
            msg->setIcon(QMessageBox::Information);
            msg->setWindowTitle("Hata");
            msg->setText("Kullanıcı silinemedi!");
            msg->setInformativeText(sorgu.lastError().text());
            msg->setStandardButtons(QMessageBox::Ok);
            msg->setDefaultButton(QMessageBox::Ok);
            // msg->setButtonText(QMessageBox::Ok, "Tamam");
            msg->exec();
        }
    }
}

QStringList Veritabani::getOdemeTipleri()
{
    QStringList odemeTipleri;
    sorgu.exec("SELECT * FROM odemetipleri");
    while (sorgu.next()) {
        odemeTipleri.append(sorgu.value(1).toString());
    }
    return odemeTipleri;
}

QStringList Veritabani::getIller()
{
    QStringList ilList;
    ilList.append("İl seçiniz.");
    sorgu.exec("SELECT il FROM iller");
    while (sorgu.next()) {
        ilList.append(sorgu.value(0).toString());
    }
    return ilList;
}

QStringList Veritabani::getIlceler(int _plaka)
{
    QStringList ilcelerList;
    ilcelerList.append("İlçe seçiniz.");
    sorgu.prepare("SELECT ilce FROM ilceler WHERE ilid = ?");
    sorgu.bindValue(0, _plaka);
    sorgu.exec();
    while (sorgu.next()) {
        ilcelerList.append(sorgu.value(0).toString());
    }
    return ilcelerList;
}

QStringList Veritabani::GetUsers()
{
    QStringList users;
    sorgu.exec("SELECT username FROM kullanicilar");
    if(sorgu.lastError().isValid()){
        qDebug() << qPrintable(sorgu.lastError().text());
    }
    while (sorgu.next()) {
        users << sorgu.value(0).toString();
    }
    return users;
}


QStringList Veritabani::stokGruplariGetir()
{
    sorgu.exec("SELECT grup FROM stokgruplari");
    QStringList liste;
    while (sorgu.next()) {
        liste.append(sorgu.value(0).toString());
    }
    return liste;
}

int Veritabani::getGrupID(QString pGrup)
{
    sorgu.prepare("SELECT id FROM stokgruplari WHERE grup = ?");
    sorgu.bindValue(0, pGrup);
    sorgu.exec();
    if(sorgu.next()){
        return sorgu.value(0).toInt();
    }
    return 0;
}

QStringList Veritabani::getTeraziler()
{
    QStringList teraziler;
    sorgu.exec("SELECT * FROM teraziler");
    while (sorgu.next()) {
        teraziler.append(sorgu.value(1).toString());
    }
    return teraziler;
}

QStringList Veritabani::getTeraziModeller(QString Marka)
{
    QStringList modeller;
    // markanın id sini alma
    sorgu.prepare("SELECT id FROM teraziler WHERE marka = ? ORDER BY marka ASC");
    sorgu.bindValue(0, Marka);
    sorgu.exec();
    sorgu.next();
    int id = sorgu.value(0).toInt();
    // markanın modellerini getirme
    sorgu.prepare("SELECT model FROM terazimodel WHERE id = ? ORDER BY model ASC");
    sorgu.bindValue(0, id);
    sorgu.exec();
    while (sorgu.next()) {
        modeller.append(sorgu.value(0).toString());
    }
    return modeller;
}
