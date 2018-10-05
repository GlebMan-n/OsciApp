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


OsciCategoryLine::OsciCategoryLine(QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    setTextDrawType(0);
    m_pressed = false;

}

OsciCategoryLine::~OsciCategoryLine()
{

}

QRectF OsciCategoryLine::boundingRect() const
{
    QRectF result;
    if(!m_chart)
        return  result;
    if(getOrientation() == Qt::Vertical)
    {
        QPointF tR = m_chart->plotArea().topRight();
        QPointF bL = m_chart->plotArea().bottomLeft();
        QPointF bottomRight(pos().x() + 15.0,bL.y() + 25.0);
        QPointF topLeft(pos().x() - 15.0,tR.y() - 25.0) ;
        result = QRectF(mapFromScene(topLeft),mapFromScene(bottomRight));
    }
    else if(getOrientation() == Qt::Horizontal)
    {
        //To Do: implementation of horizontal orientation
    }
    return result;
}

void OsciCategoryLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!m_chart)
        return;
    painter->setPen(m_pen);
    QLineF line;
    if(getOrientation() == Qt::Vertical)
    {
        QPointF tR = m_chart->plotArea().topRight();
        QPointF bL = m_chart->plotArea().bottomLeft();
        QPointF p1(m_chart->mapFromScene(pos()).x(),tR.y());
        QPointF p2(m_chart->mapFromScene(pos()).x(),bL.y());
        QPointF pt1 = mapFromScene(m_chart->mapToScene(p2));
        QPointF pt2 = mapFromScene(m_chart->mapToScene(p1));
        line = QLineF(pt1,pt2);

        if(!m_pressed && !m_label.isEmpty() && (m_drawText == 0 || m_drawText == 1))
        {
            QFontMetrics fm(painter->font());
            qreal width = fm.width(m_label);
            pt2.setX(pt2.x() - width / 2);
            pt2.setY(pt2.y() - 10);
            painter->drawText(pt2, m_label);
        }

        if(!m_pressed && (m_drawText == 0 || m_drawText == 2))
        {
            int x = (int) m_chart->mapToValue(m_chart->mapFromScene(pos())).x();
            QString valS = QString::number(x);
            QFontMetrics fm(painter->font());
            qreal width = fm.width(valS);
            pt1.setX(pt1.x() - width / 2);
            pt1.setY(pt1.y() + 10);
            painter->drawText(pt1, valS);
        }
    }
    else if(getOrientation() == Qt::Horizontal)
    {
        //To Do: implementation of horizontal orientation
    }
    if(!line.isNull())
        painter->drawLine(line);
    #ifdef QT_DEBUG
        //painter->drawRect(boundingRect());
    #endif
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void OsciCategoryLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = getCurScenePoint(event);
    m_valPoint = m_chart->mapToValue(pos);
    this->setPos(pos);
}

void OsciCategoryLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressed = true;
    m_osci->scene()->update();
    m_chart->update();
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    QPointF pos = getCurScenePoint(event);
    this->setPos(pos);
}

void OsciCategoryLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressed = false;
    this->setCursor(QCursor(Qt::ArrowCursor));
    m_osci->scene()->update();
    Q_UNUSED(event);
}

QPointF OsciCategoryLine::getValPoint() const
{
    return m_valPoint;
}

void OsciCategoryLine::setValPoint(const QPointF &valPoint)
{
    m_valPoint = valPoint;
}

Oscilloscope *OsciCategoryLine::getOsci() const
{
    return m_osci;
}

void OsciCategoryLine::setOsci(Oscilloscope *osci)
{
    m_osci = osci;
}

OsciChart *OsciCategoryLine::getChart() const
{
    return m_chart;
}

void OsciCategoryLine::setChart(OsciChart *chart)
{
    m_chart = chart;
    connect(m_chart,SIGNAL(plotAreaChanged(const QRectF &)),SLOT(slotPlotAreaChanged(const QRectF &)));
}

QString OsciCategoryLine::getLabel() const
{
    return m_label;
}

void OsciCategoryLine::setLabel(const QString &label)
{
    m_label = label;
}

QColor OsciCategoryLine::getColor() const
{
    return m_color;
}

void OsciCategoryLine::setColor(const QColor &color)
{
    m_color = color;
    m_pen.setColor(m_color);

}

QPointF OsciCategoryLine::getCurScenePoint(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = m_chart->mapToValue(mapToScene(event->pos()));
    QPointF bottomLeft = m_chart->mapToValue(m_chart->plotArea().bottomLeft());
    QPointF topRight = m_chart->mapToValue(m_chart->plotArea().topRight());

    if(pos.x() < bottomLeft.x())
        pos.setX(bottomLeft.x());
    if(pos.x() > topRight.x())
        pos.setX(topRight.x());
    if(pos.y() > bottomLeft.y())
        pos.setY(bottomLeft.y());
    if(pos.y() < topRight.y())
        pos.setY(topRight.y());
    return m_chart->mapToPosition(pos);
}

qreal OsciCategoryLine::getWidth() const
{
    return m_width;
}

void OsciCategoryLine::setWidth(const qreal &width)
{
    m_width = width;
    m_pen.setWidthF(m_width);
}

QLineF OsciCategoryLine::getLine() const
{
    QLineF line;
    if(getOrientation() == Qt::Vertical)
    {
        QPointF tR = m_chart->plotArea().topRight();
        QPointF bL = m_chart->plotArea().bottomLeft();
        QPointF p1(m_chart->mapFromScene(pos()).x(),tR.y());
        QPointF p2(m_chart->mapFromScene(pos()).x(),bL.y());
        QPointF pt1 = mapFromScene(m_chart->mapToScene(p2));
        QPointF pt2 = mapFromScene(m_chart->mapToScene(p1));
        line = QLineF(pt1,pt2);
    }
    else if(getOrientation() == Qt::Horizontal)
    {
        //
    }
    return line;
}

void OsciCategoryLine::slotPlotAreaChanged(const QRectF &plotArea)
{
    this->setPos(m_chart->mapToPosition(m_valPoint));
    m_osci->scene()->update();
    Q_UNUSED(plotArea);
}
