#ifndef KULLANICIDIALOGFORM_H
#define KULLANICIDIALOGFORM_H
#include "veritabani.h"
#include "user.h"
//*****************************
#include <QDialog>

namespace Ui {
class KullaniciDialogForm;
}

class KullaniciDialogForm : public QDialog
{
    Q_OBJECT

public:
    explicit KullaniciDialogForm(QWidget *parent = nullptr);
    ~KullaniciDialogForm();

    Veritabani vt = Veritabani();


    void setYeniMi(bool newYeniMi);

    void setDuzenlenecekUser(const QString &newDuzenlenecekUserName);

private slots:
    void on_pushButton_clicked();

    void formLoad();
    void on_pushButton_2_clicked();

private:
    Ui::KullaniciDialogForm *ui;

    bool yeniMi;
    QString duzenlenecekUserName;
};

#endif // KULLANICIDIALOGFORM_H
