#ifndef ARCENTITY_H
#define ARCENTITY_H
#include "pointentity.hpp"

class ArcEntity
{
public:
    ArcEntity();
    ArcEntity(PointEntity a, PointEntity b, PointEntity c);
    ArcEntity(const ArcEntity &other);
    ~ArcEntity();

    QString toString() const;
    bool isValid() const;
    PointEntity interpolate(float t) const;

    PointEntity m_center;
    float m_radius;
    float m_theta0;
    float m_theta1;
};
Q_DECLARE_METATYPE(ArcEntity)

QDebug operator <<(QDebug dbg, const ArcEntity &arc);

#endif // ARCENTITY_H
