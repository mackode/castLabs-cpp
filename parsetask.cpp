#include <iostream>
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
        std::cout << "No source file" << std::endl;
        delete filePath;
        emit done(NULL);
        return;
    }
    QSharedPointer<QFile> localFile = QSharedPointer<QFile>(new QFile(*filePath));
    delete filePath;

    if (!localFile.data()->open(QIODevice::ReadOnly))
    {
        std::cout << "Unable to open file for reading" << std::endl;
        emit done(NULL);
        return;
    }

    QDataStream in(localFile.data());
    // header
    qint32 size;
    qint32 box;
    int headerSize = sizeof(size) + sizeof(box);
    int readBufferSize = 1024;

    while(!in.atEnd())
    {
        in >> size;
        in >> box;

        char boxType[5] = {((char *)&box)[3], ((char *)&box)[2], ((char *)&box)[1], ((char *)&box)[0], '\0'};
        std::cout << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.z").toStdString() << " Found box of type " << boxType << " and size " << size << std::endl;

        int skipped = -1;
        if(box == ParseTask::MOOF || box == ParseTask::TRAF)
        {
            continue;
        }
        else if(box == ParseTask::MDAT)
        {
            std::cout << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.z").toStdString() << " Content of mdat box is: ";

            // if large shouldn't be read at once into the memory
            qint32 toRead = size;
            while(toRead > 0 && !in.atEnd())
            {
                QByteArray buffer(readBufferSize, Qt::Uninitialized);
                int read = in.readRawData(buffer.data(), readBufferSize);
                toRead -= read;
                if(read < 0)
                {
                    std::cout << "Malformed source file" << std::endl;
                    emit done(NULL);
                    return;
                }

                QString content(buffer);
                std::cout << qUtf8Printable(content);
            }

            skipped = size;
            std::cout << std::endl;
        }
        else
        {
            skipped = in.skipRawData(size - headerSize);
        }

        if(skipped < 0)
        {
            std::cout << "Malformed source file" << std::endl;
            emit done(NULL);
            return;
        }
    }

    emit done(NULL);
}
