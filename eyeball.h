#ifndef EYEBALL_H
#define EYEBALL_H

#include <QWidget>

class Eyeball : public QWidget
{
    Q_OBJECT
public:
    explicit Eyeball(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // EYEBALL_H
