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

    enum FaturaTipi{
        alis = 1,
        satis = 2,
        iade = 3,
        odeme = 4,
        tahsilat = 5
    };
    Q_ENUM(FaturaTipi)

    void setVergiDaireleri(const QStringList &newVergiDaireleri);

    void setKullanici(User newKullanici);

public slots:
    void cariKartlariListele();
private slots:
    void formLoad();
    void on_YenitoolButton_clicked();
    void cariHareketleriListele();
    void on_CaridenTahsilatYaptoolButton_clicked();
    void cariHareketleriTableSelectionChanged();
    void on_CariyeOdemeYaptoolButton_clicked();

    void on_SiltoolButton_clicked();

    void on_CariyiAlacaklandirtoolButton_clicked();

    void on_CariyiBorclandirtoolButton_clicked();

    void on_DuzenletoolButton_clicked();

private:
    Ui::CariKartlarDialog *ui;

    QStringList vergiDaireleri;
    User kullanici;
};

#endif // CARIKARTLARDIALOG_H
