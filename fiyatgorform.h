#ifndef FIYATGORFORM_H
#define FIYATGORFORM_H

#include <QDialog>

namespace Ui {
class FiyatGorForm;
}

class FiyatGorForm : public QDialog
{
    Q_OBJECT

public:
    explicit FiyatGorForm(QWidget *parent = nullptr);
    ~FiyatGorForm();

public slots:
    void keyPressEvent(QKeyEvent *event);
    void slotTimer();
private:
    Ui::FiyatGorForm *ui;
};

#endif // FIYATGORFORM_H
