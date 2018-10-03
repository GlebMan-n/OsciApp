#include "oscicategoryline.h"
#include "oscichart.h"
#include <QObject>
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtCharts/QChart>
#include "oscilloscope.h"
#include <QDebug>
OsciCategoryLine::OsciCategoryLine(qreal val,
                 Qt::Orientation orientaton,
                 Oscilloscope* osci,
                 OsciChart *parent)
{
    m_drawText = 0;
    m_pressed = false;
    m_osci = osci;
    m_chart = parent;
    setOrientation(orientaton);
    if(m_orientation == Qt::Horizontal)
        setY(val);
    else
        setX(val);
    m_pen.setWidth(3.0);
    m_pen.setStyle(Qt::PenStyle::SolidLine);
    m_pen.setColor(Qt::green);
}

void OsciCategoryLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(m_pen);
    if(m_pressed)
        pen.setColor(Qt::red);
    painter->setPen(pen);
    QPointF pt2 = getPoint2();
    painter->drawLine(getPoint2(), getPoint1());
    if(!m_label.isEmpty() && m_drawText != -1 )
    {
        QPointF pt2_;
        pt2_.setX(pt2.x() - (( qreal(m_label.size()) * 9.0) / 2.0 ));
        pt2_.setY(pt2.y() - 5.0);
        if(m_drawText == 0 || m_drawText == 1)
            painter->drawText(pt2_, m_label);
        QPointF pt1 = getPoint1();
        pt1.setX(pt1.x() - (((qreal)m_label.size()*9.0) /2.0 ));
        pt1.setY(pt1.y() + 12.0);
        if(m_drawText == 0 || m_drawText == 2)
            painter->drawText(pt1, QString::number(m_chart->mapToValue(getPoint1()).x()));
    }
    pen.setColor(Qt::cyan);
    painter->setPen(pen);
    painter->drawRect(boundingRect());
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QPointF OsciCategoryLine::getPoint1() const
{
    if(!m_osci) return QPointF();
    QPointF start;

    if(getOrientation() == Qt::Horizontal)
    {
        qreal xmin = m_chart->mapToValue(m_chart->plotArea().bottomLeft()).x();
        start = m_chart->mapToPosition(QPointF(xmin,m_y));
    }    
    else if(getOrientation() == Qt::Vertical)
    {
        qreal ymin = m_chart->mapToValue(m_chart->plotArea().bottomLeft()).y();
        start = m_chart->mapToPosition(QPointF(m_x,ymin));
    }
    else
        return start;

    QPointF startItem = mapToScene(start);
    return startItem;
}

QPointF OsciCategoryLine::getPoint2() const
{
    if(!m_osci) return QPointF();
    QPointF start;
    if(getOrientation() == Qt::Horizontal)
    {
        qreal xmax = m_chart->mapToValue(m_chart->plotArea().topLeft()).x();
        start = m_chart->mapToPosition(QPointF(xmax,m_y));
    }
    else if(getOrientation() == Qt::Vertical)
    {
        QPointF tt = m_chart->plotArea().topLeft();
        qreal ymax = m_chart->mapToValue(tt).y();
        start = m_chart->mapToPosition(QPointF(m_x, ymax));
    }
    else
        return start;

    QPointF startItem = mapToScene(start);
    return startItem;
}

QRectF OsciCategoryLine::boundingRect() const
{
    QPointF topLeft = getPoint2();
    topLeft.setX(topLeft.x() - 10);
    QPointF bottomRight = getPoint1();
    bottomRight.setX(bottomRight.x() + 10);
    return QRectF(topLeft,bottomRight);
}

void OsciCategoryLine::redraw()
{
    this->update();
    m_chart->scene()->update(m_chart->scene()->sceneRect());
}

void OsciCategoryLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressed = false;
    qWarning() << "mouseReleaseEvent";
    redraw();
    event->accept();
}

void OsciCategoryLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressed = true;
    qWarning() << "mouseMoveEvent";
    event->accept();
}

void OsciCategoryLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_pressed)
        return;
    qWarning() << "mouseMoveEvent";
    /*QPointF valuePos = m_chart->mapToValue(event->scenePos());
    if(m_pressed && getOrientation() == Qt::Horizontal)
        setY(valuePos.y());
    if(m_pressed && getOrientation() == Qt::Vertical)
        setX(valuePos.x());*/
    QPointF ptf;
    QPointF valuePt = m_chart->mapToValue(event->pos());
    ptf.setX(m_chart->mapToPosition(valuePt).x());
    ptf.setY(this->pos().y());
    this->setPos(ptf);
    redraw();
    event->accept();
}

void OsciCategoryLine::setX(qreal x)
{
    qreal xmin = m_chart->mapToValue(m_chart->plotArea().bottomLeft()).x();
    if(x < xmin)
        m_x = xmin;
    else
    {
        qreal xmax = m_chart->mapToValue(m_chart->plotArea().topRight()).x();
        if(x >= xmax)
            m_x = xmax;
        else
            m_x = x;
    }
}

void OsciCategoryLine::setY(qreal y)
{
    qreal ymin = m_chart->mapToValue(m_chart->plotArea().bottomLeft()).y();
    if(y < ymin)
        m_y = ymin;
    else
    {
        qreal ymax = m_chart->mapToValue(m_chart->plotArea().topLeft()).y();
        y > ymax ? m_y = ymax : m_y = y;
    }
}

QPointF OsciCategoryLine::toScreen(const GraphicCoords& coords) const
{
    /*
     * Returns the value in the series
     * specified by series at the position specified by position in a chart.     *
     *  QPointF mapToValue(const QPointF &position, QAbstractSeries *series = nullptr);
     *
     * Returns the position on the chart that corresponds
     * to the value value in the series specified by series.
    QPointF mapToPosition(const QPointF &value, QAbstractSeries *series = nullptr);*/
    return m_chart->mapToValue(coords.toPointF());
}

GraphicCoords OsciCategoryLine::toGraphic(const QPointF &point) const
{
    QPointF pt = m_chart->mapToPosition(point);
    return GraphicCoords(pt);
}
