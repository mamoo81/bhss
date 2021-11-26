#ifndef AYARLARDIALOG_H
#define AYARLARDIALOG_H

#include <QDialog>

namespace Ui {
class AyarlarDialog;
}

class AyarlarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AyarlarDialog(QWidget *parent = nullptr);
    ~AyarlarDialog();

private:
    Ui::AyarlarDialog *ui;
};

#endif // AYARLARDIALOG_H
