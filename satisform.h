#ifndef SATISFORM_H
#define SATISFORM_H

#include "user.h"


#include <QWidget>

namespace Ui {
class SatisForm;
}

class SatisForm : public QWidget
{
    Q_OBJECT

public:
    explicit SatisForm(QWidget *parent = nullptr);
    ~SatisForm();

    void setUser(User *user);
    void formLoad();

public slots:

private slots:
    void on_StokKartlariBtn_clicked();
    void initTableWidgets();

private:
    Ui::SatisForm *ui;
};

#endif // SATISFORM_H
