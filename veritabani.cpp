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
    QSqlQuery query(db);
    query.prepare("SELECT username, password FROM kullanicilar WHERE username = ?");
    query.bindValue(0, _UserName);
    query.exec();
    if(query.next()){
        if(query.value(0) == _Password){
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
    QSqlQuery query(db);
    QStringList islemler;
    query.exec("SELECT fatura_no, tarih FROM faturalar WHERE tarih::date = now()::date AND tipi = 2 ORDER BY tarih DESC");
    while(query.next()){
        islemler.append(query.value(0).toString() + " " + query.value(1).toTime().toString("hh:mm:ss"));
    }
    return islemler;
}

bool Veritabani::veritabaniYedekle(QString _dirNameAndFileName)
{
    QSqlQuery query(db);
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
    QSqlQuery query(db);
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
    QSqlQuery query(db);
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
    QSqlQuery query(db);
    QSettings hizliButonBarkodlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/hizlibutonlar.ini", QSettings::IniFormat);
    for (QString buttonName : hizliButonBarkodlar.childGroups()) {
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
    QSqlQuery query(db);
    query.exec("SELECT * FROM oturum");
    if(query.next()){
        if(query.value(1).toString() != _user.getUserName()){
            query.prepare("UPDATE oturum SET username = ?, giristarihi = ? WHERE id = 1");
            query.bindValue(0, _user.getUserName());
            query.bindValue(1, QDateTime::currentDateTime());
            query.exec();
            if(query.lastError().isValid()){
                qDebug() << qPrintable(query.lastError().text());
            }
        }
    }
    else{
        query.prepare("INSERT INTO oturum(id, username, giristarihi) VALUES('1', ?,?)");
        query.bindValue(0, _user.getUserName());
        query.bindValue(1, QDateTime::currentDateTime());
        query.exec();
        if(query.lastError().isValid()){
            qDebug() << qPrintable(query.lastError().text());
        }
    }
}

QSqlQuery Veritabani::getOturum()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM oturum");
    if(!query.next()){
        return QSqlQuery();
    }
    return query;
}

void Veritabani::oturumSonlandir()
{
    QSqlQuery query(db);
    query.exec("DELETE FROM oturum WHERE id = 1");
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
}

bool Veritabani::veritabaniVarmi()
{
    QSqlQuery query(db);
    //mhss_data veritabanı varmı kontrol
    query.exec("SELECT datname FROM pg_database WHERE datname = 'mhss_data'");
    if(query.next()){
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
    QSqlQuery query(db);
    User u;
    query.prepare("SELECT * FROM kullanicilar WHERE username = ?");
    query.bindValue(0, _UserName);
    query.exec();
    if(query.next()){
        u.setUserID(query.value(0).toString());
        u.setUserName(query.value(1).toString());
        u.setPassWord(query.value(2).toString());
        u.setAd(query.value(3).toString());
        u.setSoyad(query.value(4).toString());
        u.setCepNo(query.value(5).toString());
        u.setTarih(query.value(6).toDateTime());
        u.setKasaYetki(query.value(7).toBool());
        u.setIadeYetki(query.value(8).toBool());
        u.setStokYetki(query.value(9).toBool());
        u.setAyaryetki(query.value(10).toBool());
        u.setCariyetki(query.value(11).toBool());
    }
    return u;
}

bool Veritabani::updateUser(User _NewUserInfos)
{
    QSqlQuery query(db);
    query.prepare("UPDATE kullanicilar "
                    "SET password = ?, ad = ?, soyad = ?, cepno = ?, tarih = ?, kasayetki = ?, iadeyetki = ?, stokyetki = ?, ayaryetki = ?, cariyetki = ? "
                    "WHERE id = ?");
    query.bindValue(0, _NewUserInfos.getPassWord());
    query.bindValue(1, _NewUserInfos.getAd());
    query.bindValue(2, _NewUserInfos.getSoyad());
    query.bindValue(3, _NewUserInfos.getCepNo());
    query.bindValue(4, _NewUserInfos.getTarih());
    query.bindValue(5, _NewUserInfos.getKasaYetki());
    query.bindValue(6, _NewUserInfos.getIadeYetki());
    query.bindValue(7, _NewUserInfos.getStokYetki());
    query.bindValue(8, _NewUserInfos.getAyaryetki());
    query.bindValue(9, _NewUserInfos.getCariyetki());
    query.bindValue(10, _NewUserInfos.getUserID());
    query.exec();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
        return false;
    }
    return true;
}

bool Veritabani::CreateNewUser(User _NewUser)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO kullanicilar(id, username, password, ad, soyad, cepno, tarih, kasayetki, iadeyetki, stokyetki, ayaryetki, cariyetki)"
                    " VALUES(nextval('kullanicilar_sequence'), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bindValue(0, _NewUser.getUserName());
    query.bindValue(1, _NewUser.getPassWord());
    query.bindValue(2, _NewUser.getAd());
    query.bindValue(3, _NewUser.getSoyad());
    query.bindValue(4, _NewUser.getCepNo());
    query.bindValue(5, _NewUser.getTarih());
    query.bindValue(6, _NewUser.getKasaYetki());
    query.bindValue(7, _NewUser.getIadeYetki());
    query.bindValue(8, _NewUser.getStokYetki());
    query.bindValue(9, _NewUser.getAyaryetki());
    query.bindValue(10, _NewUser.getCariyetki());
    if(query.exec()){
        return true;
    }
    else{
        qDebug() << qPrintable(query.lastError().text());
    }
    return false;
}

void Veritabani::deleteUser(QString _DeletedUserName)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM kullanicilar WHERE username = ?");
    query.bindValue(0, _DeletedUserName);
    if(query.exec()){
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
        if(query.lastError().isValid()){
            QMessageBox *msg = new QMessageBox(0);
            msg->setIcon(QMessageBox::Information);
            msg->setWindowTitle("Hata");
            msg->setText("Kullanıcı silinemedi!");
            msg->setInformativeText(query.lastError().text());
            msg->setStandardButtons(QMessageBox::Ok);
            msg->setDefaultButton(QMessageBox::Ok);
            // msg->setButtonText(QMessageBox::Ok, "Tamam");
            msg->exec();
        }
    }
}

QStringList Veritabani::getOdemeTipleri()
{
    QSqlQuery query(db);
    QStringList odemeTipleri;
    query.exec("SELECT * FROM odemetipleri");
    while (query.next()) {
        odemeTipleri.append(query.value(1).toString());
    }
    return odemeTipleri;
}

QStringList Veritabani::getIller()
{
    QSqlQuery query(db);
    QStringList ilList;
    ilList.append("İl seçiniz.");
    query.exec("SELECT il FROM iller");
    while (query.next()) {
        ilList.append(query.value(0).toString());
    }
    return ilList;
}

QStringList Veritabani::getIlceler(int _plaka)
{
    QSqlQuery query(db);
    QStringList ilcelerList;
    ilcelerList.append("İlçe seçiniz.");
    query.prepare("SELECT ilce FROM ilceler WHERE ilid = ?");
    query.bindValue(0, _plaka);
    query.exec();
    while (query.next()) {
        ilcelerList.append(query.value(0).toString());
    }
    return ilcelerList;
}

QStringList Veritabani::GetUsers()
{
    QSqlQuery query(db);
    QStringList users;
    query.exec("SELECT username FROM kullanicilar");
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
    }
    while (query.next()) {
        users << query.value(0).toString();
    }
    return users;
}


QStringList Veritabani::stokGruplariGetir()
{
    QSqlQuery query(db);
    query.exec("SELECT grup FROM stokgruplari");
    QStringList liste;
    while (query.next()) {
        liste.append(query.value(0).toString());
    }
    return liste;
}

int Veritabani::getGrupID(QString pGrup)
{
    QSqlQuery query(db);
    query.prepare("SELECT id FROM stokgruplari WHERE grup = ?");
    query.bindValue(0, pGrup);
    query.exec();
    if(query.next()){
        return query.value(0).toInt();
    }
    return 0;
}

QStringList Veritabani::getTeraziler()
{
    QSqlQuery query(db);
    QStringList teraziler;
    query.exec("SELECT * FROM teraziler");
    while (query.next()) {
        teraziler.append(query.value(1).toString());
    }
    return teraziler;
}

QStringList Veritabani::getTeraziModeller(QString Marka)
{
    QSqlQuery query(db);
    QStringList modeller;
    // markanın id sini alma
    query.prepare("SELECT id FROM teraziler WHERE marka = ? ORDER BY marka ASC");
    query.bindValue(0, Marka);
    query.exec();
    query.next();
    int id = query.value(0).toInt();
    // markanın modellerini getirme
    query.prepare("SELECT model FROM terazimodel WHERE id = ? ORDER BY model ASC");
    query.bindValue(0, id);
    query.exec();
    while (query.next()) {
        modeller.append(query.value(0).toString());
    }
    return modeller;
}
