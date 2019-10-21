#ifndef THICKLINEGEOMETRY_H
#define THICKLINEGEOMETRY_H

#include "units.h"

#include <Qt3DExtras/qt3dextras_global.h>
#include <Qt3DRender/qgeometry.h>
//#include <Eigen/Dense>

namespace Qt3DRender {

class QBuffer;

}

class ThickLineGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT

public:
    explicit ThickLineGeometry(QNode *parent = nullptr);
    ~ThickLineGeometry();

    Qt3DRender::QAttribute *positionAttribute() const;
    Qt3DRender::QAttribute *indexAttribute() const;

    void clear();
    void addPoint(float x, float y, float z, quint8 r, quint8 g, quint8 b, float t);
    void breakStrip();
    void flush();
    void setdz(float dz);

public slots:


signals:


private:
    float m_width;
    Qt3DRender::QAttribute *m_positionAttribute;
    Qt3DRender::QAttribute *m_colorAttribute;
    Qt3DRender::QAttribute *m_thicknessAttribute;
    Qt3DRender::QAttribute *m_indexAttribute;
    Qt3DRender::QBuffer *m_vertexBuffer;
    Qt3DRender::QBuffer *m_indexBuffer;
    QByteArray m_vertexData;
    QByteArray m_indexData;
    quint32 m_currentPointIndex;
    quint32 m_globalPointIndex;
    float m_dz;

    void finishStrip();
};

#endif // THICKLINEGEOMETRY_H
