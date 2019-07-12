#include "dxfioentitiestxport.h"
#include <QDebug>
#include "3rdparty/qmsgpack/src/msgpackstream.h"
#include "2dentities/baseentity.hpp"
#include "2dentities/polylineentity.h"
#include "2dentities/markerentity.hpp"
#include "2dentities/groupentity.h"
#include "2dentities/lineentity.hpp"

DxfIOEntitiesTxPort::DxfIOEntitiesTxPort(QObject *parent) : TxPort(parent)
{

}

const qint8 POINT_ENTITY_EXT        = 0x01;
const qint8 POLYLINE_ENTITY_EXT     = 0x02;
const qint8 MARKER_ENTITY_EXT       = 0x03;
const qint8 GROUP_ENTITY_EXT        = 0x04;

QVariant deserializeBaseEntity(MsgPackStream &s);

MsgPackStream &operator>>(MsgPackStream &s, PointEntity &pt)
{
    quint32 length;
    qint8 type;
    s.readExtHeader(length, type);
    if (type != POINT_ENTITY_EXT) {
        s.setStatus(MsgPackStream::Status::ReadCorruptData);
        return s;
    }
    s >> pt.m_x >> pt.m_y >> pt.m_z;
    s >> pt.m_r >> pt.m_g >> pt.m_b;
    return s;
}

MsgPackStream &operator>>(MsgPackStream &s, PolylineEntity &pl)
{
    quint32 length;
    qint8 type;
    s.readExtHeader(length, type);
    if (type != POLYLINE_ENTITY_EXT) {
        s.setStatus(MsgPackStream::Status::ReadCorruptData);
        return s;
    }
    s.readArrayHeader(length);
    for (quint32 i = 0; i < length; ++i) {
        polyline_vertex_t pv;
        s >> pv.point;
        s >> pv.thickness;
        pl.m_points.append(pv);
    }
    return s;
}

MsgPackStream &operator>>(MsgPackStream &s, GroupEntity &ge)
{
    quint32 length;
    qint8 type;
    s.readExtHeader(length, type);
    if (type != GROUP_ENTITY_EXT) {
        s.setStatus(MsgPackStream::Status::ReadCorruptData);
        return s;
    }
    s.readArrayHeader(length);
    for (quint32 i = 0; i < length; ++i) {
        QVariant entity = deserializeBaseEntity(s);
        ge.m_entities.append(entity);
        if (s.status() != MsgPackStream::Status::Ok)
            break;
    }
    return s;
}

QVariant deserializeBaseEntity(MsgPackStream &s)
{
    QVariant v;
    PeekResult p = s.peek();
    if (p.status != PeekResult::PeekOk) {
        s.setStatus(MsgPackStream::ReadCorruptData);
        return v;
    }
    if (p.userType == POINT_ENTITY_EXT) {
        PointEntity p;
        s >> p;
        v.setValue(p);
    } else if (p.userType == POLYLINE_ENTITY_EXT) {
        PolylineEntity pl;
        s >> pl;
        v.setValue(pl);
    } else if (p.userType == MARKER_ENTITY_EXT) {
        MarkerEntity marker;
        marker.m_metadata["source"] = "dxfio";
        v.setValue(marker);
    } else if (p.userType == GROUP_ENTITY_EXT) {
        GroupEntity ge;
        s >> ge;
        v.setValue(ge);
    }
    return v;
}

void DxfIOEntitiesTxPort::deserialize(const QByteArray &block)
{
    //qDebug() << "DxfIOEntitiesTxPort::deserialize";
    MsgPackStream s(block);
    QVariant entity = deserializeBaseEntity(s);
    if (s.status() != MsgPackStream::Status::Ok) {
        qDebug() << "DxfIOEntitiesTxPort::deserialize: garbage" << block << block.toHex();
        return;
    }

    emit substreamItemReceived(entity);
}
