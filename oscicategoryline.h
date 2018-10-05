#ifndef OSCICATEGORYLINE_H
#define OSCICATEGORYLINE_H

#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QFont>
#include <QGraphicsItem>
#include <QPen>
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class OsciChart;
class Oscilloscope;

class OsciCategoryLine : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit OsciCategoryLine(QObject *parent = nullptr);
    ~OsciCategoryLine();
    void setOrientation(Qt::Orientation orientation) { m_orientation = orientation; }
    Qt::Orientation getOrientation() const {return m_orientation;}
    qreal getWidth() const;
    void setWidth(const qreal &width);
    QColor getColor() const;
    void setColor(const QColor &color);
    QPointF getCurScenePoint(QGraphicsSceneMouseEvent *event);
    void setTextDrawType(short type) {m_drawText = type;} //-1 не отображать, 0 отображать сверху и снизу, 1 только сверху, 2только снизу
    QString getLabel() const;
    void setLabel(const QString &label);
    QLineF getLine() const;
    bool needToRecalc(const QPointF &point);
    OsciChart *getChart() const;
    void setChart(OsciChart *chart);

    Oscilloscope *getOsci() const;
    void setOsci(Oscilloscope *osci);

    QPointF getValPoint() const;
    void setValPoint(const QPointF &valPoint);

signals:

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    OsciChart*      m_chart;
    Oscilloscope*   m_osci;
    Qt::Orientation m_orientation;
    qreal           m_width;
    QColor          m_color;
    QPen            m_pen;
    short           m_drawText;
    QString         m_label;
    bool            m_pressed;
    QPointF         m_valPoint; //последняя точка движения для графика

public slots:
    void slotPlotAreaChanged(const QRectF &plotArea);
};

#endif // OSCICATEGORYLINE_H
