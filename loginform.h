#ifndef LOGINFORM_H
#define LOGINFORM_H
#include "veritabani.h"
//**********************
#include <QMainWindow>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginForm; }
QT_END_NAMESPACE

class LoginForm : public QMainWindow
{
    Q_OBJECT

public:
    LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

    Veritabani vt = Veritabani();

    void formLoad();

    void setCapsLockFilePath(const QString &newCapsLockFilePath);

private slots:
    void on_GirisBtn_clicked();

    void on_KapatBtn_clicked();

    void closeEvent(QCloseEvent *);

    void keyPressEvent(QKeyEvent *event);
    bool getCapslockState();
private:
    Ui::LoginForm *ui;

    QString capsLockFilePath;
};
#endif // LOGINFORM_H
