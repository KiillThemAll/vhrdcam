#ifndef DXFIOENTITIESTXPORT_H
#define DXFIOENTITIESTXPORT_H

#include <QObject>

#include "txport.h"
#include "2dentities/baseentity.hpp"

class DxfIO;
// direction: out, substream: true, base QList<BaseEntitty>
class DxfIOEntitiesTxPort : public TxPort
{
    Q_OBJECT
public:
    explicit DxfIOEntitiesTxPort(QObject *parent = nullptr);

signals:


public slots:

private:
    void deserialize(const QByteArray &block);

    friend class DxfIO;
};

#endif // DXFIOENTITIESTXPORT_H
