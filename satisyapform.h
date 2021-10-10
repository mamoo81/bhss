#ifndef SATISYAPFORM_H
#define SATISYAPFORM_H

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

private:
    Ui::SatisYapForm *ui;
};

#endif // SATISYAPFORM_H
