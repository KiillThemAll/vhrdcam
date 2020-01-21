#include "dxfio.h"
#include "dxfioentitiestxport.h"
#include "vhapi/2dentities/markerentity.hpp"
#include <QDebug>

DxfIO::DxfIO(QObject *parent) : QObject(parent)
{
    m_entitiesTxPort = new DxfIOEntitiesTxPort(this);
    m_txports.insert("entities", m_entitiesTxPort);
}

QHash<QString, TxPort *> DxfIO::txports() const
{
    return m_txports;
}

void DxfIO::syncReceived(const QByteArray &marker)
{
    if (marker == "entities/start") {
        qDebug() << "DxfIO::syncReceived: start";
        m_entitiesTxPort->frameStarted();
    } else if (marker == "entities/end") {
        qDebug() << "DxfIO::syncReceived: end without items";
        m_entitiesTxPort->frameEnded(QVariantList());

        MarkerEntity m;
        m.m_metadata.insert("source", "dxfio");
        m.m_metadata.insert("end", QVariant());
        QVariant v;
        v.setValue<MarkerEntity>(m);
        emit m_entitiesTxPort->substreamItemReceived(v);
    }
}

void DxfIO::multipartReceived(const QByteArray &marker, const QByteArray &block)
{
    //qDebug() << "DxfIO::deserialize from" << source;
    if (marker == "entities") { // call via m_txports, make deserialize virtual?
        m_entitiesTxPort->deserialize(block);
    }
}

void DxfIO::load()
{
    const char *file = "/home/woodenprint/Desktop/test.dxf";
    QByteArray mpacked;
    mpacked.append(0x81);

    mpacked.append(0xa4);
    mpacked.append("file");

    mpacked.append(0xd9);
    mpacked.append(strlen(file));
    mpacked.append(file);

    qDebug() << "DxfIO::load req sent";
    emit serialized("slotsio", mpacked);
}
