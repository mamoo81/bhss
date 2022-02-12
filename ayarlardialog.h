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

    QString periyod = NULL;
    QString jobSaati = NULL;
    QString jobDakika = NULL;

private slots:
    void on_YeniPushButton_clicked();

    void formLoad();
    void on_KullanicilarlistWidget_itemSelectionChanged();

    void on_DuzenlePushButton_clicked();

    void on_SilPushButton_clicked();

    void getUsers();
    void on_pushButton_clicked();

    void on_YedeklepushButton_clicked();

    void on_GeriYuklepushButton_clicked();

    void on_KonuSecpushButton_clicked();

    void on_OtomatikYedekcheckBox_clicked();

    void cronJobKaydet();

    void cronJobSil();

    void on_SiklikcomboBox_currentIndexChanged(const QString &arg1);

    void on_SaatcomboBox_currentIndexChanged(int index);

    void on_dakikacomboBox_currentIndexChanged(int index);

    void on_SifirlapushButton_clicked();

    void VtYedeklemeButonlariniAyarla();
private:
    Ui::AyarlarDialog *ui;

    User currentUser;
};

#endif // AYARLARDIALOG_H
