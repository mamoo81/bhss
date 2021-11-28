#ifndef PARACEKDIALOGFORM_H
#define PARACEKDIALOGFORM_H
#include "user.h"
#include "veritabani.h"
//******************************
#include <QDialog>

namespace Ui {
class ParaCekDialogForm;
}

class ParaCekDialogForm : public QDialog
{
    Q_OBJECT

public:
    explicit ParaCekDialogForm(QWidget *parent = nullptr);
    ~ParaCekDialogForm();

    Veritabani vt = Veritabani();

public slots:

    void setKasadakiPara(double newKasadakiPara);

    void setKull(const User &newKull);

private slots:
    void on_IptalpushButton_clicked();

    void on_CekpushButton_clicked();

    void on_doubleSpinBox_valueChanged(double curValue);

private:
    Ui::ParaCekDialogForm *ui;

    double kasadakiPara;
    User kull;
};

#endif // PARACEKDIALOGFORM_H
