#include "rafetiketidialog.h"
#include "ui_rafetiketidialog.h"

RafEtiketiDialog::RafEtiketiDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RafEtiketiDialog)
{
    ui->setupUi(this);
}

RafEtiketiDialog::~RafEtiketiDialog()
{
    delete ui;
}
