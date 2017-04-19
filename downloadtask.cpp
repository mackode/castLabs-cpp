#include <QDebug>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>

#include "downloadtask.h"

DownloadTask::DownloadTask(const QString &url)
    : networkManager(new QNetworkAccessManager(this)),
      url(url)
{
    connect(networkManager.data(), SIGNAL(finished(QNetworkReply *)), this, SLOT(finished(QNetworkReply *)));
}

void DownloadTask::run(QObject *result)
{
    QFileInfo fileInfo(url);
    QFileInfo appDir(QCoreApplication::applicationDirPath());

    localFile = QSharedPointer<QFile>(new QFile(appDir.path() + QDir::separator() + fileInfo.fileName()));
    if (!localFile.data()->open(QIODevice::WriteOnly))
    {
        qDebug() << "Unable to open file for writing";
        emit done(NULL);
        return;
    }

    reply = QSharedPointer<QNetworkReply>(networkManager.data()->get(QNetworkRequest(QUrl(url))));
    connect(reply.data(), SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(reply.data(), SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

void DownloadTask::readyRead()
{
    localFile.data()->write(reply.data()->readAll());
}

void DownloadTask::error(QNetworkReply::NetworkError err)
{
    qDebug() << "Error " << reply.data()->errorString();
}

void DownloadTask::finished(QNetworkReply *reply)
{
    qDebug() << qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.z")) << "Successfully loaded file" << qPrintable(url);
    emit done((QObject *)new QString(localFile.data()->fileName()));
}
