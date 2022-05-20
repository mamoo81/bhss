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
#ifndef RESIMEKLEDIALOG_H
#define RESIMEKLEDIALOG_H

#include <QDialog>
#include <QFile>

namespace Ui {
class ResimEkleDialog;
}

class ResimEkleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResimEkleDialog(QWidget *parent = nullptr);
    ~ResimEkleDialog();

    QFile *resim;
    QString resimlerDizini;

    void setUrunBarkod(const QString &newUrunBarkod);

private slots:
    void on_kaydetpushButton_clicked();

    void on_secpushButton_clicked();

    void on_iptalpushButton_clicked();

private:
    Ui::ResimEkleDialog *ui;

    QString urunBarkod;
};

#endif // RESIMEKLEDIALOG_H
