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
    TrendOscilloscope* trend = new TrendOscilloscope(this, 0);
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
    drawAllLabels();
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
        this->scene()->update();
        m_chart->plotAreaChanged(m_chart->plotArea());
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

void Oscilloscope::addCategory(qreal val,Qt::Orientation orientation , const QString& label)
{
    OsciCategoryLine *item = new OsciCategoryLine();
    item->setChart(m_chart);
    item->setOsci(this);
    item->setOrientation(orientation);
    QPointF ptf = m_chart->mapToPosition(QPointF(val,val));

    if(orientation ==Qt::Horizontal)
        ptf = QPointF(m_chart->plotArea().center().x(), ptf.y());
    else if(orientation ==Qt::Vertical)
        ptf = QPointF(ptf.x(), m_chart->plotArea().center().y());
    item->setPos(ptf);
    item->setValPoint(m_chart->mapToValue(ptf));
    item->setLabel(label);
    item->setTextDrawType(0);
    m_catLines.push_back(item);
    this->scene()->addItem(item);
}

void Oscilloscope::addHCategory(qreal val, const QString& label)
{
    addCategory(val,Qt::Horizontal,label);
}

void Oscilloscope::addVCategory(qreal val, const QString& label)
{
    addCategory(val,Qt::Vertical,label);
}

void Oscilloscope::drawAllLabels()
{
    //находим обновленные точки
    QVector<QPointF> newLabelPoints = findAllLabelPoints();
    clearLegacyLabels();
    for (auto i = 0; i < newLabelPoints.size(); i++)
        drawLabel(newLabelPoints.at(i));
}

QVector<QPointF> Oscilloscope::findAllLabelPoints()
{
    QVector<QPointF> result;
    //получаем все линии трендов
    QVector<QLineF> lines = getTrendsLines();
    //перебор линий трендов
    for(auto i = 0; i < lines.size(); i++)
    {
        //перебор линий категорий
        for(auto j = 0; j < m_catLines.size(); j++)
        {
            OsciCategoryLine* catLine = m_catLines.at(j);
            QLineF line = catLine->getLine();
            QPointF intersectionPoint;
            int iRes = lines.at(i).intersect(line, &intersectionPoint);
            if(iRes == QLineF::BoundedIntersection)
                result.append(intersectionPoint);
         }
    }
    return result;
}

bool Oscilloscope::drawLabel(const QPointF &labelPoint)
{
    OsciTooltip* tooltip = new OsciTooltip(m_chart);
    tooltip->setText(QString("X: %1 \nY: %2 ").arg(labelPoint.x()).arg(labelPoint.y()));
    tooltip->setAnchor(labelPoint);
    tooltip->setZValue(11);
    tooltip->updateGeometry();
    tooltip->show();
    m_tooltips.append(tooltip);
    m_labelPoints.push_back(labelPoint);
    return false;
}

void Oscilloscope::clearLegacyLabels()
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
    m_labelPoints.clear();
    m_tooltips.clear();
}

QVector<QLineF> Oscilloscope::getTrendsLines()
{
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
