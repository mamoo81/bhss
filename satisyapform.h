#ifndef SATISYAPFORM_H
#define SATISYAPFORM_H
#include "sepet.h"

#include <QDialog>

namespace Ui {
class SatisYapForm;
}

class SatisYapForm : public QDialog
{
    Q_OBJECT

public:
    explicit SatisYapForm(QWidget *parent = nullptr);
    ~SatisYapForm();

    Sepet satilacakSepet;

public slots:
    void formLoad();

private slots:


    void on_satBtn_clicked();

private:
    Ui::SatisYapForm *ui;
};

#endif // SATISYAPFORM_H
