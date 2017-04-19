#include "extracttask.h"

ExtractTask::ExtractTask()
{

}

void ExtractTask::run(QObject *result)
{
    emit done(NULL);
}
