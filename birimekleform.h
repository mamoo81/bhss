#ifndef BIRIMEKLEFORM_H
#define BIRIMEKLEFORM_H
#include "veritabani.h"
//**************************
#include <QDialog>

namespace Ui {
class BirimekleForm;
}

class BirimekleForm : public QDialog
{
    Q_OBJECT

public:
    explicit BirimekleForm(QWidget *parent = nullptr);
    ~BirimekleForm();

    Veritabani *vt_birim = new Veritabani();

private slots:
    void on_eklepushButton_clicked();

    void on_silpushButton_clicked();

    void FormLoad();
private:
    Ui::BirimekleForm *ui;
};

#endif // BIRIMEKLEFORM_H
