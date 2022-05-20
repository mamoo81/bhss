#ifndef HIZLIURUNEKLEFORMDIALOG_H
#define HIZLIURUNEKLEFORMDIALOG_H

#include <QDialog>
#include <QShortcut>
#include <QList>

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
    QString hizliUrunStokID;
    bool ok = false;

public slots:
    void stokKartlariniListele();

private slots:
    void on_KaydetpushButton_clicked();

    void on_IptalpushButton_clicked();

    void on_StokKartlaritableView_doubleClicked(const QModelIndex &index);

    void on_lineEdit_textChanged(const QString &arg1);

    void key_Down_Slot();
    void key_UP_Slot();

    void on_StokKartlaritableView_clicked(const QModelIndex &index);

private:
    Ui::HizliUrunEkleFormDialog *ui;

    QShortcut *key_Down;
    QShortcut *key_DownArrow;
    QShortcut *key_Up;
    QShortcut *key_UpArrow;

    QList<int> gosterilenSatirlar;
    int sonSecilenGosterilenSatirIndexi = 0;
    int secilecekIndex;
};

#endif // HIZLIURUNEKLEFORMDIALOG_H
