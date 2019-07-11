#ifndef SHAPEENTITY_H
#define SHAPEENTITY_H

#include <QVariant>
#include <QDebug>
#include "curveentity.h"
#include "lineentity.hpp"
#include <QRectF>

class ShapeEntity
{
    Q_GADGET
    //Q_PROPERTY(QVariantList curves MEMBER m_curves)
public:
    ShapeEntity();
    ShapeEntity(const ShapeEntity &other);
    ~ShapeEntity();

    void addCurve(const CurveEntity &curve);

    QString toString() const;
    bool isValid();
    QRectF boundingBox() const;
    QList<PointEntity> intersect(const LineEntity &line) const;

    QList<CurveEntity> m_curves;
};
Q_DECLARE_METATYPE(ShapeEntity);

QDebug operator <<(QDebug dbg, const ShapeEntity &shape);

#endif // SHAPEENTITY_H
