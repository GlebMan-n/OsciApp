#include "oscilloscope.h"
#include <QColor>
#include <QMessageBox>
#include <QtMath>
#include "oscitooltip.h"
#include <QGraphicsLineItem>
#include "oscitools.h"
#include "oscicategoryline.h"

Oscilloscope::Oscilloscope(QWidget *parent/* = nullptr*/)
{    
    this->setParent(parent);
    m_chart = new OsciChart();

    m_chart->legend()->hide();
    m_chart->setTitle(QObject::tr("Осциллоскоп"));

    m_dtStart = QDateTime::currentDateTime();
    m_autoupdate = false;
    m_timeMax = 1200;
    m_valMax = 16;
    m_valMin = 0;
    m_timeMin = 0;
    m_TickCountTime = 5;
    m_TickCountVal = 5;

    QVector<QPointF> vec ;
    TrendOscilloscope* trend = new TrendOscilloscope(this, 0, true, 10);
    addTrend(trend);

    addTrend(new TrendOscilloscope(this, 1, true, 10));
    addTrend(new TrendOscilloscope(this, 2, true, 10));

    m_axisX = new QValueAxis();
    m_axisX->setLabelFormat("%i");
    m_axisX->setTitleText(QObject::tr("время (сек)"));
    m_axisX->setMin(m_timeMin);

    m_axisX->setLinePenColor(Qt::red);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);

    m_axisY = new QValueAxis;
    m_axisY->setTitleText(QObject::tr("значение (ед.)"));
    m_axisY->setLinePenColor(Qt::green);
    m_axisY->setMin(0);

    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    QList<TrendOscilloscope*>::const_iterator it = m_trends.constBegin();
    while (it != m_trends.end())
    {
        TrendOscilloscope* trend = *it;
        if (trend)
        {
            m_chart->addSeries(trend->getSeries());
            trend->getSeries()->attachAxis(m_axisX);
            trend->getSeries()->attachAxis(m_axisY);
        }
        *it++;
    }
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setChart(m_chart);
    setRenderHint(QPainter::Antialiasing);
    setRubberBand(QChartView::NoRubberBand);
    setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    //m_chart->setAnimationOptions(QChart::SeriesAnimations);
    m_axisX->setMax(m_timeMax);
    m_axisX->setMin(m_timeMin);
    m_axisY->setMax(m_valMax);
    m_axisY->setMin(m_valMin);


}

void Oscilloscope::addTrend(TrendOscilloscope* trend)
{
    m_trends.append(trend);
}

void Oscilloscope::update()
{
    if(m_autoupdate)
    {
        m_timeMin = 0;
        m_timeMax = QDateTime::currentDateTime().toTime_t() - m_dtStart.toTime_t();
        m_axisX->setMax(m_timeMax);
        m_axisX->setMin(m_timeMin);
    }
    this->scene()->update(this->rect());
}

TrendOscilloscope* Oscilloscope::findTrendById(int id)
{
    QList<TrendOscilloscope*>::const_iterator it = m_trends.constBegin();
    while (it != m_trends.end())
    {
        TrendOscilloscope* trend = *it;
        if(trend && trend->getId() == id)
            return trend;
        *it++;
    }
    return nullptr;
}

void Oscilloscope::slotNewData(QVector<ZLogData> arr)
{
    return;
    update();
    for(auto i = 0; i < arr.size(); i++)
    {
       TrendOscilloscope* trend = findTrendById(arr.at(i).m_id);
       if(trend)                         
           trend->addPoint(arr.at(i).m_data.toPointF(m_dtStart));
    }
    this->repaint();
}

void Oscilloscope::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Control:
        setRubberBand(QChartView::NoRubberBand);
    break;
    default:
        QGraphicsView::keyReleaseEvent(event);
        break;
    }
}

void Oscilloscope::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        m_timeMax = 1200;
        m_valMax = 16;
        m_valMin = 0;
        m_timeMin = 0;
        m_TickCountTime = 5;
        m_TickCountVal = 5;
        m_axisX->setMax(m_timeMax);
        m_axisX->setMin(m_timeMin);
        m_axisY->setMax(m_valMax);
        m_axisY->setMin(m_valMin);
        m_autoupdate = false;
        this->scene()->update(this->rect());
        break;
    case Qt::Key_Plus:
        m_timeMax = m_timeMax - m_timeMax/2;
        break;
    case Qt::Key_Minus:
        m_timeMax = m_timeMax + m_timeMax/2;
        break;
    case Qt::Key_Left:
        m_timeMin = m_timeMin - m_timeMax/2;
        m_timeMax = m_timeMax - m_timeMax/2;
        break;
    case Qt::Key_Right:
        m_timeMin = m_timeMin + m_timeMax/2;
        m_timeMax = m_timeMax + m_timeMax/2;
        break;
    case Qt::Key_F5:
        m_autoupdate = !m_autoupdate;
        break;
    case Qt::Key_Control:
        setRubberBand(QChartView::RectangleRubberBand);
    break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void Oscilloscope::addCategoryY(qreal val, const QString& label)
{
    OsciCategoryLine* line = new OsciCategoryLine(val, Qt::Horizontal, this, m_chart);

    line->show();
    if(m_catLines[label] != nullptr)
    {
        this->scene()->removeItem(m_catLines[label]);
        delete m_catLines[label];
    }
    m_catLines[label] = line;
    this->scene()->addItem(line);
}


void Oscilloscope::addCategoryX(qreal val, const QString& label)
{
    OsciCategoryLine* line = new OsciCategoryLine(val, Qt::Vertical, this, m_chart);
    line->setLabel(label);
    line->setMaxY(m_valMax);
    line->show();
    if(m_catLines[label] != nullptr)
    {
        this->scene()->removeItem(m_catLines[label]);
        delete m_catLines[label];
    }

    m_catLines[label] = line;
    this->scene()->addItem(line);
}

/*
void Oscilloscope::markIntersectionPoints()
{
    //контрольная линиялиния
    if(!m_selectedItemLine)
        return;
    //удаляем старые метки
    clearTooltips();
    QPointF point1 = this->chart()->mapToValue(m_selectedItemLine->line().p1());
    QPointF point2 = this->chart()->mapToValue(m_selectedItemLine->line().p2());
    QLineF controlLine(point1,point2);
    //cписок линий тренда
    QVector<QLineF> m_lines = getTrendsLines();
    //обходим все отрезки трендов и сравниваем с контрольным отрезком
    //получая точки пересечения
    for(auto i = 0; i < m_lines.size(); i++)
    {
        QPointF intersectionPoint;
        //проверка на пересечение
        QLineF lineToIntersect = m_lines.at(i);
        int iRes = controlLine.intersect(lineToIntersect, &intersectionPoint);
        //если отрезки пересеклись
        if(iRes == QLineF::BoundedIntersection)
            //сразу отображаем метки
            toolTip(intersectionPoint);
    }
    this->repaint();
}*/

/*
QVector<QLineF> Oscilloscope::getTrendsLines()
{
    //TODO: перебор трендов
    //выборка всех отрезков
    QVector<QLineF> result;
    for(auto i = 0; i < m_trends.size(); i++)
    {
       TrendOscilloscope* trend = m_trends[i];
       if(trend)
       {
           QList<QPointF> points = trend->getSeries()->points();
           for(auto i = 0; i < points.size()-1; i++)
               result.append(QLineF(points.at(i),points.at(i+1)));
       }
    }
    return result;
}
*/

/*
void Oscilloscope::clearTooltips()
{
    for(auto i = 0; i < m_tooltips.size(); i++)
    {
        OsciTooltip* tooltip = m_tooltips.at(i);
        if(!tooltip)
            continue;
        tooltip->hide();
        delete tooltip;
        tooltip = nullptr;
    }
    m_tooltips.clear();
    this->repaint();
}
*/

/*
void Oscilloscope::toolTip(QPointF point)
{
    OsciTooltip* tooltip = new OsciTooltip(m_chart);
    tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
    tooltip->setAnchor(point);
    tooltip->setZValue(11);
    tooltip->updateGeometry();
    tooltip->show();
    m_tooltips.append(tooltip);
}
*/
