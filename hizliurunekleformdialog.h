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
#ifndef HIZLIURUNEKLEFORMDIALOG_H
#define HIZLIURUNEKLEFORMDIALOG_H

#include <QDialog>
#include <QShortcut>
#include <QList>
#include <QSqlDatabase>

namespace Ui {
class HizliUrunEkleFormDialog;
}

class HizliUrunEkleFormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HizliUrunEkleFormDialog(QWidget *parent = nullptr);
    ~HizliUrunEkleFormDialog();

    QSqlDatabase db = QSqlDatabase::database("mhss_data");

    QString hizliUrunBarkod;
    QString hizliUrunAd;
    QString hizliUrunStokID;
    bool ok = false;

public slots:
    void stokKartlariniListele();

private slots:
    void on_KaydetpushButton_clicked();

    void on_IptalpushButton_clicked();

    void on_StokKartlaritableView_doubleClicked(const QModelIndex &index);

    void on_lineEdit_textChanged(const QString &arg1);

    void key_Down_Slot();
    void key_UP_Slot();

    void on_StokKartlaritableView_clicked(const QModelIndex &index);

private:
    Ui::HizliUrunEkleFormDialog *ui;

    QShortcut *key_Down;
    QShortcut *key_DownArrow;
    QShortcut *key_Up;
    QShortcut *key_UpArrow;

    QList<int> gosterilenSatirlar;
    int sonSecilenGosterilenSatirIndexi = 0;
    int secilecekIndex;
};

#endif // HIZLIURUNEKLEFORMDIALOG_H
