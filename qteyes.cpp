#include "qteyes.h"
#include "ui_qteyes.h"
#include "workerthread.h"




QtEyes::QtEyes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtEyes),
    workerThread(new WorkerThread(this))
{
    /* make the eyes transparent */
//    setStyleSheet("background-color: rgba(0,0,0,0)");
    // or
//    setWindowFlags(Qt::FramelessWindowHint);
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
