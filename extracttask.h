#ifndef EXTRACTTASK_H
#define EXTRACTTASK_H

#include <QObject>

#include "task.h"

class ExtractTask : public Task
{
    Q_OBJECT

public:
    explicit ExtractTask();

signals:

public slots:
    void run(QObject *result);

};

#endif // EXTRACTTASK_H
