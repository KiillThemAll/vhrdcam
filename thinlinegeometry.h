#ifndef THINLINEGEOMETRY_H
#define THINLINEGEOMETRY_H

#include "units.h"

#include <Qt3DExtras/qt3dextras_global.h>
#include <Qt3DRender/qgeometry.h>
//#include <Eigen/Dense>

namespace Qt3DRender {

class QBuffer;

}

class ThinLineGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT
    Q_PROPERTY(float width READ width WRITE setWidth NOTIFY widthChanged)
public:
    explicit ThinLineGeometry(QNode *parent = nullptr);
    ~ThinLineGeometry();

   //void setPath(const Eigen::Ref<const PathX2f> path);

    void updateVertices();
    void updateIndices();

    float width();

    Qt3DRender::QAttribute *positionAttribute() const;
    Qt3DRender::QAttribute *normalAttribute() const;
    Qt3DRender::QAttribute *texCoordAttribute() const;
    Qt3DRender::QAttribute *tangentAttribute() const;
    Qt3DRender::QAttribute *indexAttribute() const;

public slots:
    void setWidth(float width);

signals:
    void widthChanged(float width);

private:
    float m_width;
    Qt3DRender::QAttribute *m_positionAttribute;
    Qt3DRender::QAttribute *m_normalAttribute;
    Qt3DRender::QAttribute *m_texCoordAttribute;
    Qt3DRender::QAttribute *m_tangentAttribute;
    Qt3DRender::QAttribute *m_indexAttribute;
    Qt3DRender::QBuffer *m_vertexBuffer;
    Qt3DRender::QBuffer *m_indexBuffer;
};

#endif // THINLINEGEOMETRY_H
