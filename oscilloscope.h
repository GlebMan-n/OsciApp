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
    Oscilloscope(QWidget *parent = nullptr);
    void update();
    TrendOscilloscope* findTrendById(int id);
    void setTimeMax(qreal timeMax) {m_timeMax = timeMax;}
    void setValMax(qreal valMax)  {m_valMax = valMax;}
    void setTickCountTime(int tickCountTime)  {m_TickCountTime = tickCountTime;}
    //добавляем линию категории по горизонтали
    void addHCategory(qreal val, const QString& label = QObject::tr("нет"));
    //добавляем линию категории по вертикали
    void addVCategory(qreal val, const QString& label = QObject::tr("нет"));
    void setTickCountVal(int tickCountVal)  { m_TickCountVal = tickCountVal;}
    void addTrend(TrendOscilloscope* trend);
    qreal getValMax() const {return m_valMax; }
private:
    //нарисовать метки пересечений
    void drawAllLabels();
    //найти все метки пересечений с отсекающими прямыми (прямыми категорий)
    QVector<QPointF> findAllLabelPoints();
    //нарисовать метку пересечения
    bool drawLabel(const QPointF &labelPoint);
    //очистить метки пересечений
    void clearLegacyLabels(const QVector<QPointF> &labelPoint);
    //найдем точки, которые есть в sourcePoints и которых нет в newPoints
    //вернем их для дальнейшего удаления
    QVector<QPointF> findLegacyPoints(const QVector<QPointF> &newPoints, const QVector<QPointF> &sourcePoints );
    QVector<QLineF> getTrendsLines();
public slots:
    void slotNewData(QVector<ZLogData> arr);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
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
    QVector<OsciCategoryLine*>      m_catLines;
    QVector<QPointF>                m_labelPoints;
};

#endif // OSCILLOSCOPE_H
