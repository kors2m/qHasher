#ifndef HASHFILEMODEL_H
#define HASHFILEMODEL_H

#include <QAbstractTableModel>

#include "ealghash.h"

class Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    int appendFile(const QString &filename, const QString &filepath);
    void addHash(int rowFileRecord, EAlgHash::Algs alg, const QString &hash);
    void removeAllRows();

    enum Column {
        FILENAME = 0,
        PATH,
        CRC32,
        MD4,
        MD5,
        LAST // указывает на кол-во столбцов
    };

signals:
    void addedFile(int idxRow);
    void dataChanged(const QModelIndex &index);

private:
    typedef QHash< Column, QVariant > FileData;
    typedef QList< FileData > Files;

    Files h_files;
};

#endif // HASHFILEMODEL_H
