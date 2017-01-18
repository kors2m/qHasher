#ifndef HASHER_H
#define HASHER_H

#include <QCryptographicHash>

#include "file.h"


class Hasher : public QObject
{
public:
    Hasher();

    QString calcHashes(File *file);

private:

};

#endif // HASHER_H
