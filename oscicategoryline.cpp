#include "oscicategoryline.h"
#include "oscichart.h"
#include <QObject>
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtCharts/QChart>
#include "oscilloscope.h"

OsciCategoryLine::OsciCategoryLine(qreal val,
                 Qt::Orientation orientaton,
                 Oscilloscope* osci,
                 OsciChart *parent)
{    
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
    if(isSelected())
        pen.setColor(Qt::cyan);
    painter->setPen(pen);
    QPointF pt2 = getPoint2();
    painter->drawLine(getPoint1(), pt2);

    if(!m_label.isEmpty())
    {
        QPointF pt2_;
        pt2_.setX(pt2.x() - (( qreal(m_label.size()) * 9.0) / 2.0 ));
        pt2_.setY(pt2.y() - 5.0);
        painter->drawText(pt2_, m_label);
        QPointF pt1 = getPoint1();
        pt1.setX(pt1.x() - (((qreal)m_label.size()*9.0) /2.0 ));
        pt1.setY(pt1.y() + 12.0);
        painter->drawText(pt1, m_label);

    }
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
    else
    {
        qreal ymin = m_chart->mapToValue(m_chart->plotArea().bottomLeft()).y();
        start = m_chart->mapToPosition(QPointF(m_x,ymin));
    }
    QPointF startItem = mapFromScene(start);
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
    else
    {
        qreal ymax = m_chart->mapToValue(m_chart->plotArea().topLeft()).y();
        start = m_chart->mapToPosition(QPointF(m_x, ymax));
    }
    QPointF startItem = mapFromScene(start);
    return startItem;
}

QRectF OsciCategoryLine::boundingRect() const
{
    QPointF topLeft = getPoint2();
    topLeft.setX(topLeft.x() - 2.5);
    QPointF bottomRight = getPoint1();
    bottomRight.setX(bottomRight.x() + 2.5);
    return QRectF(getPoint1(),getPoint2());
}

void OsciCategoryLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
}

void OsciCategoryLine::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
}


void OsciCategoryLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void OsciCategoryLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void OsciCategoryLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}
