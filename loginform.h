#ifndef LOGINFORM_H
#define LOGINFORM_H

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

    bool capslock;

    void formLoad();
private slots:
    void on_GirisBtn_clicked();

    void on_KapatBtn_clicked();

    void closeEvent(QCloseEvent *);

    void getUsers();

    void keyPressEvent(QKeyEvent *event);
    void getCapslockState();
private:
    Ui::LoginForm *ui;
};
#endif // LOGINFORM_H
