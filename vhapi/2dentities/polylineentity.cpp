#include "polylineentity.h"
#include <QLineF>

PolylineEntity::PolylineEntity()
{
}

PolylineEntity::PolylineEntity(const PolylineEntity &other) :
    m_points(other.m_points)
{
}

PolylineEntity::~PolylineEntity()
{
}

void PolylineEntity::setColor(quint8 defaultR, quint8 defaultG, quint8 defaultB)
{
    m_defaultR = defaultR;
    m_defaultG = defaultG;
    m_defaultB = defaultB;
}

void PolylineEntity::setColor(const QColor &color)
{
    m_defaultR = color.red();
    m_defaultG = color.green();
    m_defaultB = color.blue();
}

void PolylineEntity::setThickness(float defaultThickness)
{
    m_defaultThickness = defaultThickness;
}

void PolylineEntity::addPoint(const PointEntity &point)
{
    polyline_vertex_t v;
    v.point = point;
    v.point.m_r = m_defaultR;
    v.point.m_g = m_defaultG;
    v.point.m_b = m_defaultB;
    v.thickness = m_defaultThickness;
    m_points.append(v);
}

void PolylineEntity::addPoint(const QPointF &point)
{
    polyline_vertex_t v;
    v.point = PointEntity(point.x(), point.y(), 0.0f, m_defaultR, m_defaultG, m_defaultB);
    v.thickness = m_defaultThickness;
    m_points.append(v);
}

void PolylineEntity::setRect(const QRectF &rect)
{
    addPoint(rect.topLeft());
    addPoint(rect.topRight());
    addPoint(rect.bottomRight());
    addPoint(rect.bottomLeft());
    addPoint(rect.topLeft());
}

QList<PointEntity> PolylineEntity::intersect(const LineEntity &line) const
{
    QLineF l1(line.m_a.m_x, line.m_a.m_y, line.m_b.m_x, line.m_b.m_y);
    QRectF box = boundingBox();
    QLineF d1 = QLineF(box.bottomLeft(), box.topRight());
    QLineF d2 = QLineF(box.topLeft(), box.bottomRight());
    QPointF pt;
    QLineF::IntersectType t1 = l1.intersect(d1, &pt);
    QLineF::IntersectType t2 = l1.intersect(d2, &pt);
    if (!( t1 == QLineF::IntersectType::BoundedIntersection &&
           t2 == QLineF::IntersectType::BoundedIntersection) ) {
        return QList<PointEntity>();
    }

    QList<PointEntity> points;
    for (qint32 i = 0, j = 1; i < m_points.length(); i++, j++) {
        if (j == m_points.length())
            j = 0;
        PointEntity p1 = m_points[i].point;
        PointEntity p2 = m_points[j].point;
        QLineF l2(p1.m_x, p1.m_y, p2.m_x, p2.m_y);

        QPointF pt;
        QLineF::IntersectType t = l1.intersect(l2, &pt);
        PointEntity px = PointEntity(pt.x(), pt.y(), 0.0);
        if (t == QLineF::IntersectType::BoundedIntersection) {
            if (!points.contains(px))
                points.append(px);
        }
    }

    return points;
}

QString PolylineEntity::toString() const
{
    QString s;
    if (m_points.isEmpty()) {
        s = QStringLiteral("empty");
    } else {
        foreach (const polyline_vertex_t &pv, m_points) {
            QString t;
            t.sprintf("%.2f", pv.thickness);
            s += pv.point.toString() + QStringLiteral("[") + t + QStringLiteral("], ");
        }
    }

    return QStringLiteral("PolylineEntity(") + s + QStringLiteral(")");
}

bool PolylineEntity::isClosed() const
{
    PointEntity p0 = m_points.first().point;
    PointEntity plast = m_points.last().point;
    return p0 == plast;
}

QRectF PolylineEntity::boundingBox() const
{
    float minx = 1000000.0f;
    float maxx = -1000000.0f;
    float miny = 1000000.0f;
    float maxy = -1000000.0f;

    foreach (const polyline_vertex_t &pv, m_points) {
        float x = pv.point.m_x;
        float y = pv.point.m_y;
        if (x > maxx)
            maxx = x;
        if (x < minx)
            minx = x;
        if (y > maxy)
            maxy = y;
        if (y < miny)
            miny = y;
    }

    return QRectF(minx, miny, maxx - minx, maxy - miny);
}

QDebug operator <<(QDebug dbg, const PolylineEntity &pl)
{
    dbg.nospace() << pl.toString();
    return dbg.maybeSpace();
}
