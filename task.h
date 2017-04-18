#ifndef TASK_H
#define TASK_H

#include <QObject>

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(QObject *parent = 0);

signals:
    void done(QObject *result);

public slots:
    virtual void run(QObject *result = NULL) = 0;

};

#endif // TASK_H
