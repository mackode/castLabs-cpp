#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H

#include <QObject>
#include "task.h"

class DownloadTask : public Task
{
    Q_OBJECT
public:
    explicit DownloadTask(const QString &url);

signals:

public slots:
    void run();

};

#endif // DOWNLOADTASK_H
