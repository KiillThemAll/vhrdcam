#include "arcentity.h"
#include <cmath>

ArcEntity::ArcEntity()
{
    m_radius = 0;
    m_theta0 = 0;
    m_theta1 = 0;
}

ArcEntity::ArcEntity(PointEntity a, PointEntity b, PointEntity c)
{
    PointEntity d1 = PointEntity(b.m_y - a.m_y, a.m_x - b.m_x, 0);
    PointEntity d2 = PointEntity(c.m_y - a.m_y, a.m_x - c.m_x, 0);
    float k = d2.m_x * d1.m_y - d2.m_y * d1.m_x;
    if (fabs(k) < 0.00001)
        return;
    PointEntity s1 = PointEntity((a.m_x + b.m_x) / 2, (a.m_y + b.m_y) / 2, 0);
    PointEntity s2 = PointEntity((a.m_x + c.m_x) / 2, (a.m_y + c.m_y) / 2, 0);
    float l = d1.m_x * (s2.m_y - s1.m_y) - d1.m_y * (s2.m_x - s1.m_x);
    float m = l / k;
    m_center = PointEntity(s2.m_x + m * d2.m_x, s2.m_y + m * d2.m_y, 0);
    float dx = m_center.m_x - a.m_x;
    float dy = m_center.m_y - a.m_y;
    m_radius = sqrtf(dx * dx + dy * dy);
    m_theta0 = atan2((a.m_y - m_center.m_y), (a.m_x - m_center.m_x));
    m_theta1 = atan2((b.m_y - m_center.m_y), (b.m_x - m_center.m_x));
    float m_theta2 = atan2((c.m_y - m_center.m_y), (c.m_x - m_center.m_x));
    if (m_theta0 < 0)
        m_theta0 = 2 * M_PI + m_theta0;
    if (m_theta1 < 0)
        m_theta1 = 2 * M_PI + m_theta1;
    if (m_theta2 < 0)
        m_theta2 = 2 * M_PI + m_theta2;

    bool g10 = m_theta1 > m_theta0;
    bool g21 = m_theta2 > m_theta1;
    bool g20 = m_theta2 > m_theta0;
    quint8 comb = 4 * g10 + 2 * g21 + g20;
    switch (comb) {
    case 0: m_theta1 = m_theta0; m_theta0 = m_theta2; break; // CW, no wraps
    case 1:  break; // not possible
    case 2: m_theta1 = m_theta2 + 2 * M_PI; break; // CCW, bc wrapped
    case 3: m_theta1 = m_theta0 + 2 * M_PI; m_theta0 = m_theta2; break; // CW, c wrapped
    case 4: m_theta1 = m_theta2 + 2 * M_PI; break; // CCW, c wrapped
    case 5: m_theta1 = m_theta0 + 2 * M_PI; m_theta0 = m_theta2; break; // CW, bc wrapped
    case 6:  break; // not possible
    case 7: m_theta1 = m_theta2; break; // CCW, no wraps
    }
}

ArcEntity::ArcEntity(const ArcEntity &other) :
    m_center(other.m_center), m_radius(other.m_radius),
    m_theta0(other.m_theta0), m_theta1(other.m_theta1)
{
}

ArcEntity::~ArcEntity()
{
}

QString ArcEntity::toString() const
{
    QString s;
    if (isValid())
        s.sprintf(", r=%.4f, t0=(%.4f)%.4f, t1=(%.4f)%.4f", m_radius, m_theta0, m_theta0 * 180 / M_PI, m_theta1, m_theta1 * 180 / M_PI);
    else
        s = QStringLiteral("empty");
    return QStringLiteral("ArcEntity(") + m_center.toString() + s + QStringLiteral(")");
}

bool ArcEntity::isValid() const
{
    return m_center.isValid() && m_radius > 0;
}

PointEntity ArcEntity::interpolate(float t) const
{
    float dt = m_theta1 - m_theta0;
    float ts = m_theta0 + t * dt;
    return PointEntity(m_center.m_x + cosf(ts) * m_radius,
                       m_center.m_y + sinf(ts) * m_radius, 0);
}

QDebug operator <<(QDebug dbg, const ArcEntity &arc)
{
    dbg.nospace() << arc.toString();
    return dbg.maybeSpace();
}
