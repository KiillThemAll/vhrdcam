#ifndef GROUPENTITY_H
#define GROUPENTITY_H

#include <qobjectdefs.h>
#include <QDebug>

class GroupEntity
{
    Q_GADGET
    Q_PROPERTY(QVariantList properties MEMBER m_entities)
public:
    GroupEntity();
    GroupEntity(const GroupEntity &other);
    ~GroupEntity();

    QString toString() const;
    bool isValid() const;

    QVariantList m_entities;
};
Q_DECLARE_METATYPE(GroupEntity);

QDebug operator <<(QDebug dbg, const GroupEntity &ge);

#endif // GROUPENTITY_H
