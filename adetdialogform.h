#ifndef ADETDIALOGFORM_H
#define ADETDIALOGFORM_H

#include <QDialog>

namespace Ui {
class AdetDialogForm;
}

class AdetDialogForm : public QDialog
{
    Q_OBJECT

public:
    explicit AdetDialogForm(QWidget *parent = nullptr);
    ~AdetDialogForm();

    int carpilacakAdet = 1;
    int mevcutAdet;

public slots:
    void keyPressEvent(QKeyEvent *keyEvent);
private slots:
    void on_pushButton_clicked();

private:
    Ui::AdetDialogForm *ui;
};

#endif // ADETDIALOGFORM_H
