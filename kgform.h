#ifndef KGFORM_H
#define KGFORM_H

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

    float girilenKilo = 0;
    void setBirimi(const QString &newBirimi);

public slots:
    float getGirilenKg();

private slots:
    void on_pushButton_clicked();

private:
    Ui::KgForm *ui;
    QString birimi;
};

#endif // KGFORM_H
