#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QPoint>

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    WorkerThread(QObject *parent = nullptr)
        : QThread(parent)
    {}
    virtual ~WorkerThread() override { requestInterruption(); quit(); wait(1010); }
    void run() override;
//signals:
//    void resultReady(const QString &s);
private:
    QPoint lastPosition;
};


#endif // WORKERTHREAD_H
