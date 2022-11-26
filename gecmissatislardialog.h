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
#ifndef GECMISSATISLARDIALOG_H
#define GECMISSATISLARDIALOG_H
#include "veritabani.h"
#include "user.h"
#include "faturayonetimi.h"

#include <QDialog>

namespace Ui {
class GecmisSatislarDialog;
}

class GecmisSatislarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GecmisSatislarDialog(QWidget *parent = nullptr);
    ~GecmisSatislarDialog();
    Veritabani vt = Veritabani();
    FaturaYonetimi faturaYonetimi = FaturaYonetimi();

    void setKullanici(const User &value);

public slots:
    void FormLoad();

    void iadeleriRenklendir();

private slots:
    void on_SonSatislarlistWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_KapatpushButton_clicked();

private:
    Ui::GecmisSatislarDialog *ui;

    User kullanici;
};

#endif // GECMISSATISLARDIALOG_H
