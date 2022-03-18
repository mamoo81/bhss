#ifndef RESIMEKLEDIALOG_H
#define RESIMEKLEDIALOG_H

#include <QDialog>
#include <QFile>

namespace Ui {
class ResimEkleDialog;
}

class ResimEkleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResimEkleDialog(QWidget *parent = nullptr);
    ~ResimEkleDialog();

    QFile *resim;

    void setUrunBarkod(const QString &newUrunBarkod);

private slots:
    void on_kaydetpushButton_clicked();

    void on_secpushButton_clicked();

private:
    Ui::ResimEkleDialog *ui;

    QString urunBarkod;
};

#endif // RESIMEKLEDIALOG_H
