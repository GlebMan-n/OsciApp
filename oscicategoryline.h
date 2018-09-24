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

class OsciCategoryLine : public QGraphicsItem
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
    QRectF boundingRect() const override;
protected:    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

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
    bool            m_pressed;
};

#endif // OSCICATEGORYLINE_H
