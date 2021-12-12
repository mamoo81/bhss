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

    void on_ilcomboBox_currentIndexChanged(int index);

    void cariBilgileriGetir(QString _cariID);
    void on_CariKartlartableView_clicked(const QModelIndex &index);

    void alanlariDoldur(QString _cariID);
private:
    Ui::CariKartlarDialog *ui;
};

#endif // CARIKARTLARDIALOG_H
