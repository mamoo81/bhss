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
#ifndef CARIKARTLARDIALOG_H
#define CARIKARTLARDIALOG_H
#include "veritabani.h"
#include "cari.h"
#include "yazici.h"
#include "cariyonetimi.h"
//**************************
#include <QDialog>
#include <QSound>

namespace Ui {
class CariKartlarDialog;
}

class CariKartlarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CariKartlarDialog(QWidget *parent = nullptr);
    ~CariKartlarDialog();

    Veritabani vt = Veritabani();
    CariYonetimi cariYonetimi = CariYonetimi();
    Yazici yazici = Yazici();
    enum FaturaTipi{
        alis = 1,
        satis = 2,
        iade = 3,
        odeme = 4,
        tahsilat = 5
    };
    Q_ENUM(FaturaTipi)

    QSound *uyariSesi = new QSound(":/sounds/sounds/warning-sound.wav", this);

    void setVergiDaireleri(const QStringList &newVergiDaireleri);

    void setKullanici(User newKullanici);

public slots:
    void cariKartlariListele();
private slots:
    void formLoad();
    void on_YenitoolButton_clicked();
    void cariHareketleriListele();
    void on_CaridenTahsilatYaptoolButton_clicked();
    void cariHareketleriTableSelectionChanged();
    void on_CariyeOdemeYaptoolButton_clicked();

    void on_SiltoolButton_clicked();

    void on_CariyiAlacaklandirtoolButton_clicked();

    void on_CariyiBorclandirtoolButton_clicked();

    void on_DuzenletoolButton_clicked();

    void on_CariKartHareketleritableView_doubleClicked(const QModelIndex &index);

    void on_TahsilatMakbuzuBastoolButton_clicked();

    void on_CariislemsiltoolButton_clicked();

    void on_guncelFiyatcheckBox_clicked();

private:
    Ui::CariKartlarDialog *ui;

    QStringList vergiDaireleri;
    User kullanici;
};

#endif // CARIKARTLARDIALOG_H
