#ifndef STOKHAREKETLERIDIALOG_H
#define STOKHAREKETLERIDIALOG_H
#include "veritabani.h"
//*******************************
#include <QDialog>

namespace Ui {
class StokHareketleriDialog;
}

class StokHareketleriDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StokHareketleriDialog(QWidget *parent = nullptr);
    ~StokHareketleriDialog();

    Veritabani vt = Veritabani();

    void setStokBarkod(const QString &newStokBarkod);

private slots:
    void on_bitisdateEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_baslangicdateEdit_dateTimeChanged(const QDateTime &dateTime);

    void getHareketler(QString _barkod, QDateTime _baslangicTarih, QDateTime _bitisTarih);
private:
    Ui::StokHareketleriDialog *ui;

    QString stokKartiBarkod;
};

#endif // STOKHAREKETLERIDIALOG_H
