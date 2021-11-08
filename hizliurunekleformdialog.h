#ifndef HIZLIURUNEKLEFORMDIALOG_H
#define HIZLIURUNEKLEFORMDIALOG_H

#include <QDialog>

namespace Ui {
class HizliUrunEkleFormDialog;
}

class HizliUrunEkleFormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HizliUrunEkleFormDialog(QWidget *parent = nullptr);
    ~HizliUrunEkleFormDialog();

    QString hizliUrunBarkod;
    QString hizliUrunAd;
    bool ok = false;

public slots:
    void stokKartlariniListele();

private slots:
    void on_KaydetpushButton_clicked();

    void on_IptalpushButton_clicked();

private:
    Ui::HizliUrunEkleFormDialog *ui;
};

#endif // HIZLIURUNEKLEFORMDIALOG_H