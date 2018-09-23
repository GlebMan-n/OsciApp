#include "oscitools.h"
#include <QGraphicsLineItem>
#include <QPointF>
#include <QLineF>

QGraphicsLineItem* OsciTools::createLineItem(const QPointF &p1,const QPointF &p2)
{
    QGraphicsLineItem* result = nullptr;
    QLineF line(p1,p2);
    result = new QGraphicsLineItem(line);
    return result;
}

QGraphicsLineItem* OsciTools::createHLineItem(qreal y, qreal x1, qreal x2)
{
    QPointF p1(x1,y);
    QPointF p2(x2,y);
    return createLineItem(p1,p2);
}

QGraphicsLineItem* OsciTools::createVLineItem(qreal x, qreal y1, qreal y2)
{
    QPointF p1(x,y1);
    QPointF p2(x,y2);
    return createLineItem(p1,p2);
}
