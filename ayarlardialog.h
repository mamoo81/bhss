#ifndef AYARLARDIALOG_H
#define AYARLARDIALOG_H
#include "user.h"
#include "veritabani.h"
//****************************
#include <QDialog>

namespace Ui {
class AyarlarDialog;
}

class AyarlarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AyarlarDialog(QWidget *parent = nullptr);
    ~AyarlarDialog();

    Veritabani vt = Veritabani();

    void setCurrentUser(const User &newCurrentUser);

private slots:
    void on_YeniPushButton_clicked();

    void formLoad();
    void on_KullanicilarlistWidget_itemSelectionChanged();

    void on_DuzenlePushButton_clicked();

    void on_SilPushButton_clicked();

    void getUsers();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_YedeklepushButton_clicked();

    void on_GeriYuklepushButton_clicked();

    void on_KonuSecpushButton_clicked();

    void on_OtomatikYedekcheckBox_clicked();


private:
    Ui::AyarlarDialog *ui;

    User currentUser;
};

#endif // AYARLARDIALOG_H
