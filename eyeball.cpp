#include "eyeball.h"

#include <QPainter>
#include <qdebug.h>


const int Eyeball::eyeline_size = 10;


Eyeball::Eyeball(QWidget *parent)
    :QWidget(parent)
{
    setMouseTracking(true);
}

Eyeball::~Eyeball()
{
}

void Eyeball::newPosition(const QPoint &p)
{
    qDebug() << this << "mouse pos " << p;
    mousePosition = p;
}

void Eyeball::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter p(this);
    QPen pen = p.pen();
    pen.setWidth(eyeline_size);
    p.setPen(pen);

    /* paint the eye into the available space.
     * get the size of the widget, reduce by 1,
     * reduce the width of the pen
     * and paint the circle */
    QRect r = rect();
    r.adjust(eyeline_size/2, eyeline_size/2,  -eyeline_size/2 -1, -eyeline_size/2 -1);

    p.drawEllipse(r);
}
