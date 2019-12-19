#ifndef QTEYES_H
#define QTEYES_H

#include <QMainWindow>

namespace Ui {
class QtEyes;
}

class QtEyes : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtEyes(QWidget *parent = nullptr);
    ~QtEyes();

private:
    Ui::QtEyes *ui;
};

#endif // QTEYES_H
