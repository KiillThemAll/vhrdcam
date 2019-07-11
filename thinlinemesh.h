#ifndef THINLINEMESH_H
#define THINLINEMESH_H

#include <Qt3DExtras/qt3dextras_global.h>
#include <Qt3DRender/qgeometryrenderer.h>
#include <QtCore/QSize>

class ThinLineMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
    Q_PROPERTY(float width READ width WRITE setWidth NOTIFY widthChanged)
public:
    explicit ThinLineMesh(Qt3DCore::QNode *parent = nullptr);
    ~ThinLineMesh();

    float width();

public slots:
    void setWidth(float width);

signals:
    void widthChanged(float width);
};

#endif // THINLINEMESH_H
