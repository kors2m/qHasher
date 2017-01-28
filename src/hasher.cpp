#include "hasher.h"

const int Hasher::kSizeBuffer = 2048;

Hasher::Hasher(Model *model)
{
    this->model = model;
}

void Hasher::addFile(File *file, QList <EAlgHash::Algs> markAlgs)
{
    hFile = file;
    selectedAlgs = markAlgs;
}

void Hasher::calcHashes()
{
    int rowIdx = model->appendFile(hFile->getFilename(), hFile->getPath());
    QString result;
    int cnt = 0;

    while (!selectedAlgs.isEmpty()) {
        auto alg = selectedAlgs.takeFirst();
        result = calcHash(hFile, alg);

        if (QThread::currentThread()->isInterruptionRequested()) {
            emit interrupted();
            return;
        }

        model->addHash(rowIdx, alg, result);
        emit valChangedProgressBar(++cnt);
    }
    emit finished();
}

QString Hasher::calcHash(File *file, EAlgHash::Algs algorithm)
{
    QString hash;
    switch (algorithm) {
    case EAlgHash::CRC32:
        hash = calcCRC32(file);
        break;
    case EAlgHash::MD4:
        hash = calcHash(file, QCryptographicHash::Md4);
        break;
    case EAlgHash::MD5:
        hash = calcHash(file, QCryptographicHash::Md5);
        break;
    }
    return hash;
}

QString Hasher::calcHash(File *file, QCryptographicHash::Algorithm algorithm)
{
    QCryptographicHash hash(algorithm);
    QString result;
    char buf[kSizeBuffer];
    qint64 readBytes;
    qint64 readResult = 0;

    while ((readBytes = file->read(buf, kSizeBuffer)) > 0) {
        readResult += readBytes;
        hash.addData(buf, readBytes);

        if (QThread::currentThread()->isInterruptionRequested()) {
            file->close();
            return "";
        }
    }
    file->close();

    result = hash.result().toHex().constData();
    return result;
}

QString Hasher::calcCRC32(File *file)
{
    char buf[kSizeBuffer];
    qint64 readBytes;
    qint64 readResult = 0;
    boost::crc_32_type hash;

    while ((readBytes = file->read(buf, kSizeBuffer)) > 0) {
        readResult += readBytes;
        hash.process_bytes(buf, readBytes);
        if (QThread::currentThread()->isInterruptionRequested()) {
            file->close();
            return "";
        }
    }
    file->close();

    qulonglong crc = static_cast<qulonglong>(hash.checksum());
    // Convert result from base 10 (Decimal) to base 16 (Hex)
    QByteArray result = QByteArray::number(crc, 16);
    return result;
}
