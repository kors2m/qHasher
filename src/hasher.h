#ifndef HASHER_H
#define HASHER_H

#include <QCryptographicHash>
#include <QMessageBox>
#include <QThread>

#include <boost/crc.hpp>

#include "file.h"
#include "model.h"
#include "ealghash.h"

class Hasher : public QObject
{
    Q_OBJECT
public:
    explicit Hasher(Model *model);
    void addFile(File *file, QList<EAlgHash::Algs> markAlgs);

    static const int kSizeBuffer;

public slots:
    void calcHashes();

signals:
    void valChangedProgressBar(int value);
    void finished();
    void interrupted();

private:
    QString calcHash(File *file, EAlgHash::Algs algorithm);
    QString calcHash(File *file, QCryptographicHash::Algorithm algorithm);
    QString calcCRC32(File *file);

    void preparedListHashes();

    Model *model;
    File *hFile;
    QList <EAlgHash::Algs> selectedAlgs;
};

#endif // HASHER_H
