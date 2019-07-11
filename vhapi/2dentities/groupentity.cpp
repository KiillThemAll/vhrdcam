#include "groupentity.h"
#include "markerentity.hpp"
#include "polylineentity.h"
#include "pointentity.hpp"
#include "lineentity.hpp"

#include <QStringList>

GroupEntity::GroupEntity()
{
}

GroupEntity::GroupEntity(const GroupEntity &other) :
    m_entities(other.m_entities)
{
}

GroupEntity::~GroupEntity()
{
}

QString GroupEntity::toString() const
{
    QStringList s;
    if (m_entities.isEmpty()) {
        s << QStringLiteral("empty");
    } else {
        foreach (const QVariant &entity, m_entities) {
            s << entity.toString();
        }
    }

    return QStringLiteral("GroupEntity(") + s.join(", ") + QStringLiteral(")");
}

bool GroupEntity::isValid() const
{
    return !m_entities.isEmpty();
}

QDebug operator <<(QDebug dbg, const GroupEntity &ge)
{
    dbg.nospace() << ge.toString();
    return dbg.maybeSpace();
}
