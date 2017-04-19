#ifndef PARSETASK_H
#define PARSETASK_H

#include <QObject>

#include "task.h"

class ParseTask : public Task
{
    Q_OBJECT

private:
    // contains other boxes
    static const qint32 MOOF = 0x6d6f6f66;
    static const qint32 TRAF = 0x74726166;
    static const qint32 MDAT = 0x6d646174;

public:
    explicit ParseTask();

signals:

public slots:
    void run(QObject *result);

};

#endif // PARSETASK_H
