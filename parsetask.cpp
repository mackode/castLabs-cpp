#include "parsetask.h"
#include "QDebug"

ParseTask::ParseTask(const QString &path)
{
}

void ParseTask::run()
{
    qDebug() << "Parse task";

    emit done();
}
