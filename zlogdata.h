#ifndef ZLOGDATA_H
#define ZLOGDATA_H
#include <QObject>
#include <QVariant>
#include <QDateTime>
#include <QPointF>

class TrendData
{
public:
    QVariant m_val;
    QDateTime m_dt;
    //возможна поправка на начальное время
    QPointF toPointF(const QDateTime &dt) const
    {
        QPointF point;
        if(!dt.isValid())
            point.setX(m_dt.toTime_t());
        else
            point.setX(m_dt.toTime_t() - dt.toTime_t());
        point.setY(m_val.toReal());
        return point;
    }
};

class ZLogData
{
public:
    ZLogData() {}
    ZLogData(int id, const QVariant& var, const QDateTime& dt) :
        m_id(id)
    {
        m_data.m_val = var;
        m_data.m_dt = dt;
    }

    int m_id;
    TrendData m_data;
};

#endif // ZLOGDATA_H
