#include "parsetask.h"
#include "QDebug"

ParseTask::ParseTask(const QString &path)
{
}

void ParseTask::run(QObject *result)
{
    qDebug() << "Parse task";

    emit done(NULL);
}
