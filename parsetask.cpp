#include <QDebug>
#include <QSharedPointer>
#include <QFile>
#include <QDataStream>
#include <QDateTime>

#include "parsetask.h"

ParseTask::ParseTask()
{
}

void ParseTask::run(QObject *result)
{
    QString *filePath = reinterpret_cast<QString *>(result);
    if (filePath == NULL) {
        qDebug() << "No source file";
        delete filePath;
        emit done(NULL);
        return;
    }
    QSharedPointer<QFile> localFile = QSharedPointer<QFile>(new QFile(*filePath));
    delete filePath;

    if (!localFile.data()->open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open file for reading";
        emit done(NULL);
        return;
    }

    QDataStream in(localFile.data());
    // header
    qint32 size;
    qint32 box;
    int headerSize = sizeof(size) + sizeof(box);

    while (!in.atEnd())
    {
        in >> size;
        in >> box;

        char boxType[4] = {((char *)&box)[3], ((char *)&box)[2], ((char *)&box)[1], ((char *)&box)[0]};
        qDebug() << qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.z")) << "Found box of type" << boxType << "and size" << size;

        if (box == ParseTask::MOOF || box == ParseTask::TRAF)
        {

        }

        qDebug() << in.skipRawData(size - headerSize);
    }

    emit done(NULL);
}
