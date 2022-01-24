#ifndef YENICARIKARTDIALOG_H
#define YENICARIKARTDIALOG_H
#include "veritabani.h"
//**************************
#include <QDialog>

namespace Ui {
class YeniCariKartDialog;
}

class YeniCariKartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit YeniCariKartDialog(QWidget *parent = nullptr);
    ~YeniCariKartDialog();

    Veritabani *vt = new Veritabani();

private slots:
    void FormLoad();
    void on_iptalpushButton_clicked();

    void on_ilcomboBox_currentIndexChanged(int index);

    void on_KaydetpushButton_clicked();

private:
    Ui::YeniCariKartDialog *ui;
};

#endif // YENICARIKARTDIALOG_H
