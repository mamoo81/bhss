#ifndef SATISFORM_H
#define SATISFORM_H

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

private:
    Ui::SatisForm *ui;
};

#endif // SATISFORM_H
