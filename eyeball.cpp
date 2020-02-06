#include "eyeball.h"

#include <QMouseEvent>
#include <qdebug.h>

Eyeball::Eyeball(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
}



void Eyeball::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mousepoint = event->globalPos();
    qDebug() << "mouse point " << mousepoint << endl;
}
