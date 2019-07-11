#include "d2entityrenderervhapi.h"
#include "d2entityrendererentitiesioportvhapi.h"

D2EntityRendererVHAPI::D2EntityRendererVHAPI(QObject *parent) : QObject(parent)
{
    m_rxports.insert("entities", new D2EntityRendererEntitiesIOPortVHAPI(this));
}

QHash<QString, RxPort *> D2EntityRendererVHAPI::rxports() const
{
    return m_rxports;
}
