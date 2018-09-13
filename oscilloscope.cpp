#include "oscilloscope.h"
#include <QColor>
#include <QMessageBox>

Oscilloscope::Oscilloscope() :
    m_isTouching(false),
    m_isCatPressed(false)
{    
    m_chart = new OsciChart();

    m_chart->legend()->hide();
    m_chart->setTitle(QObject::tr("Осциллоскоп"));

    m_catPen = QPen(Qt::yellow,3,Qt::DashDotLine,Qt::RoundCap,Qt::RoundJoin);
    m_catColor = Qt::blue;
    m_cat = new QCategoryAxis();
    m_cat->setLinePenColor(m_catColor);
    m_cat->setGridLinePen(m_catPen);

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
    m_chart->addAxis(m_cat, Qt::AlignRight);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    QList<TrendOscilloscope*>::const_iterator it = m_trends.constBegin();
    while (it != m_trends.end())
    {
        TrendOscilloscope* trend = *it;
        {
            m_chart->addSeries(trend->getSeries());
            trend->getSeries()->attachAxis(m_axisX);
            trend->getSeries()->attachAxis(m_axisY);
            trend->getSeries()->attachAxis(m_cat);
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
    //получили позицию координат графика для нажатия отправляем на проверку
    m_isCatPressed = checkIfCateg(this->chart()->mapToValue(event->pos()));

    if(m_isCatPressed)
    {
        //выбираем категорию
        return;
    }
    QChartView::mousePressEvent(event);
}

void Oscilloscope::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    if(m_isCatPressed)
    {
        //двигаем выбранную категорию за мышкой
        return;
    }
    QChartView::mouseMoveEvent(event);
}

void Oscilloscope::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching)
        m_isTouching = false;
    if(m_isCatPressed)
    {
        //фиксируем выбранную категорию
        m_isCatPressed = false;
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

void Oscilloscope::addCategory(const QVariant& val, const QString& name)
{
    m_cat->append(name,val.toReal());
}

bool Oscilloscope::checkIfCateg(const QPointF &point) const
{
    return false;
}
