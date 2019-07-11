#include "markerentity.hpp"

MarkerEntity::MarkerEntity()
{
}

MarkerEntity::MarkerEntity(const MarkerEntity &other) :
    m_metadata(other.m_metadata)
{
}

MarkerEntity::~MarkerEntity()
{
}

MarkerEntity::MarkerEntity(const QVariantHash &metadata) :
    m_metadata(metadata)
{
}

QString MarkerEntity::toString() const
{
    if (!isValid())
        return QStringLiteral("MarkerEntity(empty)");
    QStringList s;
    QVariantHash::const_iterator it = m_metadata.constBegin();
    while (it != m_metadata.constEnd()) {
        s << it.key() + "=" + it.value().toString();
    }
    return QStringLiteral("MarkerEntity(") + s.join(", ") + QStringLiteral(")");
}

bool MarkerEntity::isValid() const
{
    return !m_metadata.isEmpty();
}

QDebug operator <<(QDebug dbg, const MarkerEntity &m)
{
    dbg.nospace() << m.toString();
    return dbg.maybeSpace();
}
