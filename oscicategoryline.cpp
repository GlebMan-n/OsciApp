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
    m_osci = osci;
    m_chart = parent;
    setOrientation(orientaton);
    if(m_orientation == Qt::Horizontal)
        setY(val);
    else
        setX(val);
    m_pen.setWidth(2);
    m_pen.setStyle(Qt::PenStyle::SolidLine);
    m_pen.setColor(Qt::darkMagenta);
}
/*
QRectF OsciCategoryLine::boundingRect() const
{
    QPointF p1 = getPoint1();
    QPointF p2 = getPoint2();
    return QRectF(p1.x(), p1.y(), p2.x(), p2.y());
}
*/
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
        pt2_.setX(pt2.x() - ((m_label.size()*9) /2 ));
        pt2_.setY(pt2.y() - 5);
        painter->drawText(pt2_, m_label);
        QPointF pt1 = getPoint1();
        pt1.setX(pt1.x() - ((m_label.size()*9) /2 ));
        pt1.setY(pt1.y() + 12);
        painter->drawText(pt1, m_label);
    }
}

void OsciCategoryLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void OsciCategoryLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void OsciCategoryLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

QPointF OsciCategoryLine::getPoint1() const
{
    if(!m_osci) return QPointF();
    QPointF start;
    if(getOrientation() == Qt::Horizontal)
        start = m_chart->mapToPosition(QPointF(0,m_y));
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
        start = m_chart->mapToPosition(QPointF(m_osci->width(),m_y));
    else
    {
        qreal ymax = m_chart->mapToValue(m_chart->plotArea().topLeft()).y();

        start = m_chart->mapToPosition(QPointF(m_x, ymax));
    }
    QPointF startItem = mapFromScene(start);
    return startItem;
}
