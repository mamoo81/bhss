#ifndef STOKKARTLARIMODEL_H
#define STOKKARTLARIMODEL_H
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

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    //veritabanı metodları
    Veritabani *vt = new Veritabani();
    QSqlQueryModel *_QueryModel= new QSqlQueryModel();

    double _stokUyariMiktari = 0;
    bool _takipAktifMi = false;

private:
    int _nColumn;

};

#endif // STOKKARTLARIMODEL_H
