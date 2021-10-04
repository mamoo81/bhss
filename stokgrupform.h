#ifndef STOKGRUPFORM_H
#define STOKGRUPFORM_H

#include <QDialog>

namespace Ui {
class StokGrupForm;
}

class StokGrupForm : public QDialog
{
    Q_OBJECT

public:
    explicit StokGrupForm(QWidget *parent = nullptr);
    ~StokGrupForm();

private:
    Ui::StokGrupForm *ui;
};

#endif // STOKGRUPFORM_H
