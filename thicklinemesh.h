#ifndef THICKLINEMESH_H
#define THICKLINEMESH_H

#include <Qt3DExtras/qt3dextras_global.h>
#include <Qt3DRender/qgeometryrenderer.h>
#include <QtCore/QSize>

class ThickLineGeometry;

class ThickLineMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
    Q_PROPERTY(float width READ width WRITE setWidth NOTIFY widthChanged)
public:
    explicit ThickLineMesh(Qt3DCore::QNode *parent = nullptr);
    ~ThickLineMesh();

    float width();

    ThickLineGeometry* geometry() const;

public slots:
    void setWidth(float width);

signals:
    void widthChanged(float width);

private:
    ThickLineGeometry *m_geometry;
};

#endif // THICKLINEMESH_H
