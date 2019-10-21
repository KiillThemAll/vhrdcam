#include "d2entityrenderer.h"
#include "vhapi/d2entityrenderervhapi.h"
#include "vhapi/2dentities/lineentity.hpp"
#include "vhapi/2dentities/pointentity.hpp"
#include "vhapi/2dentities/markerentity.hpp"
#include "vhapi/2dentities/polylineentity.h"
#include "vhapi/2dentities/bsplineentity.h"
#include "vhapi/2dentities/groupentity.h"
#include "vhapi/2dentities/arcentity.h"
#include "vhapi/d2entityrendererentitiesioportvhapi.h"
#include "thickline.h"
#include "thicklinegeometry.h"
#include <cmath>

D2EntityRenderer::D2EntityRenderer(QObject *parent) : QObject(parent),
    m_targetEntity(nullptr)
{
    m_api = new D2EntityRendererVHAPI(this);

    m_entitiesPort = qobject_cast<D2EntityRendererEntitiesIOPortVHAPI *>(m_api->rxports().value("entities", nullptr));
    if (m_entitiesPort) {
        qDebug() << "found entities port";
        connect(m_entitiesPort, &D2EntityRendererEntitiesIOPortVHAPI::substreamItemReceived,
                this, &D2EntityRenderer::onEntityArrived);
    }
}

Qt3DCore::QEntity *D2EntityRenderer::targetEntity() const
{
    return m_targetEntity;
}

void D2EntityRenderer::setTargetEntity(Qt3DCore::QEntity *targetEntity)
{
    if (targetEntity != m_targetEntity)
        emit targetEntityChanged();
    m_targetEntity = targetEntity;
}

void D2EntityRenderer::onEntityArrived(const QVariant &entity)
{
    renderEntity(entity);
}

void D2EntityRenderer::interpolate(ThickLine *line, const BSplineEntity &bspline)
{
    quint32 segments = bspline.m_ctrlPoints.length() * 10;
    float dti = 1.0f / segments;

    for (float t = 0.0; t <= 1.0; t += dti) {
        PointEntity p = bspline.interpolate(t);

        line->geometry()->addPoint(p.m_x, p.m_y, 0.0f, 0xcc, 0xcc, 0xcc, 1.0f);
    }
}

void D2EntityRenderer::interpolateArc(ThickLine *line, const ArcEntity &arc)
{
    float t = 0;
    PointEntity p;
    quint32 steps = 20;
    float dt = 1.0f / steps;
    for (quint32 i = 0; i <= steps; i++) {
        p = arc.interpolate(t);
        line->geometry()->addPoint(p.m_x, p.m_y, 0.0f, 0, 255, 0, 1.0f);
        t += dt;
    }

}

void D2EntityRenderer::renderEntity(const QVariant &entity)
{
    if (entity.canConvert<MarkerEntity>()) {
        MarkerEntity marker = entity.value<MarkerEntity>();
        QString source = marker.m_metadata.value("source", QString("n/a")).toString();
        m_lastMarkerSource = source;
        qDebug() << "D2EntityRenderer:got a marker from" << source;
        ThickLine *l = m_thickLineEntities.value(source, nullptr);

        if (!l) {
            qDebug() << "D2EntityRenderer:creating ThickLine for" << source;
            if (m_targetEntity) {
                qDebug() << "D2EntityRenderer:appending to target entity";
                ThickLine *l = new ThickLine(m_targetEntity);
                l->geometry()->clear();
                m_thickLineEntities[source] = l;
                if (marker.m_metadata.contains("dz")) {
                    float dz = marker.m_metadata["dz"].toFloat();
                    qDebug() << "shifting by" << dz;
                    l->geometry()->setdz(dz);
                }
            }
        } else {
            if (marker.m_metadata.contains("end")) {
                qDebug() << "D2EntityRenderer:flushing";
                l->geometry()->flush();
            } else {
                qDebug() << "D2EntityRenderer:clearing";
                l->geometry()->clear();
            }
        }
        return;
    }

    if (entity.canConvert<LineEntity>()) {
        LineEntity line = entity.value<LineEntity>();
        //qDebug() << "D2EntityRenderer:got a line!" << line;
        ThickLine *l = m_thickLineEntities.value(m_lastMarkerSource, nullptr);
        if (l) {
            //qDebug() << "D2EntityRenderer:drawing";
            ThickLineGeometry *g = l->geometry();
            g->addPoint(line.m_a.m_x, line.m_a.m_y, line.m_a.m_z,
                        line.m_a.m_r, line.m_a.m_g, line.m_a.m_b,
                        line.m_thickness);
            g->addPoint(line.m_b.m_x, line.m_b.m_y, line.m_b.m_z,
                        line.m_b.m_r, line.m_b.m_g, line.m_b.m_b,
                        line.m_thickness);
            g->breakStrip();
            //g->flush();
        }
        return;
    }

    if (entity.canConvert<PolylineEntity>()) {
        PolylineEntity polyline = entity.value<PolylineEntity>();
        //qDebug() << "D2EntityRenderer:got a polyline";
        ThickLine *l = m_thickLineEntities.value(m_lastMarkerSource, nullptr);
        if (l) {
            //qDebug() << "D2EntityRenderer:drawing" << polyline.toString();
            ThickLineGeometry *g = l->geometry();
            foreach(const polyline_vertex_t &pt, polyline.m_points) {
                g->addPoint(pt.point.m_x, pt.point.m_y, pt.point.m_z,
                            pt.point.m_r, pt.point.m_g, pt.point.m_b,
                            pt.thickness);
            }
            g->breakStrip();
            //g->flush();
        }
        return;
    }

    if (entity.canConvert<BSplineEntity>()) {
        //return;
        BSplineEntity bspline = entity.value<BSplineEntity>();
        //qDebug() << "got bspline" << bspline;
        ThickLine *l = m_thickLineEntities.value("dxfio", nullptr);
        if (l) {
            interpolate(l, bspline);
            l->geometry()->breakStrip();
        }
        return;
    }

    if (entity.canConvert<ArcEntity>()) {
        ArcEntity arc = entity.value<ArcEntity>();
        ThickLine *l = m_thickLineEntities.value("entity_playground", nullptr);
        if (l) {
            interpolateArc(l, arc);
            l->geometry()->breakStrip();
            l->geometry()->addPoint(arc.m_center.m_x - 1, arc.m_center.m_y, 0.0f, 255, 0, 0, 1.0f);
            l->geometry()->addPoint(arc.m_center.m_x + 1, arc.m_center.m_y, 0.0f, 255, 0, 0, 1.0f);
            l->geometry()->addPoint(arc.m_center.m_x, arc.m_center.m_y - 1, 0.0f, 255, 0, 0, 1.0f);
            l->geometry()->addPoint(arc.m_center.m_x, arc.m_center.m_y + 1, 0.0f, 255, 0, 0, 1.0f);
            l->geometry()->breakStrip();
        }
        return;
    }

    if (entity.canConvert<GroupEntity>()) {
        GroupEntity group = entity.value<GroupEntity>();
        foreach (const QVariant &e, group.m_entities) {
            renderEntity(e);
        }
        return;
    }

    qDebug() << "D2EntityRenderer::renderEntity can't render" << entity.toString();
}
