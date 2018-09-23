#ifndef OSCITOOLS_H
#define OSCITOOLS_H

#include <QObject>

class QGraphicsLineItem;
class QPointF;

/*
    Класс статических вспомогательных функций
*/

class OsciTools
{
public:
    //создание линии по двум точкам
    static QGraphicsLineItem* createLineItem(const QPointF &p1,const QPointF &p2);
    //создание горизонтальной линии
    static QGraphicsLineItem* createHLineItem(qreal y, qreal x1, qreal x2);
    //создание вертикальной линии
    static QGraphicsLineItem* createVLineItem(qreal x, qreal y1, qreal y2);
};

#endif // OSCITOOLS_H
