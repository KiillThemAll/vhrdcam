#ifndef CURVEENTITY_H
#define CURVEENTITY_H

#include <QVariant>
#include <QDebug>
#include "lineentity.hpp"
#include "polylineentity.h"
//#include "splineentity.h"
//#include "circleentity.h"

// curve can contain several touching polylines, splines?
class CurveEntity
{
public:
    CurveEntity();
    CurveEntity(const CurveEntity &other);
    ~CurveEntity();

    void addCurve(const QVariant &curve);
    void addPolyline(const PolylineEntity &polyline);

    bool isValid();
    QString toString() const;
    bool isClosed() const;
    QRectF boundingBox() const;
    QList<PointEntity> intersect(const LineEntity &line) const;

    QVariant m_curve;
};
Q_DECLARE_METATYPE(CurveEntity);

QDebug operator <<(QDebug dbg, const CurveEntity &curve);

#endif // CURVEENTITY_H
