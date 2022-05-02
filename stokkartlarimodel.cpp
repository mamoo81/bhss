#include "stokkartlarimodel.h"

StokKartlariModel::StokKartlariModel(QObject *parent) : QAbstractItemModel(parent)
{
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    genelAyarlar.beginGroup("stok");
    _takipAktifMi = genelAyarlar.value("takip").toBool();
    _stokUyariMiktari = genelAyarlar.value("uyarimiktar").toDouble();
    genelAyarlar.endGroup();

    _QueryModel = vt->getStokKartlari();
    _nColumn = _QueryModel->columnCount();

    for (int var = 0; var < _QueryModel->rowCount(); ++var) {
        StokKarti kart = StokKarti();
        kart.setId(_QueryModel->record(var).value("id").toString());
        kart.setBarkod(_QueryModel->record(var).value("barkod").toString());
        kart.setKod(_QueryModel->record(var).value("kod").toString());
        kart.setAd(_QueryModel->record(var).value("ad").toString());
        kart.setBirim(_QueryModel->record(var).value("birim").toString());
        kart.setMiktar(_QueryModel->record(var).value("miktar").toDouble());
        kart.setGrup(_QueryModel->record(var).value("grup").toString());
        kart.setAFiyat(_QueryModel->record(var).value("afiyat").toDouble());
        kart.setSFiyat(_QueryModel->record(var).value("sfiyat").toDouble());
        kart.setKdv(_QueryModel->record(var).value("kdv").toInt());
        kart.setOtv(_QueryModel->record(var).value("otv").toInt());
        kart.setKdvdahil(_QueryModel->record(var).value("kdvdahil").toBool());
        kart.setOtvdahil(_QueryModel->record(var).value("otvdahil").toBool());
        kart.setTarih(_QueryModel->record(var).value("tarih").toDateTime());
        kart.setUretici(_QueryModel->record(var).value("ureticiler.ad").toString());
        kart.setTedarikci(_QueryModel->record(var).value("carikartlar.ad").toString());
        kart.setAciklama(_QueryModel->record(var).value("aciklama").toString());
        _kartlar.append(kart);
    }
}

int StokKartlariModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _kartlar.count();
}

int StokKartlariModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _nColumn;
}

QVariant StokKartlariModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() == false) return QVariant();
    if(_kartlar.count() == 0) return QVariant();
//    const StokKarti kart = _kartlar.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        if(_kartlar.count() == 0) return QVariant();

        switch (index.column()) {
        case 0:
            return QVariant(_kartlar.at(index.row()).getId());
        case 1:
            return QVariant(_kartlar.at(index.row()).getBarkod());
        case 2:
            return QVariant(_kartlar.at(index.row()).getKod());
        case 3:
            return QVariant(_kartlar.at(index.row()).getAd());
        case 4:
            return QVariant(_kartlar.at(index.row()).getBirim());
        case 5:
            return QVariant(_kartlar.at(index.row()).getMiktar());
        case 6:
            return QVariant(_kartlar.at(index.row()).getGrup());
        case 7:
            return QVariant(QString::number(_kartlar.at(index.row()).getAFiyat(), 'f', 2));
        case 8:
            return QVariant(QString::number(_kartlar.at(index.row()).getSFiyat(), 'f', 2));
        case 9:
            return QVariant("%" + QString::number(_kartlar.at(index.row()).getKdv()));
        case 10:
            return QVariant("%" + QString::number(_kartlar.at(index.row()).getOtv()));
        case 11:
            if(_kartlar.at(index.row()).getKdvdahil()){
                return QVariant("Evet");
            }
            else{
                return QVariant("Hayır");
            }
        case 12:
            if(_kartlar.at(index.row()).getOtvdahil()){
                return QVariant("Evet");
            }
            else{
                return QVariant("Hayır");
            }
        case 13:
            return QVariant(_kartlar.at(index.row()).getTarih().toString("dd.MM.yyyy hh:mm"));
        case 14:
            return QVariant(_kartlar.at(index.row()).getUretici());
        case 15:
            return QVariant(_kartlar.at(index.row()).getTedarikci());
        case 16:
            return QVariant(_kartlar.at(index.row()).getAciklama());
        default:
            return QVariant();
        }
    case Qt::BackgroundRole:
        if(_takipAktifMi){
            if(index.column() == 5){
                if(index.data().toDouble() < _stokUyariMiktari){
                    return QBrush(Qt::red);
                }
            }
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
    int rowMax = _kartlar.count();
    if(row < rowMax && row >= 0 && column < _nColumn && column >= 0){
        return createIndex(row, column);
    }
    return QModelIndex();
}
