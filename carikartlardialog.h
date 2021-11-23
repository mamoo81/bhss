#ifndef CARIKARTLARDIALOG_H
#define CARIKARTLARDIALOG_H

#include <QDialog>

namespace Ui {
class CariKartlarDialog;
}

class CariKartlarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CariKartlarDialog(QWidget *parent = nullptr);
    ~CariKartlarDialog();

private:
    Ui::CariKartlarDialog *ui;
};

#endif // CARIKARTLARDIALOG_H
