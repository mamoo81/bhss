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
#ifndef AYARLARDIALOG_H
#define AYARLARDIALOG_H
#include "user.h"
#include "veritabani.h"
//****************************
#include <QDialog>
#include <QFile>
#include <QSoundEffect>

namespace Ui {
class AyarlarDialog;
}

class AyarlarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AyarlarDialog(QWidget *parent = nullptr);
    ~AyarlarDialog();

    Veritabani vt = Veritabani();

    QSoundEffect uyariSesi;

    void setCurrentUser(const User &newCurrentUser);

    QString periyod = NULL;
    QString jobSaati = NULL;
    QString jobDakika = NULL;

private slots:
    void on_YeniPushButton_clicked();

    void formLoad();
    void on_KullanicilarlistWidget_itemSelectionChanged();

    void on_DuzenlePushButton_clicked();

    void on_SilPushButton_clicked();

    void getUsers();
    void on_pushButton_clicked();

    void on_YedeklepushButton_clicked();

    void on_GeriYuklepushButton_clicked();

    void on_KonuSecpushButton_clicked();

    void on_OtomatikYedekcheckBox_clicked();

    void cronJobKaydet();

    void cronJobSil();

    void on_SiklikcomboBox_currentIndexChanged(const QString &arg1);

    void on_SaatcomboBox_currentIndexChanged(int index);

    void on_dakikacomboBox_currentIndexChanged(int index);

    void on_SifirlapushButton_clicked();

    void VtYedeklemeButonlariniAyarla();
    void on_kapatpushButton_clicked();

    void on_hizliButtonSifirlapushButton_clicked();

    void on_TeraziMarkacomboBox_currentIndexChanged(int index);

    void SeriPortComboBoxDoldur();

    void on_SekmeVarsayilanpushButton_clicked();

    void on_StokTakibicheckBox_stateChanged(int arg1);

    void on_YonelimcomboBox_currentIndexChanged(int index);

private:
    Ui::AyarlarDialog *ui;

    QFile *cronMevcutDosya = new QFile();
    User currentUser;
};

#endif // AYARLARDIALOG_H
