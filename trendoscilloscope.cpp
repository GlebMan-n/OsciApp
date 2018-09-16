#include "trendoscilloscope.h"

TrendOscilloscope::TrendOscilloscope(QObject* parent, int id, bool autoremove /*= false*/, int countMax/* = 250*/) :
    m_id(id)
{
    this->setParent(parent);
    m_series = new QSplineSeries();
    m_autoremove = autoremove;
    m_countMax = countMax;
}

void TrendOscilloscope::addPoint(const QPointF &point)
{
    if(m_autoremove && m_series->count() > m_countMax)
        m_series->removePoints(0,1);
    m_series->append(point);
}
