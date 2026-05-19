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
    QString escapedUser = _UserName;
    escapedUser.replace("'", "''");
    query.exec(QString("SELECT username, password FROM kullanicilar WHERE username = '%1'").arg(escapedUser));
    if(query.next()){
        if(query.value(1).toString() == _Password){
            return true;
        }
    }
    return false;
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
    QString yedeklemeKomutu = "pg_dump -Fc -h localhost mhss_data > " + _dirNameAndFileName + ".sql";
    int exitCode = system(qPrintable(yedeklemeKomutu));
    return (exitCode == 0);
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
    yedekSorgu.exec("DROP DATABASE IF EXISTS mhss_data");
    if(!QString(yedekSorgu.lastError().text()).isEmpty()){
        qDebug() << yedekSorgu.lastError().text();
    }
    //veritabanını oluşturma
    QString dbOwner = db.userName().isEmpty() ? "postgres" : db.userName();
    yedekSorgu.exec(QString("CREATE DATABASE mhss_data OWNER %1").arg(dbOwner));
    if(!QString(yedekSorgu.lastError().text()).isEmpty()){
        qDebug() << yedekSorgu.lastError().text();
    }
    db.close();
    db.setDatabaseName("mhss_data");
    db.open();
    QString geriYuklemeKomutu = "pg_restore --no-owner -h localhost -d mhss_data < " + _dosyaYolu;
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
            QString userName = _user.getUserName();
            userName.replace("'", "''");
            query.exec(QString("UPDATE oturum SET username = '%1', giristarihi = '%2' WHERE id = 1")
                .arg(userName, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            if(query.lastError().isValid()){
                qDebug() << qPrintable(query.lastError().text());
            }
        }
    }
    else{
        QString userName = _user.getUserName();
        userName.replace("'", "''");
        query.exec(QString("INSERT INTO oturum(id, username, giristarihi) VALUES(1, '%1', '%2')")
            .arg(userName, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
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
    QString escapedUser = _UserName;
    escapedUser.replace("'", "''");
    query.exec(QString("SELECT * FROM kullanicilar WHERE username = '%1'").arg(escapedUser));
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
    QString pw = _NewUserInfos.getPassWord(); pw.replace("'", "''");
    QString ad = _NewUserInfos.getAd(); ad.replace("'", "''");
    QString soyad = _NewUserInfos.getSoyad(); soyad.replace("'", "''");
    QString cepno = _NewUserInfos.getCepNo(); cepno.replace("'", "''");
    query.exec(QString("UPDATE kullanicilar SET password = '%1', ad = '%2', soyad = '%3', cepno = '%4', tarih = '%5', kasayetki = %6, iadeyetki = %7, stokyetki = %8, ayaryetki = %9, cariyetki = %10 WHERE id = %11")
        .arg(pw, ad, soyad, cepno,
             _NewUserInfos.getTarih().toString("yyyy-MM-dd hh:mm:ss"),
             QString::number(_NewUserInfos.getKasaYetki()),
             QString::number(_NewUserInfos.getIadeYetki()),
             QString::number(_NewUserInfos.getStokYetki()),
             QString::number(_NewUserInfos.getAyaryetki()),
             QString::number(_NewUserInfos.getCariyetki()),
             _NewUserInfos.getUserID()));
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
        return false;
    }
    return true;
}

bool Veritabani::CreateNewUser(User _NewUser)
{
    QSqlQuery query(db);
    QString uname = _NewUser.getUserName(); uname.replace("'", "''");
    QString pw = _NewUser.getPassWord(); pw.replace("'", "''");
    QString ad = _NewUser.getAd(); ad.replace("'", "''");
    QString soyad = _NewUser.getSoyad(); soyad.replace("'", "''");
    QString cepno = _NewUser.getCepNo(); cepno.replace("'", "''");
    if(query.exec(QString("INSERT INTO kullanicilar(id, username, password, ad, soyad, cepno, tarih, kasayetki, iadeyetki, stokyetki, ayaryetki, cariyetki) VALUES(nextval('kullanicilar_sequence'), '%1', '%2', '%3', '%4', '%5', '%6', %7, %8, %9, %10, %11)")
        .arg(uname, pw, ad, soyad, cepno,
             _NewUser.getTarih().toString("yyyy-MM-dd hh:mm:ss"),
             QString::number(_NewUser.getKasaYetki()),
             QString::number(_NewUser.getIadeYetki()),
             QString::number(_NewUser.getStokYetki()),
             QString::number(_NewUser.getAyaryetki()),
             QString::number(_NewUser.getCariyetki())))){
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
    QString uname = _DeletedUserName;
    uname.replace("'", "''");
    if(query.exec(QString("DELETE FROM kullanicilar WHERE username = '%1'").arg(uname))){
        QMessageBox *msg = new QMessageBox(0);
        msg->setIcon(QMessageBox::Information);
        msg->setWindowTitle("Başarılı");
        msg->setText(_DeletedUserName + " kullanıcısı silindi.");
        msg->setStandardButtons(QMessageBox::Ok);
        msg->setDefaultButton(QMessageBox::Ok);
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
    query.exec(QString("SELECT ilce FROM ilceler WHERE ilid = %1").arg(_plaka));
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
    QString grup = pGrup;
    grup.replace("'", "''");
    query.exec(QString("SELECT id FROM stokgruplari WHERE grup = '%1'").arg(grup));
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
    QString marka = Marka;
    marka.replace("'", "''");
    // markanın id sini alma
    query.exec(QString("SELECT id FROM teraziler WHERE marka = '%1' ORDER BY marka ASC").arg(marka));
    query.next();
    int id = query.value(0).toInt();
    // markanın modellerini getirme
    query.exec(QString("SELECT model FROM terazimodel WHERE id = %1 ORDER BY model ASC").arg(id));
    while (query.next()) {
        modeller.append(query.value(0).toString());
    }
    return modeller;
}
