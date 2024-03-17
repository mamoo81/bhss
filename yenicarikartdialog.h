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
#ifndef YENICARIKARTDIALOG_H
#define YENICARIKARTDIALOG_H
#include "veritabani.h"
#include "cariyonetimi.h"
//**************************
#include <QDialog>
#include <QRegExp>
#include <QPalette>
#include <QSoundEffect>

namespace Ui {
class YeniCariKartDialog;
}

class YeniCariKartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit YeniCariKartDialog(QWidget *parent = nullptr);
    ~YeniCariKartDialog();

    QSoundEffect uyariSesi;

    Veritabani *vt = new Veritabani();
    CariYonetimi cariYonetimi = CariYonetimi();

    void setDuzenle(bool newDuzenle);

    void setDuzenlenecekCariID(const QString &newDuzenlenecekCariID);

private slots:
    void FormLoad();
    void on_iptalpushButton_clicked();

    void on_ilcomboBox_currentIndexChanged(int index);

    void on_KaydetpushButton_clicked();

    void on_CariAdilineEdit_textChanged(const QString &arg1);

    void on_YetkililineEdit_textChanged(const QString &arg1);

    void on_VergiNolineEdit_textChanged(const QString &arg1);

    void on_MaillineEdit_textChanged(const QString &arg1);

    void on_TelefonlineEdit_textChanged(const QString &arg1);

    void on_ilcecomboBox_currentIndexChanged(int index);

private:
    Ui::YeniCariKartDialog *ui;

    bool duzenle;
    QString duzenlenecekCariID;
    Cari duzenlenecekCariKart;

    QPalette LineEditBackColorPaletteRed;
    QPalette LineEditBackColorPaletteDefault;

    QRegExp RegEXPcariAdi;
    QRegExp RegEXPVergiNo;
    QRegExp RegEXPtelefon;
    QRegExp RegEXPmail;
};

#endif // YENICARIKARTDIALOG_H
