#include "oscilloscope.h"
#include <QColor>
#include <QMessageBox>
#include <QtMath>


static void selectItem(QGraphicsLineItem* gItem , const QColor &color)
{
    if(!gItem)
        return;
    QPen pen;
    pen = gItem->pen();
    pen.setColor(color);
    gItem->setPen(pen);
}

static bool pointInLine(const QPointF &point, const QVector& line)
{
    return false;
}

Oscilloscope::Oscilloscope() :
    m_isTouching(false),
    m_isCatPressed(false),
    m_selectedItemLine(nullptr),
    m_curCatAxis(nullptr)
{    
    m_chart = new OsciChart();

    m_chart->legend()->hide();
    m_chart->setTitle(QObject::tr("Осциллоскоп"));
    m_defAxColor = Qt::yellow;

    m_dtStart = QDateTime::currentDateTime();
    m_autoupdate = false;
    m_timeMax = 1200;
    m_valMax = 16;
    m_valMin = 0;
    m_timeMin = 0;
    m_TickCountTime = 5;
    m_TickCountVal = 5;

    QVector<QPointF> vec ;
    addTrend(new TrendOscilloscope(this, 0));
    addTrend(new TrendOscilloscope(this, 1));
    addTrend(new TrendOscilloscope(this, 2));

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
    m_catPen = QPen(m_defAxColor,3,Qt::DashDotLine,Qt::RoundCap,Qt::RoundJoin);
    m_catColor = Qt::blue;

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
    setRubberBand(QChartView::RectangleRubberBand);
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
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
}

TrendOscilloscope* Oscilloscope::findTrendById(int id)
{
    QList<TrendOscilloscope*>::const_iterator it = m_trends.constBegin();
    while (it != m_trends.end())
    {
        TrendOscilloscope* trend = *it;
        if(trend->getId() == id)
            return trend;
        *it++;
    }
    return nullptr;
}

void Oscilloscope::slotNewData(QVector<ZLogData> arr)
{
    update();
    for(auto i = 0; i < arr.size(); i++)
    {
       TrendOscilloscope* trend = findTrendById(arr.at(i).m_id);
       if(trend)                  
           trend->getSeries()->append(arr.at(i).m_data.toPointF(m_dtStart));
    }
}

bool Oscilloscope::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin) {
        m_isTouching = true;
        chart()->setAnimationOptions(QChart::NoAnimation);
    }
    return QChartView::viewportEvent(event);
}

void Oscilloscope::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    //получили позицию координат графика для нажатия отправляем на проверку и поиск категории
    m_isCatPressed = findCatByPoint(this->chart()->mapToValue(event->pos()));
    if(m_isCatPressed)
    {
        setRubberBand(QChartView::NoRubberBand);
        m_selectedItemLine = dynamic_cast<QGraphicsLineItem*> (itemAt(event->pos()));
        if(m_selectedItemLine && m_selectedItemLine->pen().color() == m_defAxColor)
            selectItem(m_selectedItemLine,Qt::red);
        return;
    }
    QChartView::mousePressEvent(event);
}

void Oscilloscope::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    if(m_isCatPressed && moveCat(event->pos()))
        return;
    QChartView::mouseMoveEvent(event);
}

void Oscilloscope::mouseReleaseEvent(QMouseEvent *event)
{
    setRubberBand(QChartView::RectangleRubberBand);
    if (m_isTouching)
        m_isTouching = false;
    if(m_isCatPressed)
    {
        m_isCatPressed = false;
        if(m_selectedItemLine)
            selectItem(m_selectedItemLine,m_defAxColor);
        m_selectedItemLine = nullptr;
        return;
    }
    chart()->setAnimationOptions(QChart::SeriesAnimations);
    QChartView::mouseReleaseEvent(event);
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
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void Oscilloscope::addCategory(const QVariant& startVal, const QVariant& endVal, const QString& name/* = QObject::tr("нет")*/)
{
    QCategoryAxis* cat = new QCategoryAxis();
    cat->setLinePenColor(m_catColor);
    cat->setGridLinePen(m_catPen);
    cat->setStartValue(startVal.toReal());
    cat->append(name,endVal.toReal());
    m_chart->addAxis(cat, Qt::AlignRight);

    QList<TrendOscilloscope*>::const_iterator it = m_trends.constBegin();
    while (it != m_trends.end())
    {
        TrendOscilloscope* trend = *it;
        if (trend)

            trend->getSeries()->attachAxis(cat);

        *it++;
    }
    m_cats.append(cat);
}

bool Oscilloscope::findCatByPoint(const QPointF &point)
{
    if(m_cats.isEmpty())
        return false;
    QList<QCategoryAxis*>::const_iterator it = m_cats.constBegin();
    while (it != m_cats.end())
    {
        QCategoryAxis* cat = *it;
        if(cat)
        {
            QStringList list = cat->categoriesLabels();
            for(auto i = 0; i < list.size(); i++)
            {
                int y = qCeil(point.y());
                int start = qCeil(cat->startValue(list.at(i)));
                int end = qCeil(cat->endValue(list.at(i)));
                if(qCeil(point.y()) == qCeil(cat->startValue(list.at(i))))
                {
                    m_changeCat.first = list.at(i);
                    m_changeCat.second = true;
                    m_curCatAxis = cat;
                    return true;
                }

                if(qCeil(point.y()) == qCeil(cat->endValue(list.at(i))))
                {
                    m_changeCat.first = list.at(i);
                    m_changeCat.second = false;
                    m_curCatAxis = cat;
                    return true;
                }
            }

        }
        *it++;
    }

    m_changeCat.first = QString();
    return false;
}

QList<QPoint> Oscilloscope::getPointsToDrawHint(const QPointF &point)
{
    QPointF grafPoint = this->chart()->mapToValue(point);
    qreal y = this->chart()->mapToValue(point).y();

    QList<TrendOscilloscope*>::const_iterator it = m_trends.constBegin();
    while (it != m_trends.end())
    {
        TrendOscilloscope* trend = *it;
        if(trend)
        {
            QSplineSeries* series = trend->getSeries();
            //m_timeMin m_timeMax
            //все точки тренда
            QVector<QPointF> points = series->points();
            if(points.size() < 2)
                continue;
            for (auto i = 1; i < points.size(); i++)
            {
                QVector vec;
                vec.append(points.at(i-1));
                vec.append(points.at(i));
                pointInLine(grafPoint,vec);

            }
        }
        *it++;
    }
}

bool Oscilloscope::moveCat(const QPointF &point)
{    
    int y = this->chart()->mapToValue(point).y();
    if(!m_curCatAxis)
        return false;
    if(m_changeCat.second)
        m_curCatAxis->setStartValue(y);
    else
    {
        int startV = qCeil(m_curCatAxis->startValue(m_changeCat.first));
        m_curCatAxis->remove(m_changeCat.first);
        m_curCatAxis->append(m_changeCat.first,qCeil(y));
        selectItem(m_selectedItemLine,m_defAxColor);
        m_selectedItemLine = dynamic_cast<QGraphicsLineItem*> (itemAt(point.x(),point.y()));
        if(m_selectedItemLine && m_selectedItemLine->pen().color() == m_defAxColor)
            selectItem(m_selectedItemLine,Qt::red);
    }
     return true;
}

