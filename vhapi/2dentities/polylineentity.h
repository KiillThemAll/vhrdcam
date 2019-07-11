#ifndef POLYLINEENTITY_H
#define POLYLINEENTITY_H

#include "pointentity.hpp"
#include "lineentity.hpp"
#include <QRectF>
#include <QColor>

struct polyline_vertex_t {
    PointEntity point;
    float thickness;
};

class PolylineEntity
{
public:
    PolylineEntity();
    PolylineEntity(const PolylineEntity &other);
    ~PolylineEntity();

    void setColor(quint8 defaultR, quint8 defaultG, quint8 defaultB);
    void setColor(const QColor &defaultColor);
    void setThickness(float defaultThickness);
    void addPoint(const PointEntity &point);
    void addPoint(const QPointF &point);
    void setRect(const QRectF &rect);
    QList<PointEntity> intersect(const LineEntity &line) const;

    QString toString() const;
    bool isClosed() const;
    QRectF boundingBox() const;

    QVector<polyline_vertex_t> m_points;

private:
    quint8 m_defaultR;
    quint8 m_defaultG;
    quint8 m_defaultB;
    float m_defaultThickness;
};
Q_DECLARE_METATYPE(PolylineEntity)

QDebug operator <<(QDebug dbg, const PolylineEntity &pl);

#endif // POLYLINEENTITY_H
