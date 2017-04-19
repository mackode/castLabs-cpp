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

    while(!in.atEnd())
    {
        in >> size;
        in >> box;

        char boxType[5] = {((char *)&box)[3], ((char *)&box)[2], ((char *)&box)[1], ((char *)&box)[0], '\0'};
        qDebug() << qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.z")) << "Found box of type" << boxType << "and size" << size;

        int skipped = -1;
        if(box == ParseTask::MOOF || box == ParseTask::TRAF)
        {
            continue;
        }
        else if(box == ParseTask::MDAT)
        {
            QByteArray buffer(size, Qt::Uninitialized);
            in.readRawData(buffer.data(), size);
            QString content(buffer);
            qDebug() << qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.z")) << "Content of mdat box is:" << qPrintable(content);
            skipped = in.skipRawData(size - headerSize);
        }
        else
        {
            skipped = in.skipRawData(size - headerSize);
        }

        if(skipped < 0)
        {
            qDebug() << "Malformed source file";
            emit done(NULL);
            return;
        }
    }

    emit done(NULL);
}
