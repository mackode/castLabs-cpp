#include <QCoreApplication>
#include <QSharedPointer>
#include <QTimer>
#include <QLoggingCategory>

#include "downloadtask.h"
#include "parsetask.h"
#include "extracttask.h"

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");
    QCoreApplication a(argc, argv);
    QString filename = QString("text0.mp4");

    QSharedPointer<DownloadTask> downloadTask = QSharedPointer<DownloadTask>(new DownloadTask("http://demo.castlabs.com/tmp/" + filename));
    QSharedPointer<ParseTask> parseTask = QSharedPointer<ParseTask>(new ParseTask());
    QSharedPointer<ExtractTask> extractTask = QSharedPointer<ExtractTask>(new ExtractTask());

    // chain tasks signal & slots
    QTimer::singleShot(0, downloadTask.data(), SLOT(run()));
    QObject::connect(downloadTask.data(), SIGNAL(done(QObject *)), parseTask.data(), SLOT(run(QObject *)));
    QObject::connect(parseTask.data(), SIGNAL(done(QObject *)), extractTask.data(), SLOT(run(QObject *)));
    QObject::connect(extractTask.data(), SIGNAL(done(QObject *)), &a, SLOT(quit()));

    return a.exec();
}
