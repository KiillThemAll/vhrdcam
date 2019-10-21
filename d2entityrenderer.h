#ifndef D2ENTITYRENDERER_H
#define D2ENTITYRENDERER_H

#include "vhapi/2dentities/baseentity.hpp"
#include "vhapi/2dentities/bsplineentity.h"
#include "vhapi/2dentities/arcentity.h"

#include <QObject>
#include <QHash>

namespace Qt3DCore {
class QEntity;
}

class D2EntityRendererVHAPI;
class D2EntityRendererEntitiesIOPortVHAPI;
class ThickLine;

class D2EntityRenderer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Qt3DCore::QEntity* targetEntity READ targetEntity WRITE setTargetEntity NOTIFY targetEntityChanged)
public:
    explicit D2EntityRenderer(QObject *parent = nullptr);

    Qt3DCore::QEntity* targetEntity() const;
    void setTargetEntity(Qt3DCore::QEntity *targetEntity);

    D2EntityRendererVHAPI *m_api;
    D2EntityRendererEntitiesIOPortVHAPI *m_entitiesPort;
signals:
    void targetEntityChanged();


public slots:

private slots:
    void onEntityArrived(const QVariant &entity);

private:
    void interpolate(ThickLine *line, const BSplineEntity &bspline);
    void interpolateArc(ThickLine *line, const ArcEntity &arc);
    void renderEntity(const QVariant &entity);

    Qt3DCore::QEntity* m_targetEntity;
    QHash<QString, ThickLine *> m_thickLineEntities;
    QString m_lastMarkerSource;
};

#endif // D2ENTITYRENDERER_H
