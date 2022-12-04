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
#ifndef VERITABANI_H
#define VERITABANI_H

#include "user.h"
#include "cari.h"
#include "stokkarti.h"
#include "sepet.h"
//************************
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QList>
#include <QListWidgetItem>

class Veritabani
{
public:
    Veritabani();
    ~Veritabani();

    QSqlDatabase db = QSqlDatabase::database("mhss_data");
    QSqlQuery sorgu = QSqlQuery(db);
//    QSqlQueryModel *stokKartlariModel = new QSqlQueryModel();
//    QSqlQueryModel *stokHareketleriModel = new QSqlQueryModel();
//    QSqlQueryModel *cariKartIsımleriModel = new QSqlQueryModel();
//    QSqlQueryModel *cariHareketleriModel = new QSqlQueryModel();
//    QSqlQueryModel *kasaHareketlerimodel = new QSqlQueryModel();
//    QStringList stokbirimleri;

    QStringList GetUsers();
private:
    Sepet satilacakSepet;

public slots:
    User GetUserInfos(QString _UserName);
    bool loginControl(QString _UserName, QString _Password);
    QStringList getSonIslemler();
    bool veritabaniVarmi();
    QStringList stokGruplariGetir();
    int getGrupID(QString pGrup);
    bool CreateNewUser(User _NewUser);
    bool updateUser(User _NewUserInfos);
    void deleteUser(QString _DeletedUserName);
    QStringList getIller();
    QStringList getIlceler(int _plaka);
    QStringList getOdemeTipleri();
    bool veritabaniYedekle(QString _dirNameAndFileName);
    bool veritabaniYedektenGeriYukle(QString _dosyaYolu);
    bool veritabaniSifirla();
    void setHizliButon(StokKarti _stokKarti);
    void setOturum(User _user);
    QSqlQuery getOturum();
    void oturumSonlandir();
    QStringList getTeraziler();
    QStringList getTeraziModeller(QString Marka);

};

#endif // VERITABANI_H
