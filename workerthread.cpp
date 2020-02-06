#include "workerthread.h"
#include <QCursor>
#include <qdebug.h>



void WorkerThread::run()  {
    while( ! isInterruptionRequested() ) {
        QPoint mousepoint = QCursor::pos();
        qDebug() << "mouse point " << mousepoint << endl;
        msleep(100);
    }
}
