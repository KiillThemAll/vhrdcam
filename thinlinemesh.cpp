#include "thinlinemesh.h"

#include "thinlinegeometry.h"

ThinLineMesh::ThinLineMesh(Qt3DCore::QNode *parent) :
    QGeometryRenderer(parent)
{
    setPrimitiveType(QGeometryRenderer::PrimitiveType::Points);

    ThinLineGeometry *geometry = new ThinLineGeometry(this);
    QGeometryRenderer::setGeometry(geometry);
}

ThinLineMesh::~ThinLineMesh()
{

}

float ThinLineMesh::width()
{
    return 0;
}

void ThinLineMesh::setWidth(float width)
{

}
