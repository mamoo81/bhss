#ifndef SATISGRAFIGIFORM_H
#define SATISGRAFIGIFORM_H

#include <QDialog>

namespace Ui {
class SatisGrafigiForm;
}

class SatisGrafigiForm : public QDialog
{
    Q_OBJECT

public:
    explicit SatisGrafigiForm(QWidget *parent = nullptr);
    ~SatisGrafigiForm();

private slots:
    void FormLoad();
private:
    Ui::SatisGrafigiForm *ui;
};

#endif // SATISGRAFIGIFORM_H
