#ifndef STOKGRUPFORM_H
#define STOKGRUPFORM_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class StokGrupForm;
}

class StokGrupForm : public QDialog
{
    Q_OBJECT

public:
    explicit StokGrupForm(QWidget *parent = nullptr);
    ~StokGrupForm();

private slots:
    void formLoad();

    void on_EkleBtn_clicked();

    void on_SilBtn_clicked();

    void on_KaydetBtn_clicked();

    void stokGruplariGetir();

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::StokGrupForm *ui;
};

#endif // STOKGRUPFORM_H
