#include "workerthread.h"
#include <QCursor>
#include <qdebug.h>


/* sleep duration between mouse position polls */
const int WorkerThread::poll_duration_ms = 100;



WorkerThread::WorkerThread(QObject *parent)
    : QThread(parent),
      lastPosition(-1,-1)
{
}

WorkerThread::~WorkerThread()
{
    requestInterruption();
    quit();
    wait(1010);
}

/* poll the global mouse position.
 * cache and emit the value to the eye widget */
void WorkerThread::run()
{
    while( ! isInterruptionRequested() ) {
        QPoint mousepoint = QCursor::pos();
        if (mousepoint != lastPosition) {
//            qDebug() << "mouse point " << mousepoint;
            lastPosition = mousepoint;
            emit newPosition(mousepoint);
        }
        msleep(poll_duration_ms);
    }
}
