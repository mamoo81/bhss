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
#ifndef CARIHAREKETIEKLEFORM_H
#define CARIHAREKETIEKLEFORM_H
#include "veritabani.h"
#include "cariyonetimi.h"
//****************************
#include <QDialog>

namespace Ui {
class CariHareketiEkleForm;
}

class CariHareketiEkleForm : public QDialog
{
    Q_OBJECT

public:
    explicit CariHareketiEkleForm(QWidget *parent = nullptr);
    ~CariHareketiEkleForm();

    enum FaturaTipi{
        alis = 1,
        satis = 2,
        iade = 3,
        odeme = 4,
        tahsilat = 5
    };
    Q_ENUM(FaturaTipi)
    enum OdemeTipi{
        nakit = 1,
        kredikarti = 2,
        havale = 3,
        cek = 4,
        senet = 5
    };
    Q_ENUM(OdemeTipi)
    Veritabani *vt = new Veritabani();
    CariYonetimi cariYonetimi = CariYonetimi();
    QString cariID;
    Cari cariKart;
    FaturaTipi faturaTip;
    OdemeTipi odemeTip;


    void setCariID(const QString &newCariID);

    void setFaturaTip(FaturaTipi newFaturaTip);

    void setKullanici(const User &newKullanici);

private slots:
    void FormLoad();
    void on_KaydetpushButton_clicked();

    void on_YetkililineEdit_textChanged(const QString &arg1);

    void on_iptalpushButton_clicked();

    void on_OdemeTipcomboBox_currentIndexChanged(int index);

private:
    User kullanici;

    Ui::CariHareketiEkleForm *ui;
};

#endif // CARIHAREKETIEKLEFORM_H
