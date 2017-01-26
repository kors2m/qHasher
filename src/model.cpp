#include "model.h"

Model::Model(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int Model::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return h_files.count();
}

int Model::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return LAST;
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || h_files.count() <= index.row()
        || role != Qt::DisplayRole)
        return QVariant();
    return h_files[index.row()][Column(index.column())];
}

bool Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole || h_files.count() <= index.row())
        return false;

    h_files[ index.row() ][ Column(index.column()) ] = value;
    emit dataChanged(index);

    return true;
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
        return section;

    switch (section) {
    case FILENAME:
        return tr("FileName");
    case PATH:
        return tr("Path");
    }

    return QVariant();
}

int Model::appendFile(const QString &filename, const QString &filepath)
{
    FileData file;
    file[ FILENAME ] = filename;
    file[ PATH ] = filepath;

    int row = h_files.count();
    beginInsertRows(QModelIndex(), row, row);
    h_files.append(file);
    endInsertRows();

    emit addedFile(row);

    return row;
}

void Model::addHash(int rowFileRecord, EAlgHash::Algs alg, const QString &hash)
{
    switch (alg) {
    case EAlgHash::CRC32:
        setData(index(rowFileRecord, Model::CRC32), hash, Qt::EditRole);
        break;
    case EAlgHash::MD4:
        setData(index(rowFileRecord, Model::MD4), hash, Qt::EditRole);
        break;
    case EAlgHash::MD5:
        setData(index(rowFileRecord, Model::MD5), hash, Qt::EditRole);
        break;
    }
}
