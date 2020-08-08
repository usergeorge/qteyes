#include "qteyes.h"
#include "ui_qteyes.h"
#include "workerthread.h"
#include <QMenu>
#include <QMouseEvent>
#include <QDebug>



QtEyes::QtEyes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtEyes),
    workerThread(new WorkerThread(this))
{
    /* make the eyes transparent */
//    setStyleSheet("background-color: rgba(0,0,0,0)");
    // or
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->setupUi(this);


    // tell eyeball the position of the mouse pointer
    connect(workerThread, &WorkerThread::newPosition, ui->lefteye, &Eyeball::newPosition);
    connect(workerThread, &WorkerThread::newPosition, ui->righteye, &Eyeball::newPosition);

    workerThread->start();
}


QtEyes::~QtEyes()
{
    delete ui;
    delete workerThread;
}


void QtEyes::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action_exit("Exit", this);
    connect(&action_exit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()), Qt::QueuedConnection);
    contextMenu.addAction(&action_exit);

    contextMenu.exec(mapToGlobal(pos));
}

void QtEyes::mousePressEvent(QMouseEvent *event)
{
//    qDebug() << "mouse press " << event;
    if (Qt::LeftButton == event->button()) {
        inMoveLocalPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);

//        QPoint upper_left_window_point = event->globalPos() - inMoveLocalPoint;
//        qDebug() << "window position " << upper_left_window_point;
    }
}

void QtEyes::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug() << "mouse move " << event;

    QPoint upper_left_window_point = event->globalPos() - inMoveLocalPoint;
//    qDebug() << "window position " << upper_left_window_point;
    move(upper_left_window_point);
}

void QtEyes::mouseReleaseEvent(QMouseEvent *event)
{
//    qDebug() << "mouse release " << event;
    setCursor(Qt::ArrowCursor);
}
