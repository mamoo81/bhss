#include "yenicarikartdialog.h"
#include "ui_yenicarikartdialog.h"
//************************************
#include <QCompleter>

YeniCariKartDialog::YeniCariKartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YeniCariKartDialog)
{
    ui->setupUi(this);
    FormLoad();
}

YeniCariKartDialog::~YeniCariKartDialog()
{
    delete ui;
}

void YeniCariKartDialog::FormLoad()
{
    ui->CariTipicomboBox->addItems(vt->getCariTipleri());
    // vergi dairelerni veritabanından alıp ilgili objeye yazarken otomatik doldurma için.
    QCompleter *tamamlayici = new QCompleter(vt->getVergiDaireleri(), this);
    tamamlayici->setCompletionMode(QCompleter::PopupCompletion);
    tamamlayici->setCaseSensitivity(Qt::CaseInsensitive);
    ui->VergiDairesilineEdit->setCompleter(tamamlayici);
    //
    ui->ilcomboBox->addItems(vt->getIller());
}

void YeniCariKartDialog::on_iptalpushButton_clicked()
{
    this->close();
}

