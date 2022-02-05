#ifndef TOPLUSTOKYUKLEDIALOG_H
#define TOPLUSTOKYUKLEDIALOG_H
#include "veritabani.h"
//*****************************
#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>

namespace Ui {
class TopluStokYukleDialog;
}

class TopluStokYukleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TopluStokYukleDialog(QWidget *parent = nullptr);
    ~TopluStokYukleDialog();

    QSqlDatabase db = QSqlDatabase::database("mhss_data");
    QSqlQuery sorgu = QSqlQuery(db);
    Veritabani vt = Veritabani();
    QFile dosya;
    QString dosyaYolu;
    int yuklenen = 0;
    int esGecilen = 0;
    int basarisiz = 0;

private slots:
    void on_SecpushButton_clicked();

    void on_YuklepushButton_clicked();

private:
    Ui::TopluStokYukleDialog *ui;
};

#endif // TOPLUSTOKYUKLEDIALOG_H
