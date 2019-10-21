#ifndef BSPLINEENTITY_H
#define BSPLINEENTITY_H

#include "point4fentity.h"
#include "pointentity.hpp"

class BSplineEntity
{
public:
    BSplineEntity();
    BSplineEntity(const BSplineEntity &other);
    ~BSplineEntity();

    QString toString() const;

    QVector<float> m_knots;
    QVector<Point4fEntity> m_ctrlPoints;
    quint8 m_order;

    PointEntity interpolate(float t) const;

    typedef struct {
        float x;
        float y;
        float w;
    } coord_t;
    mutable QVector<coord_t> m_v;
};
Q_DECLARE_METATYPE(BSplineEntity)

QDebug operator <<(QDebug dbg, const BSplineEntity &pl);


#endif // BSPLINEENTITY_H
