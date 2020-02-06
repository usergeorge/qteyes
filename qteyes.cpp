#include "qteyes.h"
#include "ui_qteyes.h"
#include "workerthread.h"




QtEyes::QtEyes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtEyes),
    workerThread(new WorkerThread(this))
{
//    setStyleSheet("background-color: rgba(0,0,0,0)");
    // or
//    setWindowFlags(Qt::FramelessWindowHint);
//    setAttribute(Qt::WA_NoSystemBackground);
//    setAttribute(Qt::WA_TranslucentBackground);
//    setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->setupUi(this);



    // print mouse position in a row
//    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}


QtEyes::~QtEyes()
{
    delete ui;
    delete workerThread;
}
