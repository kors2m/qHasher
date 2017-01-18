#include <QTest>

#include "test/filetest.h"
#include "test/hashertest.h"

int main(int argc, char *argv[])
{
    FileTest test1;
    QTest::qExec(&test1, argc, argv);

    HasherTest test2;
    QTest::qExec(&test2, argc, argv);

    return 0;
}
