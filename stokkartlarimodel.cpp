#include "stokkartlarimodel.h"

#include <QBrush>

StokKartlariModel::StokKartlariModel(QObject *parent) : QAbstractItemModel(parent)
{
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    genelAyarlar.beginGroup("stok");
    _takipAktifMi = genelAyarlar.value("takip").toBool();
    _stokUyariMiktari = genelAyarlar.value("uyarimiktar").toDouble();
    genelAyarlar.endGroup();

    _QueryModel = vt->getStokKartlari();
    _nColumn = _QueryModel->columnCount();
}

int StokKartlariModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _QueryModel->rowCount();
}

int StokKartlariModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _nColumn;
}

QVariant StokKartlariModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() == false) return QVariant();

    if(_QueryModel->rowCount() == 0) return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return QVariant(_QueryModel->record(index.row()).value("id").toString());
        case 1:
            return QVariant(_QueryModel->record(index.row()).value("barkod").toString());
        case 2:
            return QVariant(_QueryModel->record(index.row()).value("kod").toString());
        case 3:
            return QVariant(_QueryModel->record(index.row()).value("ad").toString());
        case 4:
            return QVariant(_QueryModel->record(index.row()).value("stokbirimleri.birim").toString());
        case 5:
            return QVariant(_QueryModel->record(index.row()).value("miktar").toDouble());
        case 6:
            return QVariant(_QueryModel->record(index.row()).value("grup").toString());
        case 7:
            return QVariant(QString::number(_QueryModel->record(index.row()).value("afiyat").toDouble(), 'f', 2));
        case 8:
            return QVariant(QString::number(_QueryModel->record(index.row()).value("sfiyat").toDouble(), 'f', 2));
        case 9:
            return QVariant("%" + QString::number(_QueryModel->record(index.row()).value("kdv").toInt()));
        case 10:
            return QVariant("%" + QString::number(_QueryModel->record(index.row()).value("otv").toInt()));
        case 11:
            if(_QueryModel->record(index.row()).value("kdvdahil").toBool()){
                return QVariant("Evet");
            }
            else{
                return QVariant("Hayır");
            }
        case 12:
            if(_QueryModel->record(index.row()).value("otvdahil").toBool()){
                return QVariant("Evet");
            }
            else{
                return QVariant("Hayır");
            }
        case 13:
            return QVariant(_QueryModel->record(index.row()).value("tarih").toDateTime().toString("dd.MM.yyyy hh:mm"));
        case 14:
            return QVariant(_QueryModel->record(index.row()).value("ureticiler.ad").toString());
        case 15:
            return QVariant(_QueryModel->record(index.row()).value("carikartlar.ad").toString());
        case 16:
                return QVariant(_QueryModel->record(index.row()).value("aciklama").toString());
        default:
            return QVariant();
        }
    case Qt::BackgroundColorRole: // renklendirme için
        if(_takipAktifMi){
            // satırı renklendirme
            if(_QueryModel->record(index.row()).value("miktar").toDouble() <= _stokUyariMiktari){// miktar uyarı miktarından düşük veya eşitse
                return QBrush(Qt::red);
            }
//            else{
//                if(index.column() == 5){ // hücreyi renklendirme
//                    return QBrush(Qt::green);
//                }
//            }
        }
        break;
    case Qt::TextAlignmentRole:
        switch (index.column()) {
        case 5:
            return int(Qt::AlignRight | Qt::AlignVCenter);
        case 6:
            return int(Qt::AlignRight | Qt::AlignVCenter);
        case 7:
            return int(Qt::AlignRight | Qt::AlignVCenter);
        case 8:
            return int(Qt::AlignRight | Qt::AlignVCenter);
        case 9:
            return int(Qt::AlignRight | Qt::AlignVCenter);
        case 10:
            return int(Qt::AlignRight | Qt::AlignVCenter);
        case 11:
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        case 12:
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        default:
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    return QVariant();
}

QVariant StokKartlariModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            switch (section) {
            case 0:
                return "ID";
            case 1:
                return "Barkod";
            case 2:
                return "Kod";
            case 3:
                return "Stok Adı";
            case 4:
                return "Birimi";
            case 5:
                return "Miktarı";
            case 6:
                return "Stok grubu";
            case 7:
                return "Alış Fiyat";
            case 8:
                return "Satış Fiyat";
            case 9:
                return "KDV";
            case 10:
                return "OTV";
            case 11:
                return "KDV Dahil";
            case 12:
                return "OTV Dahil";
            case 13:
                return "Değişiklik Tarihi";
            case 14:
                return "Üretici";
            case 15:
                return "Tedarikçi";
            case 16:
                return "Açıklama";
            default:
                return QAbstractItemModel::headerData(section,orientation,role);
            }
        }
    }
    return QAbstractItemModel::headerData(section,orientation,role);
}

QModelIndex StokKartlariModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QModelIndex StokKartlariModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int rowMax = _QueryModel->rowCount();
    if(row < rowMax && row >= 0 && column < _nColumn && column >= 0){
        return createIndex(row, column);
    }
    return QModelIndex();
}
