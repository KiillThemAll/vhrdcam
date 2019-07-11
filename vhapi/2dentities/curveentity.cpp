#include "curveentity.h"

CurveEntity::CurveEntity()
{
}

CurveEntity::CurveEntity(const CurveEntity &other) :
    m_curve(other.m_curve)
{
}

CurveEntity::~CurveEntity()
{
}

void CurveEntity::addCurve(const QVariant &curve)
{
    m_curve = curve;
}

void CurveEntity::addPolyline(const PolylineEntity &polyline)
{
    m_curve.setValue<PolylineEntity>(polyline);
}

bool CurveEntity::isValid()
{
    return m_curve.canConvert<PolylineEntity>();
    // m_curve.canConvert<SplineEntity>() ||
}

QString CurveEntity::toString() const
{
    return QStringLiteral("CurveEntity(") + m_curve.toString() + QStringLiteral(")");
}

bool CurveEntity::isClosed() const
{
    if (m_curve.canConvert<PolylineEntity>())
        return m_curve.value<PolylineEntity>().isClosed();
    return false;
}

QRectF CurveEntity::boundingBox() const
{
    if (m_curve.canConvert<PolylineEntity>())
        return m_curve.value<PolylineEntity>().boundingBox();
    return QRectF();
}

QList<PointEntity> CurveEntity::intersect(const LineEntity &line) const
{
    if (m_curve.canConvert<PolylineEntity>())
        return m_curve.value<PolylineEntity>().intersect(line);
    return QList<PointEntity>();
}

QDebug operator <<(QDebug dbg, const CurveEntity &curve)
{
    dbg.nospace() << curve.toString();
    return dbg.maybeSpace();
}
