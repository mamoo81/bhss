#ifndef CARIFORM_H
#define CARIFORM_H

#include <QDialog>

namespace Ui {
class CariForm;
}

class CariForm : public QDialog
{
    Q_OBJECT

public:
    explicit CariForm(QWidget *parent = nullptr);
    ~CariForm();

private:
    Ui::CariForm *ui;
};

#endif // CARIFORM_H
