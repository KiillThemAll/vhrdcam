#include "pointentity.hpp"

#include <cmath>
#include <limits>

PointEntity::PointEntity() :
    m_r(0), m_g(0), m_b(0)
{
    m_x = std::numeric_limits<float>::quiet_NaN();
    m_y = std::numeric_limits<float>::quiet_NaN();
    m_z = std::numeric_limits<float>::quiet_NaN();
}

PointEntity::PointEntity(const PointEntity &other) :
    m_x(other.m_x), m_y(other.m_y), m_z(other.m_z),
    m_r(other.m_r), m_g(other.m_g), m_b(other.m_b)
{
}

PointEntity::~PointEntity()
{
}

PointEntity::PointEntity(float x, float y, float z) :
    m_x(x), m_y(y), m_z(z), m_r(0), m_g(0), m_b(0)
{
}

PointEntity::PointEntity(float x, float y, float z, quint8 r, quint8 g, quint8 b) :
    m_x(x), m_y(y), m_z(z), m_r(r), m_g(g), m_b(b)
{
}

bool PointEntity::isValid() const
{
    return !std::isnan(m_x);
}

bool PointEntity::operator ==(const PointEntity &other) const
{
    return m_x == other.m_x &&
            m_y == other.m_y &&
            m_z == other.m_z;
}

bool PointEntity::operator !=(const PointEntity &other) const
{
    return !(*this == other);
}

QString PointEntity::toString() const
{
    QString s;
    if (isValid())
        s.sprintf("%.4f, %.4f, %.4f, %d, %d, %d", m_x, m_y, m_z, m_r, m_g, m_b);
    else
        s = QStringLiteral("empty");
    return QStringLiteral("PointEntity(") + s + QStringLiteral(")");
}

QDebug operator <<(QDebug dbg, const PointEntity &pt)
{
    dbg.nospace() << pt.toString();
    return dbg.maybeSpace();
}
