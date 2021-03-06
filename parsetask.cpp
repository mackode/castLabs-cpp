#include <iostream>
#include <QSharedPointer>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <QXmlStreamReader>
#include <QImage>
#include <QXmlStreamAttribute>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>

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

    QFileInfo appDir(QCoreApplication::applicationDirPath());
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
            QXmlStreamReader reader;

            // if large shouldn't be read at once into the memory
            qint32 toRead = size - headerSize;
            while(toRead > 0 && !in.atEnd())
            {
                QByteArray buffer(toRead < readBufferSize ? toRead : readBufferSize, Qt::Uninitialized);
                int read = in.readRawData(buffer.data(), toRead < readBufferSize ? toRead : readBufferSize);

                toRead -= read;
                if(read < 0)
                {
                    std::cout << "Malformed source file. Cannot read data." << std::endl;
                    emit done(NULL);
                    return;
                }

                QString content = QString::fromUtf8(buffer);
                std::cout << qUtf8Printable(content);
                reader.addData(content);
            }

            skipped = size;
            std::cout << std::endl;

            // smpte:image
            while (!reader.atEnd()) {
                reader.readNext();

                if(reader.name() == "image")
                {
                    QString filename = "no_file_name";
                    QXmlStreamAttributes attributes = reader.attributes();
                    // change to iterator
                    for (int i = 0; i < attributes.size(); ++i) {
                        QXmlStreamAttribute attribute = attributes[i];
                        if(attribute.name().contains("id"))
                        {
                            filename = attribute.value().toString() + ".png";
                            break;
                        }
                    }

                    QImage image;
                    image.loadFromData(QByteArray::fromBase64(reader.readElementText().toLocal8Bit()), "PNG");
                    image.save(appDir.path() + QDir::separator() + filename, "PNG");

                    std::cout << "extracted image " << filename.toStdString() << std::endl;
                }
            }

            if (reader.hasError()) {
                std::cout << "Malformed source file. XML image parsing error." << std::endl;
                emit done(NULL);
                return;
            }
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

    // flush file
    localFile.data()->close();
    emit done(NULL);
}
