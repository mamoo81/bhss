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
#ifndef TOPLUSTOKYUKLEDIALOG_H
#define TOPLUSTOKYUKLEDIALOG_H
#include "veritabani.h"
#include "stokkarti.h"
//*****************************
#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QSound>
#include <QVector>

namespace Ui {
class TopluStokYukleDialog;
}

class TopluStokYukleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TopluStokYukleDialog(QWidget *parent = nullptr);
    ~TopluStokYukleDialog();

    QSqlDatabase db = QSqlDatabase::database("mhss_data");
    QSqlQuery sorgu = QSqlQuery(db);
    Veritabani vt = Veritabani();
    QFile dosya;
    QString dosyaYolu;
    int yuklenen = 0;
    int esGecilen = 0;
    int basarisiz = 0;
    QStringList mevcutBarkodlar;

    QSound *uyariSesi = new QSound(":/sounds/sounds/warning-sound.wav", this);

    void jsondanYukle();

    void csvdenYukle();

private slots:
    void on_SecpushButton_clicked();

    void on_YuklepushButton_clicked();

    void on_KapatpushButton_clicked();

private:
    Ui::TopluStokYukleDialog *ui;

    QVector<StokKarti> kartlar;
};

#endif // TOPLUSTOKYUKLEDIALOG_H
