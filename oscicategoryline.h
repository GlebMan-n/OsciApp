#ifndef OSCICATEGORYLINE_H
#define OSCICATEGORYLINE_H

#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QFont>
#include <QGraphicsItem>
#include <QPen>
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class OsciChart;
class Oscilloscope;

class OsciCategoryLine : public QGraphicsLineItem
{
public:
    OsciCategoryLine(qreal val,
                     Qt::Orientation orientaton,
                     Oscilloscope* osci,
                     OsciChart *parent);
    void setX(qreal x) { m_x = x; }
    void setY(qreal y) { m_y = y; }
    void setMaxY(qreal max_y) {m_maxY = max_y;}
    void setLabel(const QString& label) {m_label = label;}
    void setOrientation(Qt::Orientation orientation) { m_orientation = orientation; }
    Qt::Orientation getOrientation() const {return m_orientation;}
protected:
    //virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QPointF getPoint1() const;
    QPointF getPoint2() const;
private:
    Oscilloscope*   m_osci;
    OsciChart*      m_chart;
    QColor          m_defColor;
    QColor          m_selColor;
    QPen            m_pen;
    qreal           m_x;
    qreal           m_y;
    qreal           m_maxY;
    Qt::Orientation m_orientation;
    QString         m_label;
};

#endif // OSCICATEGORYLINE_H
