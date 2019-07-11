#include "shapeentity.h"

ShapeEntity::ShapeEntity()
{
}

ShapeEntity::ShapeEntity(const ShapeEntity &other) :
    m_curves(other.m_curves)
{
}

ShapeEntity::~ShapeEntity()
{
}

void ShapeEntity::addCurve(const CurveEntity &curve)
{
    m_curves.append(curve);
}

QString ShapeEntity::toString() const
{
    QStringList s;
    if (m_curves.isEmpty()) {
        s << QStringLiteral("empty");
    } else {
        foreach (const CurveEntity &curve, m_curves) {
            s << curve.toString();
        }
    }

    return QStringLiteral("ShapeEntity(") + s.join(", ") + QStringLiteral(")");
}

bool ShapeEntity::isValid()
{
    return !m_curves.isEmpty();
}

QRectF ShapeEntity::boundingBox() const
{
    float minx = 1000000.0f;
    float maxx = -1000000.0f;
    float miny = 1000000.0f;
    float maxy = -1000000.0f;

    foreach (const CurveEntity &curve, m_curves) {
        QRectF box = curve.boundingBox();
        if (box.bottom() > maxy)
            maxy = box.bottom();
        if (box.top() < miny)
            miny = box.top();
        if (box.right() > maxx)
            maxx = box.right();
        if (box.left() < minx)
            minx = box.left();
    }

    return QRectF(minx, miny, maxx - minx, maxy - miny);
}

QList<PointEntity> ShapeEntity::intersect(const LineEntity &line) const
{
    QList<PointEntity> points;
    foreach (const CurveEntity &curve, m_curves) {
        points << curve.intersect(line);
    }
    return points;
}

QDebug operator <<(QDebug dbg, const ShapeEntity &shape)
{
    dbg.nospace() << shape.toString();
    return dbg.maybeSpace();
}
