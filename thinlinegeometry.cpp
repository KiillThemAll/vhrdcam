#include "thinlinegeometry.h"

#include <Qt3DRender/qattribute.h>
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qbufferdatagenerator.h>
#include <Qt3DRender/qattribute.h>

using namespace Qt3DRender;

#include <iostream>
using namespace std;

ThinLineGeometry::ThinLineGeometry(QNode *parent) :
    QGeometry(parent),
    m_width(1.0),
    m_positionAttribute(nullptr),
    m_normalAttribute(nullptr),
    m_texCoordAttribute(nullptr),
    m_tangentAttribute(nullptr),
    m_indexAttribute(nullptr),
    m_vertexBuffer(nullptr),
    m_indexBuffer(nullptr)
{
    m_positionAttribute = new QAttribute(this);
    //m_normalAttribute = new QAttribute(this);
    //m_texCoordAttribute = new QAttribute(this);
    //m_tangentAttribute = new QAttribute(this);
    m_indexAttribute = new QAttribute(this);
    m_vertexBuffer = new Qt3DRender::QBuffer(this);
    m_indexBuffer = new Qt3DRender::QBuffer(this);

    const int nVertices = 4;
    const int stride = (3/* + 2 + 3 + 4*/) * sizeof(float);
    //const int faces = 4;

    m_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setVertexBaseType(QAttribute::Float);
    m_positionAttribute->setVertexSize(3);
    m_positionAttribute->setAttributeType(QAttribute::VertexAttribute);

    m_positionAttribute->setByteStride(stride);
    m_positionAttribute->setCount(nVertices);

    QByteArray verticesBytes;
    verticesBytes.resize(nVertices * stride);
    float *vertptr = reinterpret_cast<float*>(verticesBytes.data());
    vertptr[0] = 0.0f;
    vertptr[1] = 0.0f;
    vertptr[2] = 0.0f;
    vertptr += 3;

    vertptr[0] = 0.0f;
    vertptr[1] = 0.1f;
    vertptr[2] = 0.0f;
    vertptr += 3;

    vertptr[0] = 0.1f;
    vertptr[1] = 0.1f;
    vertptr[2] = 0.0f;
    vertptr += 3;

    vertptr[0] = 0.2f;
    vertptr[1] = 0.2f;
    vertptr[2] = 0.0f;
    vertptr += 3;

    m_vertexBuffer->setData(verticesBytes);
    m_positionAttribute->setBuffer(m_vertexBuffer);

    m_indexAttribute->setAttributeType(QAttribute::IndexAttribute);
    m_indexAttribute->setVertexBaseType(QAttribute::UnsignedShort);

    m_indexAttribute->setCount(nVertices);

    QByteArray indexBytes;
    indexBytes.resize(nVertices * sizeof(quint16));
    quint16* idxptr = reinterpret_cast<quint16*>(indexBytes.data());
    idxptr[0] = 0;
    idxptr[1] = 1;
    idxptr[2] = 2;
    idxptr[3] = 3;
    m_indexBuffer->setData(indexBytes);
    m_indexAttribute->setBuffer(m_indexBuffer);

    addAttribute(m_positionAttribute);
    //addAttribute(m_texCoordAttribute);
    //addAttribute(m_normalAttribute);
    //addAttribute(m_tangentAttribute);
    addAttribute(m_indexAttribute);
}

ThinLineGeometry::~ThinLineGeometry()
{

}

QAttribute *ThinLineGeometry::positionAttribute() const
{
    return m_positionAttribute;
}

QAttribute *ThinLineGeometry::normalAttribute() const
{
    return m_normalAttribute;
}

QAttribute *ThinLineGeometry::texCoordAttribute() const
{
    return m_texCoordAttribute;
}

QAttribute *ThinLineGeometry::tangentAttribute() const
{
    return m_tangentAttribute;
}

QAttribute *ThinLineGeometry::indexAttribute() const
{
    return m_indexAttribute;
}

float ThinLineGeometry::width()
{
    return m_width;
}

void ThinLineGeometry::setWidth(float width)
{
    m_width = width;
    emit widthChanged(width);
}
