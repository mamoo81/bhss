#ifndef SATISGOSTERDIALOG_H
#define SATISGOSTERDIALOG_H

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


    void setSatisFaturaNo(const QString &newSatisFaturaNo);

public slots:
    void sepetiCek();
private slots:
    void initTableWidgets();
    void on_kapatpushButton_clicked();

private:
    Ui::SatisGosterDialog *ui;

    QString satisFaturaNo;
};

#endif // SATISGOSTERDIALOG_H
