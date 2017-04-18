#include <QDebug>
#include "downloadtask.h"

DownloadTask::DownloadTask(const QString &url)
    : networkManager(new QNetworkAccessManager(this)),
      url(url)
{
    connect(networkManager.data(), SIGNAL(finished(QNetworkReply *)), this, SLOT(finished(QNetworkReply *)));
}

void DownloadTask::run()
{
    reply = QSharedPointer<QNetworkReply>(networkManager.data()->get(QNetworkRequest(QUrl(url))));
    connect(reply.data(), SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(reply.data(), SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

}

void DownloadTask::readyRead()
{
    qDebug() << "Read";
}

void DownloadTask::error(QNetworkReply::NetworkError err)
{
    qDebug() << "Error " << reply.data()->errorString();
}

void DownloadTask::finished(QNetworkReply *reply)
{
    qDebug() << "Download task";
    emit done();
}
