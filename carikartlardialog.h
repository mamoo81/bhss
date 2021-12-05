#ifndef CARIKARTLARDIALOG_H
#define CARIKARTLARDIALOG_H
#include "veritabani.h"
#include "cari.h"
//**************************
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

    Veritabani vt = Veritabani();

private slots:
    void formLoad();
    void on_YenitoolButton_clicked();

    void alanlariTemizle();
    void on_KaydettoolButton_clicked();

private:
    Ui::CariKartlarDialog *ui;
};

#endif // CARIKARTLARDIALOG_H
