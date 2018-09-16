#ifndef TRENDOSCILLOSCOPE_H
#define TRENDOSCILLOSCOPE_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <zlogdata.h>
#include <QList>
#include <QPointF>

#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>

QT_CHARTS_USE_NAMESPACE

class TrendOscilloscope : public QObject
{
public:
    TrendOscilloscope(QObject* parent, int id, bool autoremove = false, int countMax = 250);
    int getId() const {return m_id;}
    void setId(int id) {m_id = id;}
    QSplineSeries* getSeries() {return m_series;}
    void addPoint(const QPointF &point);
private:
    int                         m_id;
    QSplineSeries*              m_series;
    bool                        m_autoremove;
    int                         m_countMax;
};

#endif // TRENDOSCILLOSCOPE_H
