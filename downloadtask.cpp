#include "downloadtask.h"
#include "QDebug"

DownloadTask::DownloadTask(const QString &url)
{
}

void DownloadTask::run()
{
    qDebug() << "Download task";

    emit done();
}
