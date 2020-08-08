#ifndef QTEYES_H
#define QTEYES_H

#include <QMainWindow>

namespace Ui {
class QtEyes;
}
class WorkerThread;

class QtEyes : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtEyes(QWidget *parent = nullptr);
    ~QtEyes();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void showContextMenu(const QPoint &pos);

private:
    Ui::QtEyes *ui;
    WorkerThread *workerThread;

    QPoint inMoveLocalPoint;
};

#endif // QTEYES_H
