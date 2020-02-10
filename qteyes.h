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

private slots:
    void showContextMenu(const QPoint &pos);

private:
    Ui::QtEyes *ui;
    WorkerThread *workerThread;
};

#endif // QTEYES_H
