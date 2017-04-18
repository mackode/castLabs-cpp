#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QSharedPointer>
#include <QNetworkReply>
#include <QFile>

#include "task.h"

class DownloadTask : public Task
{
    Q_OBJECT

private:
    QSharedPointer<QNetworkAccessManager> networkManager;
    QString url;
    QSharedPointer<QNetworkReply> reply;
    QSharedPointer<QFile> localFile;

public:
    explicit DownloadTask(const QString &url);

signals:

public slots:
    void run();

    void readyRead();
    void error(QNetworkReply::NetworkError err);
    void finished(QNetworkReply *reply);

};

#endif // DOWNLOADTASK_H
