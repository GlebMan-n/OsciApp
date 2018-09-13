#ifndef OSCICHART_H
#define OSCICHART_H

#include <QtCharts/QChart>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class OsciChart : public QChart
{
public:
    explicit OsciChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    ~OsciChart();

protected:
    bool sceneEvent(QEvent *event);

private:
    bool gestureEvent(QGestureEvent *event);

private:
};

#endif // OSCICHART_H
