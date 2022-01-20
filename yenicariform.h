#ifndef YENICARIFORM_H
#define YENICARIFORM_H

#include <QDialog>

namespace Ui {
class YeniCariForm;
}

class YeniCariForm : public QDialog
{
    Q_OBJECT

public:
    explicit YeniCariForm(QWidget *parent = nullptr);
    ~YeniCariForm();

private:
    Ui::YeniCariForm *ui;
};

#endif // YENICARIFORM_H
