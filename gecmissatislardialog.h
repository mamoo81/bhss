#ifndef GECMISSATISLARDIALOG_H
#define GECMISSATISLARDIALOG_H
#include "veritabani.h"

#include <QDialog>

namespace Ui {
class GecmisSatislarDialog;
}

class GecmisSatislarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GecmisSatislarDialog(QWidget *parent = nullptr);
    ~GecmisSatislarDialog();
    Veritabani vt = Veritabani();

public slots:
    void FormLoad();

private slots:
    void on_SonSatislarlistWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::GecmisSatislarDialog *ui;
};

#endif // GECMISSATISLARDIALOG_H
