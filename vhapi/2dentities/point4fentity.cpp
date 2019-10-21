#include "point4fentity.h"

#include <cmath>
#include <limits>

Point4fEntity::Point4fEntity()
{
    m_x = std::numeric_limits<float>::quiet_NaN();
    m_y = std::numeric_limits<float>::quiet_NaN();
    m_z = std::numeric_limits<float>::quiet_NaN();
    m_w = std::numeric_limits<float>::quiet_NaN();
}

Point4fEntity::Point4fEntity(const Point4fEntity &other) :
    m_x(other.m_x), m_y(other.m_y), m_z(other.m_z), m_w(other.m_w)
{
}

Point4fEntity::~Point4fEntity()
{
}

bool Point4fEntity::isValid() const
{
    return !std::isnan(m_x);
}

bool Point4fEntity::operator ==(const Point4fEntity &other) const
{
    return fabs(m_x - other.m_x) < 1e-6 &&
           fabs(m_y - other.m_y) < 1e-6 &&
           fabs(m_z - other.m_z) < 1e-6 &&
           fabs(m_w - other.m_w) < 1e-6;
}

bool Point4fEntity::operator !=(const Point4fEntity &other) const
{
    return !(*this == other);
}

QString Point4fEntity::toString() const
{
    QString s;
    if (isValid())
        s.sprintf("%.4f, %.4f, %.4f, %.4f", m_x, m_y, m_z, m_w);
    else
        s = QStringLiteral("empty");
    return QStringLiteral("Point4fEntity(") + s + QStringLiteral(")");
}

QDebug operator <<(QDebug dbg, const Point4fEntity &pt)
{
    dbg.nospace() << pt.toString();
    return dbg.maybeSpace();
}
