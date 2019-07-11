#ifndef LINEENTITY_HPP
#define LINEENTITY_HPP

#include <qobjectdefs.h>
#include <QDebug>
#include "pointentity.hpp"

class LineEntity
{
    Q_GADGET
    Q_PROPERTY(PointEntity a MEMBER m_a)
    Q_PROPERTY(PointEntity b MEMBER m_b)
    Q_PROPERTY(float thickness MEMBER m_thickness)
public:
    LineEntity();
    LineEntity(const LineEntity &other);
    ~LineEntity();

    LineEntity(PointEntity a, PointEntity b);
    LineEntity(PointEntity a, PointEntity b, float thickness);

    bool isValid() const;

    QString toString() const;

    PointEntity m_a;
    PointEntity m_b;
    float m_thickness;
};
Q_DECLARE_METATYPE(LineEntity)

QDebug operator << (QDebug dbg, const LineEntity &entity);

#endif // LINEENTITY_HPP
