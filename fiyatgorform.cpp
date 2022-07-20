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
#include "fiyatgorform.h"
#include "ui_fiyatgorform.h"
#include "veritabani.h"
#include "stokkarti.h"
//*****************************
#include <QKeyEvent>
#include <QTimer>

FiyatGorForm::FiyatGorForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiyatGorForm)
{
    ui->setupUi(this);
}

FiyatGorForm::~FiyatGorForm()
{
    delete ui;
}

void FiyatGorForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        if(!ui->BarkodlineEdit->text().isEmpty()){
            Veritabani *vt = new Veritabani();
            StokKarti kart = vt->getStokKarti(ui->BarkodlineEdit->text());
            ui->urunAdiLabel->setText(kart.getAd());
            ui->fiyatLabel->setText("₺" + QString::number(kart.getSFiyat(), 'f', 2));
            ui->urunResmilabel->setPixmap(kart.getResim());
            QTimer::singleShot(1500, this, &FiyatGorForm::slotTimer);
            delete vt;
        }
    }
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
}

void FiyatGorForm::slotTimer()
{
    this->close();
}
