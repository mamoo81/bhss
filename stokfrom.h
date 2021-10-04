#ifndef STOKFROM_H
#define STOKFROM_H

#include <QDialog>

namespace Ui {
class StokFrom;
}

class StokFrom : public QDialog
{
    Q_OBJECT

public:
    explicit StokFrom(QWidget *parent = nullptr);
    ~StokFrom();

private:
    Ui::StokFrom *ui;
};

#endif // STOKFROM_H
