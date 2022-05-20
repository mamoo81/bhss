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
#ifndef KULLANICIDIALOGFORM_H
#define KULLANICIDIALOGFORM_H
#include "veritabani.h"
#include "user.h"
//*****************************
#include <QDialog>

namespace Ui {
class KullaniciDialogForm;
}

class KullaniciDialogForm : public QDialog
{
    Q_OBJECT

public:
    explicit KullaniciDialogForm(QWidget *parent = nullptr);
    ~KullaniciDialogForm();

    Veritabani vt = Veritabani();


    void setYeniMi(bool newYeniMi);

    void setDuzenlenecekUser(const QString &newDuzenlenecekUserName);

private slots:
    void on_pushButton_clicked();

    void formLoad();
    void on_pushButton_2_clicked();

private:
    Ui::KullaniciDialogForm *ui;

    bool yeniMi;
    QString duzenlenecekUserName;
};

#endif // KULLANICIDIALOGFORM_H
