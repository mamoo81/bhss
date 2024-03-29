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
#ifndef STOKGRUPFORM_H
#define STOKGRUPFORM_H
#include "stokyonetimi.h"
//*****************************
#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class StokGrupForm;
}

class StokGrupForm : public QDialog
{
    Q_OBJECT

public:
    explicit StokGrupForm(QWidget *parent = nullptr);
    ~StokGrupForm();

//    Veritabani *vt_grup = new Veritabani();
    StokYonetimi stokYonetimi = StokYonetimi();

private slots:
    void formLoad();

    void on_EkleBtn_clicked();

    void on_SilBtn_clicked();

    void on_KaydetBtn_clicked();

    void stokGruplariGetir();

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::StokGrupForm *ui;
};

#endif // STOKGRUPFORM_H
