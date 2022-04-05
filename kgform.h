#ifndef KGFORM_H
#define KGFORM_H
#include "terazi.h"

#include <QDialog>

namespace Ui {
class KgForm;
}

class KgForm : public QDialog
{
    Q_OBJECT

public:
    explicit KgForm(QWidget *parent = nullptr);
    ~KgForm();

    Terazi *terazi;
    float girilenKilo = 0;
    void setBirimi(const QString &newBirimi);

public slots:
    float getGirilenKg();

    void StabilOlcumGelince(double olcum);
    void OlcumGelince(double olcum);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::KgForm *ui;
    QString birimi;
};

#endif // KGFORM_H
