#include "lineentity.hpp"

LineEntity::LineEntity()
{
}

LineEntity::LineEntity(const LineEntity &other) :
    m_a(other.m_a), m_b(other.m_b), m_thickness(other.m_thickness)
{
}

LineEntity::~LineEntity()
{
}

LineEntity::LineEntity(PointEntity a, PointEntity b) :
    m_a(a), m_b(b), m_thickness(1)
{
}

LineEntity::LineEntity(PointEntity a, PointEntity b, float thickness) :
    m_a(a), m_b(b), m_thickness(thickness)
{
}

bool LineEntity::isValid() const
{
    return m_a.isValid() && m_b.isValid();
}

QString LineEntity::toString() const
{
    if (!isValid())
        return QStringLiteral("LineEntity(empty)");
    QString t;
    t.sprintf("%.2f, ", m_thickness);
    return QStringLiteral("LineEntity(") + t + m_a.toString() + QStringLiteral(", ") + m_b.toString() + QStringLiteral(")");
}

QDebug operator <<(QDebug dbg, const LineEntity &entity)
{
    dbg.nospace() << entity.toString();
    return dbg.maybeSpace();
}
