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

struct GraphicCoords
{
    GraphicCoords(qreal x, qreal y)
    {
        m_x = x;
        m_y = y;
    }
    GraphicCoords(QPointF point)
    {
        m_x = point.x();
        m_y = point.y();
    }

    QPointF toPointF() const
    {
        return QPointF(m_x,m_y);
    }

    qreal m_x;
    qreal m_y;
};

class OsciCategoryLine : public QGraphicsItem
{
public:
    OsciCategoryLine(qreal val,
                     Qt::Orientation orientaton,
                     Oscilloscope* osci,
                     OsciChart *parent);
    //принимает значения графиков
    void setX(qreal x);// { m_x = x; }
    //принимает значения графиков
    void setY(qreal y);// { m_y = y; }
    void setMaxY(qreal max_y) {m_maxY = max_y;}
    void setLabel(const QString& label) {m_label = label;}
    void setOrientation(Qt::Orientation orientation) { m_orientation = orientation; }
    Qt::Orientation getOrientation() const {return m_orientation;}
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    //-1 не отображать, 0 отображать сверху и снизу, 1 только сверху, 2только снизу
    void setTextDrawType(short type) {m_drawText = type;}
    void redraw();
    QPointF toScreen(const GraphicCoords& coords) const;
    GraphicCoords toGraphic(const QPointF &point) const;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

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
    bool            m_pressed; //-1 не отображать, 0 отображать сверху и снизу, 1 только сверху, 2только снизу
    short           m_drawText;
};

#endif // OSCICATEGORYLINE_H
