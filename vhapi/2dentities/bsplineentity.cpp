#include "bsplineentity.h"

BSplineEntity::BSplineEntity() :
    m_order(0)
{
}

BSplineEntity::BSplineEntity(const BSplineEntity &other) :
    m_knots(other.m_knots), m_ctrlPoints(other.m_ctrlPoints), m_order(other.m_order)
{
}

BSplineEntity::~BSplineEntity()
{
}

QString BSplineEntity::toString() const
{
    QString s;
    if (m_order == 0) {
        s = QStringLiteral("empty");
    } else {
        s += QString().sprintf("order: %d, ", m_order);
        s += QString().sprintf("knots: %d[", m_knots.length());
        foreach (float k, m_knots) {
            s += QString().sprintf("%.4f", k) + QStringLiteral(", ");
        }
        s += QString().sprintf("] ctrl: %d[", m_ctrlPoints.length());
        foreach (const Point4fEntity &cp, m_ctrlPoints) {
            s += cp.toString() + QStringLiteral(", ");
        }
        s += "]";
    }

    return QStringLiteral("BSplineEntity(") + s + QStringLiteral(")");
}

PointEntity BSplineEntity::interpolate(float t) const
{
    m_v.resize(m_ctrlPoints.length());

    for (int i = 0; i < m_ctrlPoints.length(); ++i) {
        m_v[i].x = m_ctrlPoints[i].m_x;
        m_v[i].y = m_ctrlPoints[i].m_y;
        m_v[i].w = 1.0f;
    }

    quint32 degree = m_order - 1;
    float low = m_knots[degree];
    float high = m_knots[m_knots.length() - 1 - degree];
    float ts = t * (high - low) + low;

    quint32 s;
    for (s = degree; s < m_knots.length() - 1 - degree; s++) {
        if (ts >= m_knots[s] && ts <= m_knots[s + 1])
            break;
    }

    float alpha;
    for (quint32 l = 1; l <= degree + 1; l++) {
        for (quint32 i = s; i > s - degree - 1 + l; i--) {
            alpha = (ts - m_knots[i]) / (m_knots[i + degree + 1 - l] - m_knots[i]);

            m_v[i].x = (1 - alpha) * m_v[i - 1].x + alpha * m_v[i].x;
            m_v[i].y = (1 - alpha) * m_v[i - 1].y + alpha * m_v[i].y;
            m_v[i].w = (1 - alpha) * m_v[i - 1].w + alpha * m_v[i].w;
        }
    }

    return PointEntity(m_v[s].x / m_v[s].w, m_v[s].y / m_v[s].w, 0.0f);
}

QDebug operator <<(QDebug dbg, const BSplineEntity &pl)
{
    dbg.nospace() << pl.toString();
    return dbg.maybeSpace();
}
