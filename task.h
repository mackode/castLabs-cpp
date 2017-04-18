#ifndef TASK_H
#define TASK_H

#include <QObject>

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(QObject *parent = 0);

signals:
    void done();

public slots:
    virtual void run() = 0;

};

#endif // TASK_H
