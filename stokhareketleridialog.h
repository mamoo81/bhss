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
#ifndef STOKHAREKETLERIDIALOG_H
#define STOKHAREKETLERIDIALOG_H
#include "veritabani.h"
#include "stokkarti.h"
//*******************************
#include <QDialog>

namespace Ui {
class StokHareketleriDialog;
}

class StokHareketleriDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StokHareketleriDialog(QWidget *parent = nullptr);
    ~StokHareketleriDialog();

    Veritabani vt = Veritabani();

    void setStokKarti(const StokKarti &newStokKarti);

private slots:
    void on_bitisdateEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_baslangicdateEdit_dateTimeChanged(const QDateTime &dateTime);

    void getHareketler(QString _barkod, QDateTime _baslangicTarih, QDateTime _bitisTarih);
    void on_pushButton_clicked();

private:
    Ui::StokHareketleriDialog *ui;

    QString stokKartiBarkod;
    StokKarti kart;
};

#endif // STOKHAREKETLERIDIALOG_H
