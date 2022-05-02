#ifndef STOKKARTLARIMODEL_H
#define STOKKARTLARIMODEL_H
#include "stokkarti.h"
#include "veritabani.h"
//*******************************
#include <QObject>
#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlRecord>
#include <QSettings>

class StokKartlariModel : public QAbstractItemModel
{

public:
    explicit StokKartlariModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;


    //veritabanı metodları
    Veritabani *vt = new Veritabani();
    QSqlQueryModel *_QueryModel;
    QVector<StokKarti> _kartlar;

    double _stokUyariMiktari = 0;
    bool _takipAktifMi = false;

private:
    int _nColumn;

};

#endif // STOKKARTLARIMODEL_H
