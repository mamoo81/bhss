#ifndef SATISGOSTERDIALOG_H
#define SATISGOSTERDIALOG_H
#include "yazici.h"
//*****************************
#include <QDialog>

namespace Ui {
class SatisGosterDialog;
}

class SatisGosterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SatisGosterDialog(QWidget *parent = nullptr);
    ~SatisGosterDialog();

    Sepet satilmisSepet;
    Yazici fis = Yazici();

    void setSatisFaturaNo(const QString &newSatisFaturaNo);

public slots:
    void sepetiCek();
private slots:
    void initTableWidgets();
    void on_kapatpushButton_clicked();

    void on_FisYazdirpushButton_clicked();

private:
    Ui::SatisGosterDialog *ui;

    QString satisFaturaNo;
};

#endif // SATISGOSTERDIALOG_H
