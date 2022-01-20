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

    void setVergiDaireleri(const QStringList &newVergiDaireleri);

private slots:
    void formLoad();
    void on_YenitoolButton_clicked();
    void cariHareketleriListele();
    void on_CaridenTahsilatYaptoolButton_clicked();
    void cariHareketleriTableSelectionChanged();
private:
    Ui::CariKartlarDialog *ui;

    QStringList vergiDaireleri;
};

#endif // CARIKARTLARDIALOG_H
