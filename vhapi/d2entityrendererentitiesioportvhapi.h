#ifndef D2ENTITYRENDERERENTITIESIOPORTVHAPI_H
#define D2ENTITYRENDERERENTITIESIOPORTVHAPI_H

#include "2dentities/baseentity.hpp"
#include "rxport.h"

#include <QObject>

// direction: in, substream: true, base: QList<BaseEntity>
class D2EntityRendererEntitiesIOPortVHAPI : public RxPort
{
    Q_OBJECT
public:
    explicit D2EntityRendererEntitiesIOPortVHAPI(QObject *parent = nullptr);



    // normally signals would be generated internally or connected to other ioports siganls?

signals:
    void substreamItemReceived(const QVariant &entity);

public slots:
};

#endif // D2ENTITYRENDERERENTITIESIOPORTVHAPI_H
