#ifndef MARKERENTITY_HPP
#define MARKERENTITY_HPP

#include <qobjectdefs.h>
#include <QVariant>
#include <QHash>
#include <QDebug>

class MarkerEntity {
    Q_GADGET
    Q_PROPERTY(QVariantHash metadata MEMBER m_metadata);
public:
    MarkerEntity();
    MarkerEntity(const MarkerEntity &other);
    ~MarkerEntity();

    MarkerEntity(const QVariantHash &metadata);

    QString toString() const;
    bool isValid() const;

    QHash<QString, QVariant> m_metadata;
};
Q_DECLARE_METATYPE(MarkerEntity);

QDebug operator <<(QDebug dbg, const MarkerEntity &m);

#endif // MARKERENTITY_HPP


