#ifndef RAFETIKETIDIALOG_H
#define RAFETIKETIDIALOG_H

#include <QDialog>

namespace Ui {
class RafEtiketiDialog;
}

class RafEtiketiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RafEtiketiDialog(QWidget *parent = nullptr);
    ~RafEtiketiDialog();

private:
    Ui::RafEtiketiDialog *ui;
};

#endif // RAFETIKETIDIALOG_H
