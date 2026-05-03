#include "kasayonetimi.h"

#include <QMessageBox>
#include <QDebug>

KasaYonetimi::KasaYonetimi()
{

}

KasaYonetimi::~KasaYonetimi()
{

}

double KasaYonetimi::getKasaToplamGiren(QDateTime baslangicTarih, QDateTime bitisTarih)
{
    QSqlQuery query(db);
    QString sql = QString("SELECT SUM(CAST(miktar AS DECIMAL)) FROM kasahareketleri WHERE islem = 'GİRİŞ' AND tarih BETWEEN '%1'::timestamp AND '%2'::timestamp")
                      .arg(baslangicTarih.toString(Qt::ISODate))
                      .arg(bitisTarih.toString(Qt::ISODate));
    query.exec(sql);
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
        return 0;
    }
    else{
        if(query.next()){
            return query.value(0).toDouble();
        }
        else{
            return 0;
        }
    }
}

double KasaYonetimi::getKasaToplamCikan(QDateTime baslangicTarih, QDateTime bitisTarih)
{
    QSqlQuery query(db);
    QString sql = QString("SELECT SUM(CAST(miktar AS DECIMAL)) FROM kasahareketleri WHERE islem IN ('ÇIKIŞ','İADE') AND tarih BETWEEN '%1'::timestamp AND '%2'::timestamp")
                      .arg(baslangicTarih.toString(Qt::ISODate))
                      .arg(bitisTarih.toString(Qt::ISODate));
    query.exec(sql);
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
        return 0;
    }
    else{
        if(query.next()){
            return query.value(0).toDouble();
        }
        else{
            return 0;
        }
    }
}

QSqlQueryModel *KasaYonetimi::getKasaHareketleri(QDateTime baslangicTarih, QDateTime bitisTarih)
{
    QString sql = QString("SELECT kasahareketleri.id, kasahareketleri.islem, CAST(miktar AS DECIMAL), kasahareketleri.tarih, kullanicilar.username, evrakno, kasahareketleri.aciklama FROM kasahareketleri "
                          "INNER JOIN kullanicilar ON kasahareketleri.kullanici = kullanicilar.id "
                          "WHERE kasahareketleri.tarih BETWEEN '%1'::timestamp AND '%2'::timestamp "
                          "ORDER BY kasahareketleri.id DESC")
                      .arg(baslangicTarih.toString(Qt::ISODate))
                      .arg(bitisTarih.toString(Qt::ISODate));
    kasaHareketlerimodel->clear();
    kasaHareketlerimodel->setQuery(sql, db);
    kasaHareketlerimodel->setHeaderData(0, Qt::Horizontal, "ID");
    kasaHareketlerimodel->setHeaderData(1, Qt::Horizontal, "Hareket");
    kasaHareketlerimodel->setHeaderData(2, Qt::Horizontal, "Miktar");
    kasaHareketlerimodel->setHeaderData(3, Qt::Horizontal, "Tarih");
    kasaHareketlerimodel->setHeaderData(4, Qt::Horizontal, "Kullanici");
    kasaHareketlerimodel->setHeaderData(5, Qt::Horizontal, "Evrak No");
    kasaHareketlerimodel->setHeaderData(6, Qt::Horizontal, "Açıklama");
    if(kasaHareketlerimodel->lastError().isValid()){
        qDebug() << qPrintable(kasaHareketlerimodel->lastError().text());
        return NULL;
    }
    else{
        return kasaHareketlerimodel;
    }
}

double KasaYonetimi::getNetKar(QDateTime baslangicTarih, QDateTime bitisTarih)
{
    QSqlQuery query(db);
    QString sql = QString("SELECT SUM(CAST(kar AS DECIMAL)) FROM kasahareketleri WHERE tarih BETWEEN '%1'::timestamp AND '%2'::timestamp")
                      .arg(baslangicTarih.toString(Qt::ISODate))
                      .arg(bitisTarih.toString(Qt::ISODate));
    query.exec(sql);
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
        return 0;
    }
    else{
        if(query.next()){
            return query.value(0).toDouble();
        }
        else{
            return 0;
        }
    }
}

int KasaYonetimi::KasaHareketiEkle(User user, KasaHareketi hareket, double tutar, QString aciklama, QDateTime tarih, QString evrakno, double netKar)
{
    double mevcutPara = 0;
    bool kasaOk = false;
    {
        QSqlQuery q(db);
        q.exec("select para FROM kasa");
        if(q.next()){
            mevcutPara = q.value(0).toDouble();
            kasaOk = true;
        }
    }
    double sonPara = 0;
    if(kasaOk){
        QString dtStr = tarih.toString(Qt::ISODate);
        QString hareketStr = enumToString(hareket);
        switch (hareket) {
        case KasaHareketi::Giris:
            sonPara = mevcutPara + tutar;
            {
                QSqlQuery q(db);
                QString sql = QString("UPDATE kasa SET para = %1 WHERE id = '1'")
                                  .arg(sonPara);
                q.exec(sql);
                if(q.lastError().isValid()){
                    qDebug() << "kasahareketiekle() hata:\n" << qPrintable(q.lastError().text());
                }
            }
            break;
        case KasaHareketi::Cikis:
            sonPara = mevcutPara - tutar;
            if(sonPara < 0){//KASADAKİ PARA EKSİ DEĞERE DÜŞMESİN
                QMessageBox msg(0);
                msg.setWindowTitle("Dikkat");
                msg.setIcon(QMessageBox::Warning);
                msg.setText("Kasada ki miktardan fazla çıkış yapamazsınız!");
                msg.setStandardButtons(QMessageBox::Ok);
                // msg.setButtonText(QMessageBox::Ok, "Tamam");
                msg.exec();
                return 0;
            }
            else{
                QSqlQuery q(db);
                QString sql = QString("UPDATE kasa SET para = %1 WHERE id = '1'")
                                  .arg(sonPara);
                q.exec(sql);
                if(q.lastError().isValid()){
                    qDebug() << "kasahareketiekle() hata:\n" << qPrintable(q.lastError().text());
                }
            }
            break;
        case KasaHareketi::Satis:
            break;
        case KasaHareketi::Iade:
            break;
        case KasaHareketi::BankaVirman:
            break;
        }
        //HAREKET EKLEME BAŞLANGICI
        {
            QSqlQuery q(db);
            QString sql = QString("INSERT INTO kasahareketleri (id, miktar, kullanici, islem, tarih, kar, evrakno, aciklama) "
                                  "VALUES (nextval('kasahareketleri_sequence'), %1, %2, '%3', '%4'::timestamp, %5, '%6', '%7')")
                              .arg(tutar)
                              .arg(user.getUserID().toInt())
                              .arg(hareketStr)
                              .arg(dtStr)
                              .arg(netKar)
                              .arg(evrakno.isEmpty() ? QString() : evrakno.replace("'", "''"))
                              .arg(aciklama.isEmpty() ? QString() : aciklama.replace("'", "''"));
            q.exec(sql);
            if(q.lastError().isValid()){
                qDebug() << qPrintable(q.lastError().text());
            }
        }
        QMessageBox msg(0);
        msg.setWindowTitle("Bilgi");
        msg.setIcon(QMessageBox::Information);
        if(hareket == KasaHareketi::Giris){// hareket giriş ise
            msg.setText("Kasaya " + QString::number(tutar, 'f', 2) + " TL giriş yapıldı.");
        }
        else if(hareket == KasaHareketi::Cikis){// hareket çıkış ise
            msg.setText("Kasadan " + QString::number(tutar, 'f', 2) + " TL çıkış yapıldı.");
        }
        msg.setStandardButtons(QMessageBox::Ok);
        // msg.setButtonText(QMessageBox::Ok, "Tamam");
        msg.exec();
        return 1;
    }
    return 0;
}

int KasaYonetimi::KasaHareketiEkle(User user, KasaHareketi hareket, double tutar, QString aciklama, QDateTime tarih, double netKar)
{
    double mevcutPara = 0;
    {
        QSqlQuery q(db);
        q.exec("select para FROM kasa");
        if(q.next()){
            mevcutPara = q.value(0).toDouble();
        }
    }
    double sonPara = 0;
    QString dtStr = tarih.toString(Qt::ISODate);
    QString hareketStr = enumToString(hareket);
    switch (hareket) {
    case KasaHareketi::Giris:
        sonPara = mevcutPara + tutar;
        {
            QSqlQuery q(db);
            QString sql = QString("UPDATE kasa SET para = %1 WHERE id = '1'")
                              .arg(sonPara);
            q.exec(sql);
            if(q.lastError().isValid()){
                qDebug() << "kasahareketiekle() hata:\n" << qPrintable(q.lastError().text());
            }
        }
        break;
    case KasaHareketi::Cikis:
        sonPara = mevcutPara - tutar;
        {
            QSqlQuery q(db);
            QString sql = QString("UPDATE kasa SET para = %1 WHERE id = '1'")
                              .arg(sonPara);
            q.exec(sql);
            if(q.lastError().isValid()){
                qDebug() << "kasahareketiekle() hata:\n" << qPrintable(q.lastError().text());
            }
        }
        break;
    case KasaHareketi::Satis:
        break;
    case KasaHareketi::Iade:
        break;
    case KasaHareketi::BankaVirman:
        break;
    }

    //HAREKET EKLEME BAŞLANGICI
    {
        double karDegeri = 0;
        switch (hareket) {
        case KasaHareketi::Giris:
            karDegeri = netKar;
            break;
        case KasaHareketi::Cikis:
            karDegeri = netKar * -1;
            break;
        default:
            break;
        }
        QSqlQuery q(db);
        QString sql = QString("INSERT INTO kasahareketleri (id, miktar, kullanici, islem, tarih, kar, aciklama) "
                              "VALUES (nextval('kasahareketleri_sequence'), %1, %2, '%3', '%4'::timestamp, %5, '%6')")
                          .arg(tutar)
                          .arg(user.getUserID().toInt())
                          .arg(hareketStr)
                          .arg(dtStr)
                          .arg(karDegeri)
                          .arg(aciklama.isEmpty() ? QString() : aciklama.replace("'", "''"));
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << qPrintable(q.lastError().text());
        }
    }
    return 1;
}

int KasaYonetimi::kasaHareketiDuzenle(User user, QString hareketID, KasaHareketi hareket, double tutar, QString aciklama, QDateTime tarih, QString evrakNo)
{
    double oncekiTutar = 0;
    KasaHareketi oncekiHareket = KasaHareketi::Giris;
    {
        QSqlQuery q(db);
        QString sql = QString("SELECT islem, CAST(miktar AS DECIMAL) FROM kasahareketleri WHERE id = %1")
                          .arg(hareketID);
        q.exec(sql);
        if(q.next()){
            oncekiTutar = q.value(1).toDouble();
            oncekiHareket = enumFromString(q.value(0).toString());
        }
    }
    //----------------------------------------
    //KASA HAREKETLERİ TABLOSUNU DÜZELTME
    {
        QSqlQuery q(db);
        QString sql = QString("UPDATE kasahareketleri "
                              "SET miktar = %1, kullanici = %2, islem = '%3', tarih = '%4'::timestamp, evrakno = '%5', aciklama = '%6' "
                              "WHERE id = %7")
                          .arg(tutar)
                          .arg(user.getUserID())
                          .arg(enumToString(hareket))
                          .arg(tarih.toString(Qt::ISODate))
                          .arg(evrakNo.isEmpty() ? QString() : evrakNo.replace("'", "''"))
                          .arg(QString("İŞLEM DÜZELTME:%1").arg(aciklama).replace("'", "''"))
                          .arg(hareketID);
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << qPrintable(q.lastError().text());
        }
    }
    // DÜZELTME YAPILACAK HAREKET DEĞİŞTİ İSE TUTARI KASAda ki paraya EKLE/ÇIKAR
    if(hareket != oncekiHareket){
        switch (oncekiHareket) {
            case KasaHareketi::Giris:{
                double suankiPara = getKasadakiPara();
                double guncelPara = suankiPara - oncekiTutar;
                {
                    QSqlQuery q(db);
                    QString sql = QString("UPDATE kasa SET para = %1 WHERE id = 1")
                                      .arg(guncelPara);
                    q.exec(sql);
                }
                guncelPara = getKasadakiPara();
                guncelPara -= tutar;
                {
                    QSqlQuery q(db);
                    QString sql = QString("UPDATE kasa SET para = %1 WHERE id = 1")
                                      .arg(guncelPara);
                    q.exec(sql);
                }
                break;
            }
            case KasaHareketi::Cikis:{
                double suankiPara = getKasadakiPara();
                double guncelPara = suankiPara + oncekiTutar;
                {
                    QSqlQuery q(db);
                    QString sql = QString("UPDATE kasa SET para = %1 WHERE id = 1")
                                      .arg(guncelPara);
                    q.exec(sql);
                }
                guncelPara = getKasadakiPara();
                guncelPara += tutar;
                {
                    QSqlQuery q(db);
                    QString sql = QString("UPDATE kasa SET para = %1 WHERE id = 1")
                                      .arg(guncelPara);
                    q.exec(sql);
                }
                break;
            }
            case KasaHareketi::Satis:{
                break;
            }
            case KasaHareketi::Iade:{
                break;
            }
            case KasaHareketi::BankaVirman:{
                break;
            }
        }
    }
    return 1;
}

bool KasaYonetimi::kasaHareketiSil(QString hareketID, QString hareket, double tutar)
{
    // kasahareketleri tablosundan kayıdı silme
    {
        QSqlQuery q(db);
        QString sql = QString("DELETE FROM kasahareketleri WHERE id = %1")
                          .arg(hareketID);
        q.exec(sql);
    }
    //kasadan silinen işlemin parasını düşme.
    if(hareket == "GİRİŞ"){
        double suankiPara = getKasadakiPara();
        double guncelPara = suankiPara - tutar;
        QSqlQuery q(db);
        QString sql = QString("UPDATE kasa SET para = %1 WHERE id = 1")
                          .arg(guncelPara);
        q.exec(sql);
    }
    else if(hareket == "ÇIKIŞ"){
        double suankiPara = getKasadakiPara();
        double guncelPara = suankiPara + tutar;
        QSqlQuery q(db);
        QString sql = QString("UPDATE kasa SET para = %1 WHERE id = 1")
                          .arg(guncelPara);
        q.exec(sql);
    }
    return true;
}

void KasaYonetimi::kasadanParaCek(double cekilecekTutar, User kullanici)
{
    // kasada ki parayı güncelleme
    double kasadaKalanPara = getKasadakiPara() - cekilecekTutar;
    {
        QSqlQuery q(db);
        QString sql = QString("UPDATE kasa SET para = %1")
                          .arg(kasadaKalanPara);
        q.exec(sql);
        if(q.lastError().isValid()){
            qWarning() << "kasadanParaCek UPDATE hatası:\n" << qPrintable(q.lastError().text());
        }
    }
    //kasa hareketlerini girme
    {
        QSqlQuery q(db);
        QString dtStr = QDateTime::currentDateTime().toString(Qt::ISODate);
        QString sql = QString("INSERT INTO kasahareketleri(id, miktar, kullanici, islem, tarih) VALUES (nextval('kasahareketleri_sequence'), %1, %2, 'ÇIKIŞ', '%3'::timestamp)")
                          .arg(cekilecekTutar)
                          .arg(kullanici.getUserID())
                          .arg(dtStr);
        q.exec(sql);
        if(q.lastError().isValid()){
            qDebug() << "kasadanParaCek kasa hareketleri hatası:\n" << qPrintable(q.lastError().text());
        }
    }
}

double KasaYonetimi::getKasadakiPara()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM kasa");
    query.next();
    if(query.lastError().isValid()){
        return 0;
    }
    return query.value(1).toDouble();
}

double KasaYonetimi::getGunlukCiro()
{
    QSqlQuery query(db);
    QString sql = QString("SELECT SUM(miktar) FROM kasahareketleri WHERE tarih > '%1'::timestamp AND islem IN('GİRİŞ','İADE')")
                      .arg(QDateTime::currentDateTime().date().toString(Qt::ISODate));
    query.exec(sql);
    query.next();
    if(query.lastError().isValid()){
        qDebug() << qPrintable(query.lastError().text());
        return 0;
    }
    return query.value(0).toDouble();
}

QString KasaYonetimi::enumToString(KasaYonetimi::KasaHareketi value)
{
    switch (value) {
    case KasaYonetimi::KasaHareketi::Giris:
        return QString("GİRİŞ");
        break;
    case KasaYonetimi::KasaHareketi::Cikis:
        return QString("ÇIKIŞ");
        break;
    case KasaYonetimi::KasaHareketi::Satis:
        return QString("SATIŞ");
        break;
    case KasaYonetimi::KasaHareketi::Iade:
        return QString("İADE");
        break;
    default:
        return QString();
        break;
    }
}

KasaYonetimi::KasaHareketi KasaYonetimi::enumFromString(QString value)
{
    if(value == "Giris"){
        return KasaHareketi::Giris;
    }
    else if(value == "Cikis"){
        return KasaHareketi::Cikis;
    }
    else if(value == "Satis"){
        return KasaHareketi::Satis;
    }
    else if(value == "Iade"){
        return KasaHareketi::Iade;
    }
    else if(value == "BankaVirman"){
        return KasaHareketi::BankaVirman;
    }
    return KasaHareketi::Giris;
}
