#ifndef D2ENTITYRENDERERVHAPI_H
#define D2ENTITYRENDERERVHAPI_H

#include "rxport.h"

#include <QObject>
#include <QHash>

class D2EntityRendererVHAPI : public QObject
{
    Q_OBJECT
public:
    explicit D2EntityRendererVHAPI(QObject *parent = nullptr);

    QHash<QString, RxPort *> rxports() const;

private:
    QHash<QString, RxPort *> m_rxports;
};

#endif // D2ENTITYRENDERERVHAPI_H
