#ifndef POINTENTITY_HPP
#define POINTENTITY_HPP

#include <qobjectdefs.h>
#include <QString>
#include <QtDebug>

class PointEntity {
    Q_GADGET
    Q_PROPERTY(float x MEMBER m_x)
    Q_PROPERTY(float y MEMBER m_y)
    Q_PROPERTY(float z MEMBER m_z)
    Q_PROPERTY(quint8 r MEMBER m_r)
    Q_PROPERTY(quint8 g MEMBER m_g)
    Q_PROPERTY(quint8 b MEMBER m_b)
public:
    PointEntity();
    PointEntity(const PointEntity &other);
    ~PointEntity();
    PointEntity(float x, float y, float z);
    PointEntity(float x, float y, float z, quint8 r, quint8 g, quint8 b);

    bool isValid() const;
    bool operator == (const PointEntity &other) const;
    bool operator != (const PointEntity &other) const;
    QString toString() const;

    float m_x;
    float m_y;
    float m_z;
    quint8 m_r;
    quint8 m_g;
    quint8 m_b;
};
Q_DECLARE_METATYPE(PointEntity)

QDebug operator <<(QDebug dbg, const PointEntity &pt);

#endif // POINTENTITY_HPP
