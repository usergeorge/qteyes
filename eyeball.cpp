#include "eyeball.h"

#include <QPainter>
#include <qdebug.h>
#include <math.h>


const int Eyeball::eyeline_size = 10;
const int Eyeball::paint_widget_size = 120;
const int Eyeball::iris_size = Eyeball::eyeline_size-2;
const int Eyeball::iris_radius_reduction = 3*Eyeball::eyeline_size;

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
//    qDebug() << this << "mouse pos " << p;
    mousePosition = p;
    /* repaint the whole screen */
    update();   // slow screen update, optimized for multi updates
//    repaint();   // repaint the screen immediately, may flicker
}



/* paint the eye line and the iris.
 * first get the rectangle of this widget.
 * then expand to size to quadratic form and calculate
 * the expansion factor as real number.
 * (move the expanded rectangle to match the center
 * of the non expanded one.)
 * Draw a circle and the iris in the rectangle, then
 * shrink the rectangle by the expansion factor to its
 * original size.
 */
void Eyeball::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter p(this);
    QPen pen = p.pen();
    pen.setWidth(eyeline_size);
    p.setPen(pen);
//    p.setBackgroundMode(Qt::TransparentMode);
    p.setRenderHint(QPainter::Antialiasing, true);

#if 0
    /* paint the eye into the available space.
     * get the size of the widget, reduce by 1,
     * reduce the width of the pen
     * and paint the circle */
    QRect r = rect();
    r.adjust(eyeline_size/2, eyeline_size/2,  -eyeline_size/2 -1, -eyeline_size/2 -1);
    QSize r_size = r.size();
    const QPoint origcenter = r.center();
    const QPoint centermove = makeMaxSquareCenter(r);
    QRect r2 = rect();
    r2.adjust(eyeline_size/2, eyeline_size/2,  -eyeline_size/2 -1, -eyeline_size/2 -1);
//    const QMatrix squarematrix = makeMaxSquare(r2);
    const QTransform squareTransform = makeMaxSquare(r2);


#if 0
    double width = r_size.width();
    double heigth = r_size.height();
    double scale_x = centermove.x() / width;
    double scale_y = centermove.y() / heigth;
    p.scale(1.0 - scale_x,
            1.0 - scale_y);
    p.translate(centermove.x()/2, centermove.y()/2);
#else
//    const QTransform transform = p.worldTransform();
//    const QTransform squareTransform(squarematrix);
    p.setTransform(squareTransform);
#endif
    p.drawEllipse(r);
#else

    /* get the paint rectangle of the widget.
     * set new rectange of quadratic size.
     * create a transformation matrix to correct the
     * new square rectangle */
    const QRect widget_rect = rect();
    QRect paint_rect(widget_rect.left(), widget_rect.top(), paint_widget_size, paint_widget_size);

    /* prepare transformation matrix for the eye line */
    QTransform squareTransform;
    QSizeF rect_new_size = widget_rect.size();
    rect_new_size /= paint_rect.width();
    squareTransform.scale(rect_new_size.width(), rect_new_size.height());
    p.setTransform(squareTransform);

    /* adjust (shrink) the painting square for the bigger pen size */
    const int paintrect_correction_topleft = eyeline_size/2;
    const int paintrect_correction_bottomright = -eyeline_size/2 -1;
    paint_rect.adjust(paintrect_correction_topleft, paintrect_correction_topleft, paintrect_correction_bottomright, paintrect_correction_bottomright);


    QPainterPath circle_painter_path;
    circle_painter_path.addEllipse(paint_rect);
    /* draw white eyeball */
    p.fillPath(circle_painter_path, Qt::white);
    /* draw black eye line */
    p.drawPath(circle_painter_path);


#if 0
    /* draw the iris
     *
     * get the center of the this widget.
     * paint a line between the center point and
     * the mouse position.
     */
    const QPoint widget_center = widget_rect.center();
    const QPoint mouse_center = mousePosition;
    const QPoint diff_center = mouse_center - widget_center;
//    double invarc = diff_center.y();
//    invarc /= diff_center.x();
    double arc = atan2( diff_center.y(), diff_center.x() );
    double deg_arc = arc * 180;
    deg_arc /= M_PI;

//    squareTransform.rotate(arc);

//    p.setTransform(squareTransform, false);

////    p.drawLine(widget_center, mouse_center);
//    p.drawLine(0, 0, widget_center.x(), 0);


//    /* iris is painted at a 0° rotation.
//     * first rotate the ball to the angle which is
//     * interpolated between the mouse pointer and the
//     * widget center and then
//     * move its center at [0,0] to the correct center
//     * at widget.center() */
//    QTransform iris_transform;
////    QPointF painter_center = paint_rect.center();
////    iris_transform.translate(painter_center.x(), painter_center.y());
////    iris_transform.translate(dwidth/2, dheight/2);

//    /* move iris to upper left corner at (0,0) */
//    iris_transform.translate(-paintrect_correction_topleft, -paintrect_correction_topleft);
//    /* move iris with a 'looking radius' to the right (at 0°) */
//    iris_transform.translate(50, 0);

////    iris_transform.translate(paint_rect.width()/2, 0);
//    /* turn eye by N degree to meet the mouse pointer */
//    iris_transform.rotate(15);

//    QPointF iris_position = paint_rect.center();
//    iris_position -= QPointF(1,1);
//    iris_transform.translate(iris_position.x(), iris_position.y());

//    /* add the widget squeeze transformation from the window resize */
//    iris_transform *= squareTransform;
//    /* now apply */
//    p.setTransform(iris_transform);

#else


    /* we need to have the inverse transformation matrix
     * to calculate a virtual mouse position. This is to
     * get the real mouse position after applying the
     * paint transformation matrix. */
    bool is_paint_matrix_invertible;
    QTransform inverse_paint_matrix = squareTransform.inverted(&is_paint_matrix_invertible);
    if ( !is_paint_matrix_invertible ) {
        qDebug() << "error: can not invert the paint matrix " << squareTransform;
        exit(EXIT_FAILURE);
    }



    /* calculate new iris position.
     *
     * The iris follows the mouse pointer.
     * Either it is directly under the mouse pointer.
     * Or it is at the border of the circle of the maximal radius
     * in which case it points in a line from the center of the
     * eye line circle to the mouse pointer.    */
    const QPoint paint_rect_center_point = paint_rect.center();
//    const QPoint global_paint_center = mapToGlobal(paint_rect_center_point);
    /* get the arc between the widget center and the mouse pointer */
    const QPoint widget_center = widget_rect.center();
    const QPoint global_widget_center = mapToGlobal(widget_center);
    const QPoint global_mouse_point = mousePosition;
//    const QPoint global_mouse_point = inverse_paint_matrix.map(QPoint(100,100) + global_paint_center);  // test code
//    const QPoint global_mouse_point = inverse_paint_matrix.map(QPoint(0,100) + global_widget_center);  // test code
//    const QPoint global_mouse_point = (QPoint(0,100) + global_widget_center);  // test code
//    const QPoint global_mouse_point = inverse_paint_matrix.map(mousePosition);
//    const QPoint diff_center = mouse_center - widget_center;
//    const QPoint diff_center = global_mouse_point - global_widget_center;
    const QPoint diff_center = inverse_paint_matrix.map(global_mouse_point - global_widget_center);

//    const double arc = atan2( diff_center.y(), diff_center.x() );
//        // this is only test code, calculate the arc in degree
//        double darc = arc * 180;
//        darc /= M_PI;

    /* now we got the arc lets calculate the radius length
     * to the mouse pointer */
#if 0
    const double cos_arc = cos(arc);
    double radius = diff_center.x();
    radius /= cos_arc;
#else
//    double r1 = (diff_center.x()*diff_center.x());
//    double r2 = (diff_center.y()*diff_center.y());
    double radius = sqrt((diff_center.x()*diff_center.x()) + (diff_center.y()*diff_center.y()));
//    double radius = sqrt(r1 + r2);
#endif
    /* if radius is too much, we need to clip it.
     * Clip it to the radius of the eye line
     * minus pen width
     * minus some pixel */
    const double max_radius = paint_rect_center_point.x() - iris_radius_reduction;
    double length_correction_factor = 1.0;
    if (radius > max_radius) {
        length_correction_factor = max_radius / radius;
        radius = max_radius;
    }

    const QPoint iris_movement = diff_center * length_correction_factor;
//    const QPoint iris_movement = QPoint(0, 0); // test
//    const QPoint iris_movement = QPoint(-paint_widget_size/2, -paint_widget_size/2); // test
    const QPoint iris_in_center = paint_rect_center_point - QPoint(iris_size, iris_size)/2;
    const QPoint iris_upper_left_point = iris_in_center + iris_movement;

    /* note: a quite sound looking combination of rectangular
     * size and pen width produces a blind spot (white pixel)
     * in the center of the iris. Be caucious before changing
     * these values */
//    QRect iris_rect(0, 0, eyeline_size-2, eyeline_size-2);
////    const QRect iris_rect(paint_rect.width()/2, 0, eyeline_size-2, eyeline_size-2);
////    const QRect iris_rect(width/2 + paint_rect.width()/2, height/2, eyeline_size-2, eyeline_size-2);
////    QPoint iris_position = paint_rect.center();
////    iris_position -= QPoint(1,1);
////    const QRect iris_rect(iris_position, QSize(eyeline_size-2, eyeline_size-2));
//    iris_rect.moveTo(iris_center_point);
    QRect iris_rect(iris_upper_left_point, QSize(iris_size, iris_size));

//    const QPoint iris_widget_upper_left_point = squareTransform.map(iris_upper_left_point); // test code

    pen.setWidth(iris_size);
    p.setPen(pen);

//    QTransform iris_transformation;
////    iris_transformation.translate(-paint_widget_size/2, -paint_widget_size/2);
//    iris_transformation.rotate(30);
//    iris_transformation.translate(+paint_widget_size/2, +paint_widget_size/2);
//    p.setTransform(iris_transformation);

    p.drawEllipse(iris_rect);

#endif

#endif

//    /* draw the iris */
//    r = rect();
//    // get the center of the eye
//    QPoint center = r.center();
//    // get the arc from the center to the mouse position
//    QPoint relativeMousePosition = center - mousePosition;

//    /* get the inner box of the eye where width and height are equal.
//     * paint the iris on arc = 0, then turn the box with arc. Now the
//     * iris looks to the mouse pointer.
//     */
//    double fx = relativeMousePosition.x();
//    double fy = relativeMousePosition.y();
//    double arc = ::tan( fy/fx );

//    r.adjust(eyeline_size, eyeline_size,  -eyeline_size -1, -eyeline_size -1);

//    p.drawRect(r);

}

