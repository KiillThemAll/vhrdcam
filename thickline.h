#ifndef THICKLINE_H
#define THICKLINE_H

#include <QEntity>

class ThickLineMesh;

namespace Qt3DCore {
    class QTransform;
}

namespace Qt3DRender {
    class QMaterial;
    class QLayer;
}

class ThickLineGeometry;

class ThickLine : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QLayer *layer READ layer WRITE setLayer NOTIFY layerChanged)
public:
    ThickLine(Qt3DCore::QNode *parent = nullptr);
    ~ThickLine();

    Q_INVOKABLE void call_smth_new();

    void setLayer(Qt3DRender::QLayer *layer);
    Qt3DRender::QLayer* layer() const;

    ThickLineGeometry * geometry() const;

signals:
    void layerChanged();

private:
    ThickLineMesh *m_mesh;
    Qt3DCore::QTransform *m_transform;
    Qt3DRender::QMaterial *m_material;
};

#endif // THICKLINE_H
