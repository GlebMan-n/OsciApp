#include "trendoscilloscope.h"

TrendOscilloscope::TrendOscilloscope(QObject* parent, int id) :
    m_id(id)
{
    this->setParent(parent);
    m_series = new QSplineSeries();
    m_series->setPointLabelsVisible();
}
