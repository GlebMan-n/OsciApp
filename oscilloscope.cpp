#include "oscilloscope.h"
#include <QColor>
#include <QMessageBox>
#include <QtMath>
#include "oscitooltip.h"
#include <QGraphicsLineItem>
#include "oscitools.h"
#include "oscicategoryline.h"

Oscilloscope::Oscilloscope(int id, QWidget *parent/* = nullptr*/)
{    
    this->setParent(parent);
    setId(id);
    addTrend(new TrendOscilloscope(this, id));
    m_chart = new OsciChart();

    m_chart->legend()->hide();


    m_dtStart = QDateTime::currentDateTime();
    m_autoupdate = false;
    m_timeMax = 1200;
    m_valMax = 16;
    m_valMin = 0;
    m_timeMin = 0;
    m_TickCountTime = 5;
    m_TickCountVal = 5;

    QVector<QPointF> vec ;
    m_axisX = new QValueAxis();
    m_axisX->setLabelFormat("%i");

    m_axisX->setMin(m_timeMin);

    m_axisX->setLinePenColor(Qt::red);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);

    m_axisY = new QValueAxis;

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
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
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

QString Oscilloscope::getLeftLabel() const
{
    return m_leftLabel;
}

void Oscilloscope::setLeftLabel(const QString &leftLabel)
{
    m_leftLabel = leftLabel;
    m_axisY->setTitleText(m_leftLabel);
}

QString Oscilloscope::getBottomLabel() const
{
    return m_bottomLabel;
}

void Oscilloscope::setBottomLabel(const QString &bottomLabel)
{
    m_bottomLabel = bottomLabel;
    m_axisX->setTitleText(m_bottomLabel);
}

QString Oscilloscope::getHeader() const
{
    return m_header;
}

void Oscilloscope::setHeader(const QString &header)
{
    m_header = header;
    m_chart->setTitle(m_header);
}

int Oscilloscope::getId() const
{
    return m_id;
}

void Oscilloscope::setId(int id)
{
    m_id = id;
}

void Oscilloscope::refresh()
{
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
}

void Oscilloscope::zoomIn()
{
    m_chart->zoomIn();
}

void Oscilloscope::zoomOut()
{
    m_chart->zoomOut();
}

void Oscilloscope::mooveLeft()
{
    qreal tmp = m_timeMax/4;
    m_timeMax -= tmp;
    m_timeMin -= tmp;
    m_axisX->setMax(m_timeMax);
    m_axisX->setMin(m_timeMin);
    m_autoupdate = false;
    this->scene()->update();
    m_chart->plotAreaChanged(m_chart->plotArea());
}

void Oscilloscope::mooveRight()
{
    qreal tmp = m_timeMax/4;
    m_timeMax += tmp;
    m_timeMin += tmp;
    m_axisX->setMax(m_timeMax);
    m_axisX->setMin(m_timeMin);
    m_autoupdate = false;
    this->scene()->update();
    m_chart->plotAreaChanged(m_chart->plotArea());
}

void Oscilloscope::autoupdate()
{
    m_autoupdate = !m_autoupdate;
}

void Oscilloscope::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        refresh();
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        mooveLeft();
        break;
    case Qt::Key_Right:
        mooveRight();
        break;
    case Qt::Key_F5:
        autoupdate();
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
    //tooltip->setText(QString("X: %1 \nY: %2 ").arg(labelPoint.x()).arg(labelPoint.y()));
    tooltip->setText(QString("Y: %1 ").arg(QString::number(labelPoint.y(),'f',2)));
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

qreal Oscilloscope::getTimeMax() const
{
    return m_chart->mapToValue(m_chart->plotArea().bottomRight()).x();
}

qreal Oscilloscope::getTimeMin() const
{
    return m_chart->mapToValue(m_chart->plotArea().bottomLeft()).x();
}

qreal Oscilloscope::getValMax() const
{
    return m_chart->mapToValue(m_chart->plotArea().topRight()).y();
}
qreal Oscilloscope::getValMin() const
{
    return m_chart->mapToValue(m_chart->plotArea().bottomRight()).y();
}
