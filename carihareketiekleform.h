#ifndef CARIHAREKETIEKLEFORM_H
#define CARIHAREKETIEKLEFORM_H
#include "veritabani.h"
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

    Veritabani *vt = new Veritabani();

private slots:
    void FormLoad();
private:
    Ui::CariHareketiEkleForm *ui;
};

#endif // CARIHAREKETIEKLEFORM_H
