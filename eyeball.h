#ifndef EYEBALL_H
#define EYEBALL_H

#include <QWidget>

class Eyeball : public QWidget
{
    Q_OBJECT
public:
    explicit Eyeball(QWidget *parent = nullptr);
    virtual ~Eyeball() Q_DECL_OVERRIDE;

public slots:
    void newPosition(const QPoint &p);

protected:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QPoint mousePosition;
    static const int eyeline_size;
    static const int paint_widget_size;
    static const int iris_size;
};

#endif // EYEBALL_H
