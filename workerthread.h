#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QPoint>

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    WorkerThread(QObject *parent = nullptr);
    virtual ~WorkerThread() override;
    void run() override;
signals:
    void newPosition(const QPoint &p);

private:
    QPoint lastPosition;

    static const int poll_duration_ms;
};


#endif // WORKERTHREAD_H
