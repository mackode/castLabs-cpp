#ifndef PARSETASK_H
#define PARSETASK_H

#include <QObject>
#include "task.h"

class ParseTask : public Task
{
    Q_OBJECT
public:
    explicit ParseTask(const QString &path);

signals:

public slots:
    void run();

};

#endif // PARSETASK_H
