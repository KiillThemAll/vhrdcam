#include "thicklinemesh.h"

#include "thicklinegeometry.h"

ThickLineMesh::ThickLineMesh(Qt3DCore::QNode *parent) :
    QGeometryRenderer(parent)
{
    setPrimitiveType(QGeometryRenderer::PrimitiveType::LinesAdjacency);

    m_geometry = new ThickLineGeometry(this);
    QGeometryRenderer::setGeometry(m_geometry);
}

ThickLineMesh::~ThickLineMesh()
{

}

float ThickLineMesh::width()
{
    return 0;
}

ThickLineGeometry *ThickLineMesh::geometry() const
{
    return m_geometry;
}

void ThickLineMesh::setWidth(float width)
{

}
