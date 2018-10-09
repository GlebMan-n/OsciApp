#ifndef OSCITOOLTIP_H
#define OSCITOOLTIP_H


/*class OsciTooltip
{
public:
    OsciTooltip();
};*/


#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QFont>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class OsciChart;

QT_CHARTS_USE_NAMESPACE

class OsciTooltip : public QGraphicsItem
{
public:
    OsciTooltip(OsciChart *parent);

    void setText(const QString &text);
    void setAnchor(QPointF point);
    void updateGeometry();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
private:
    QString     m_text;
    QRectF      m_textRect;
    QRectF      m_rect;
    QPointF     m_anchor;
    QFont       m_font;
    OsciChart*  m_chart;
};

#endif // OSCITOOLTIP_H
