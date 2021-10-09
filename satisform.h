#ifndef SATISFORM_H
#define SATISFORM_H

#include "user.h"
#include "stokkarti.h"

#include <QWidget>
#include <QKeyEvent>
#include <QCloseEvent>

namespace Ui {
class SatisForm;
}

class SatisForm : public QWidget
{
    Q_OBJECT

public:
    explicit SatisForm(QWidget *parent = nullptr);
    ~SatisForm();

    void setUser(User *user);
    void formLoad();

    bool eventFilter(QObject *watched, QEvent *event);
public slots:

private slots:
    void on_StokKartlariBtn_clicked();
    void initTableWidgets();

    void on_SepetlertabWidget_currentChanged(int index);
    void sepetToplaminiYaz(int p_HangiSepet);
    void barkodVarmi(QString bakilacakBarkod);
    void sepeteEkle(StokKarti p_StokKarti);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool urunSepetteVarmi(QString p_Barkod);
private:
    Ui::SatisForm *ui;
};

#endif // SATISFORM_H
