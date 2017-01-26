#ifndef EALGHASH_H
#define EALGHASH_H

#include <QObject>

class EAlgHash : public QObject
{
    Q_OBJECT

public:
    EAlgHash()
    {
    }

    enum Alg {
        CRC32 = 0x01,
        MD4 = 0x02,
        MD5 = 0x04
    };
    Q_DECLARE_FLAGS(Algs, Alg)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(EAlgHash::Algs)

#endif // EALGHASH_H
