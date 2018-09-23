#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <zlogdata.h>
#include <QList>
#include "trendoscilloscope.h"
#include "oscichart.h"
#include <QtCharts/QCategoryAxis>
#include <QLineF>
#include <QVector>

class OsciTooltip;
class OsciCategoryLine;
class QGraphicsLineItem;

class Oscilloscope : public QChartView
{
public:
    Oscilloscope();
    void update();
    TrendOscilloscope* findTrendById(int id);
    void setTimeMax(qreal timeMax) {m_timeMax = timeMax;}
    void setValMax(qreal valMax)  {m_valMax = valMax;}
    void setTickCountTime(int tickCountTime)  {m_TickCountTime = tickCountTime;}
    //добавляем линию категории по горизонтали
    void addCategoryY(qreal val, const QString& label = QObject::tr("нет"));
    //добавляем линию категории по вертикали
    void addCategoryX(qreal val, const QString& label = QObject::tr("нет"));
    void setTickCountVal(int tickCountVal)  { m_TickCountVal = tickCountVal;}
    void addTrend(TrendOscilloscope* trend);
    qreal getValMax() const {return m_valMax; }
public slots:
    void slotNewData(QVector<ZLogData> arr);

protected:
    bool viewportEvent(QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE
    {
        chart()->scroll(event->delta(), 0);
        m_timeMin = m_timeMin - event->delta();
        m_timeMax = m_timeMax - event->delta();
        event->accept();
        QChartView::wheelEvent(event);
    }
/*private:
    //передавать координаты графика
    bool findCatByPoint(const QPointF &point);
    //передавать экранные координаты
    bool moveCat(const QPointF &point);
    //принимает координаты графика
    void toolTip(QPointF point);
    QVector<QLineF> getTrendsLines();
    void markIntersectionPoints();
    void clearTooltips();*/
private:
    QList<TrendOscilloscope*>       m_trends;
    OsciChart*                      m_chart;
    QValueAxis*                     m_axisY;
    QValueAxis*                     m_axisX;
    QDateTime                       m_dtStart;
    qreal                           m_timeMax;
    qreal                           m_timeMin;
    qreal                           m_valMax;
    qreal                           m_valMin;
    int                             m_TickCountTime;
    int                             m_TickCountVal;
    bool                            m_autoupdate;
    QGraphicsLineItem*              m_categoryLine;
    QList<OsciTooltip*>             m_tooltips;
    QMap<QString,OsciCategoryLine*> m_catLines;
};

#endif // OSCILLOSCOPE_H
