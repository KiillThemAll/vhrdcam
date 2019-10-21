#ifndef POINT4FENTITY_H
#define POINT4FENTITY_H

#include <qobjectdefs.h>
#include <QString>
#include <QtDebug>

class Point4fEntity
{
public:
    Point4fEntity();
    Point4fEntity(const Point4fEntity &other);
    ~Point4fEntity();

    bool isValid() const;
    bool operator == (const Point4fEntity &other) const;
    bool operator != (const Point4fEntity &other) const;
    QString toString() const;

    float m_x;
    float m_y;
    float m_z;
    float m_w;
};
Q_DECLARE_METATYPE(Point4fEntity)

QDebug operator <<(QDebug dbg, const Point4fEntity &pt);

#endif // POINT4FENTITY_H
