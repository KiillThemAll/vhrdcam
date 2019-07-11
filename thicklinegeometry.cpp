#include "thicklinegeometry.h"

#include <Qt3DRender/qattribute.h>
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qbufferdatagenerator.h>
#include <Qt3DRender/qattribute.h>

#include <QDebug>

using namespace Qt3DRender;

#include <iostream>
using namespace std;

typedef struct {
    float x;
    float y;
    float z;
    quint8 r;
    quint8 g;
    quint8 b;
    quint8 _dymmy;
    float t;
} thick_line_stride_t;

ThickLineGeometry::ThickLineGeometry(QNode *parent) :
    QGeometry(parent),
    m_width(1.0),
    m_positionAttribute(nullptr),
    m_indexAttribute(nullptr),
    m_vertexBuffer(nullptr),
    m_indexBuffer(nullptr)
{
    m_positionAttribute = new QAttribute(this);
    m_colorAttribute = new QAttribute(this);
    m_thicknessAttribute = new QAttribute(this);
    m_indexAttribute = new QAttribute(this);
    m_vertexBuffer = new Qt3DRender::QBuffer(this);
    m_indexBuffer = new Qt3DRender::QBuffer(this);

    quint32 stride = sizeof(thick_line_stride_t);

    m_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setVertexBaseType(QAttribute::Float);
    m_positionAttribute->setVertexSize(3);
    m_positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    m_positionAttribute->setByteStride(stride);
    m_positionAttribute->setCount(0);
    m_positionAttribute->setBuffer(m_vertexBuffer);

    m_colorAttribute->setName(QAttribute::defaultColorAttributeName());
    m_colorAttribute->setVertexBaseType(QAttribute::UnsignedByte);
    m_colorAttribute->setVertexSize(3);
    m_colorAttribute->setAttributeType(QAttribute::VertexAttribute);
    m_colorAttribute->setByteStride(stride);
    m_colorAttribute->setByteOffset(3 * sizeof(float));
    m_colorAttribute->setCount(0);
    m_colorAttribute->setBuffer(m_vertexBuffer);

    m_thicknessAttribute->setName("lineThickness");
    m_thicknessAttribute->setVertexBaseType(QAttribute::Float);
    m_thicknessAttribute->setVertexSize(1);
    m_thicknessAttribute->setAttributeType(QAttribute::VertexAttribute);
    m_thicknessAttribute->setByteStride(stride);
    m_thicknessAttribute->setByteOffset(3 * sizeof(float) + 4 * sizeof(quint8));
    m_thicknessAttribute->setCount(0);
    m_thicknessAttribute->setBuffer(m_vertexBuffer);

    m_indexAttribute->setAttributeType(QAttribute::IndexAttribute);
    m_indexAttribute->setVertexBaseType(QAttribute::UnsignedShort);
    m_indexAttribute->setBuffer(m_indexBuffer);

    addAttribute(m_positionAttribute);
    addAttribute(m_colorAttribute);
    addAttribute(m_thicknessAttribute);
    addAttribute(m_indexAttribute);
}

ThickLineGeometry::~ThickLineGeometry()
{

}

QAttribute *ThickLineGeometry::positionAttribute() const
{
    return m_positionAttribute;
}

QAttribute *ThickLineGeometry::indexAttribute() const
{
    return m_indexAttribute;
}

void ThickLineGeometry::clear()
{
    qDebug() << "startDrawing(), capacity: " << m_vertexData.capacity() << ", size: " << m_vertexData.size();
    m_vertexData.clear();
    m_indexData.clear();
    m_vertexData.reserve(sizeof(thick_line_stride_t) * 100);
    m_indexData.reserve(4 * sizeof(quint16) * 100);
    m_currentPointIndex = 0;
    m_globalPointIndex = 0;
    qDebug() << "clear, capacity: " << m_vertexData.capacity() << ", size: " << m_vertexData.size();
}

void ThickLineGeometry::addPoint(float x, float y, float z, quint8 r, quint8 g, quint8 b, float t)
{
    x = x / 2000;
    y = y / 2000;
    z = z / 2000;
    if (m_currentPointIndex == 0) { // add 4 strides for 2 real points and 2 adjacent points in advance
        //qDebug() << "if0";
        m_vertexData.resize(m_vertexData.size() + 4 * sizeof(thick_line_stride_t));
        thick_line_stride_t * adjBefore = reinterpret_cast<thick_line_stride_t *>(m_vertexData.data()
                                                                         + m_vertexData.size()
                                                                         - 4 * sizeof(thick_line_stride_t));
        thick_line_stride_t * firstPoint = adjBefore + 1;
        firstPoint->x = x; firstPoint->y = y; firstPoint->z = z;
        firstPoint->r = r; firstPoint->g = g; firstPoint->b = b;
        firstPoint->t = t;

        m_currentPointIndex++;
    } else if (m_currentPointIndex == 1) { // add this point and normal after it
        //qDebug() << "if1";
        thick_line_stride_t * adjBefore = reinterpret_cast<thick_line_stride_t *>(m_vertexData.data()
                                                                         + m_vertexData.size()
                                                                         - 4 * sizeof(thick_line_stride_t));
        thick_line_stride_t * firstPoint = adjBefore + 1;
        thick_line_stride_t * secondPoint = adjBefore + 2;
        secondPoint->x = x; secondPoint->y = y; secondPoint->z = z;
        secondPoint->r = r; secondPoint->g = g; secondPoint->b = b;
        secondPoint->t = t;

        // now we can calculate adjacent point before 2 inserted real points
        adjBefore->x = 2 * firstPoint->x - x;
        adjBefore->y = 2 * firstPoint->y - y;
        adjBefore->z = 2 * firstPoint->z - z;
        adjBefore->r = 0; adjBefore->g = 0; adjBefore->b = 0; adjBefore->t = 0;

        m_indexData.resize(m_indexData.size() + sizeof(quint16) * 4);
        quint16 * idx = reinterpret_cast<quint16 *>(m_indexData.data()
                                                    + m_indexData.size()
                                                    - sizeof(quint16) * 4);
        idx[0] = m_globalPointIndex + m_currentPointIndex - 1;
        idx[1] = m_globalPointIndex + m_currentPointIndex;
        idx[2] = m_globalPointIndex + m_currentPointIndex + 1;
        idx[3] = m_globalPointIndex + m_currentPointIndex + 2;
        m_currentPointIndex++;
    } else { // overwrite previously created adj point with this point, and add adj point after it
        //qDebug() << "if2";
        thick_line_stride_t * lastPoint = reinterpret_cast<thick_line_stride_t *>(m_vertexData.data()
                                                                         + m_vertexData.size()
                                                                         - sizeof(thick_line_stride_t));
        lastPoint->x = x; lastPoint->y = y; lastPoint->z = z;
        lastPoint->r = r; lastPoint->g = g; lastPoint->b = b;
        lastPoint->t = t;
        // reserve space for next point or last adj point
        m_vertexData.resize(m_vertexData.size() + 1 * sizeof(thick_line_stride_t));

        m_indexData.resize(m_indexData.size() + sizeof(quint16) * 4);
        quint16 * idx = reinterpret_cast<quint16 *>(m_indexData.data()
                                                    + m_indexData.size()
                                                    - sizeof(quint16) * 4);
        idx[0] = m_globalPointIndex + m_currentPointIndex - 1;
        idx[1] = m_globalPointIndex + m_currentPointIndex;
        idx[2] = m_globalPointIndex + m_currentPointIndex + 1;
        idx[3] = m_globalPointIndex + m_currentPointIndex + 2;
        m_currentPointIndex++;
    }
    //qDebug() << "add point, capacity: " << m_vertexData.capacity() << ", size: " << m_vertexData.size() << ", idx: " << m_currentPointIndex << m_globalPointIndex;
}

void ThickLineGeometry::breakStrip()
{
    finishStrip();
    m_globalPointIndex += m_currentPointIndex + 2;
    m_currentPointIndex = 0;
}

void ThickLineGeometry::flush()
{
    // not enough data to render, only 4 strides reserved and 0 or 1 point added
    if (m_currentPointIndex < 2 && m_globalPointIndex == 0 && (m_vertexData.size() == 4 * sizeof(thick_line_stride_t))) {
        qDebug() << "flush clear";
        m_positionAttribute->setCount(0);
        m_colorAttribute->setCount(0);
        m_thicknessAttribute->setCount(0);
        m_indexAttribute->setCount(0);
        return;
    }
    //qDebug() << "g flash";
//qDebug() << "flush, capacity: " << m_vertexData.capacity() << ", size: " << m_vertexData.size() << ", idx: " << m_currentPointIndex;
    // temporarily fill last stride with adjacent point for 2 previously added real points
    // it will be overwritten with next point or not if next strip will start
    finishStrip();

    quint32 strideCount = m_vertexData.size() / sizeof(thick_line_stride_t);

//    thick_line_stride_t *s = reinterpret_cast<thick_line_stride_t *>(m_vertexData.data());
//    for (quint32 i = 0; i < strideCount; ++i) {
//        qDebug() << "xyz: " << s->x << s->y << s->z << "rgb:" << s->r << s->g << s->b << "t:" << s->t;
//        s++;
//    }


    m_positionAttribute->setCount(strideCount);
    m_colorAttribute->setCount(strideCount);
    m_thicknessAttribute->setCount(strideCount);
    m_vertexBuffer->setData(m_vertexData);

    quint32 indexCount = m_indexData.size() / sizeof(quint16);

//    quint16 *idx = reinterpret_cast<quint16 *>(m_indexData.data());
//    for (quint32 i = 0; i < indexCount; i += 4) {
//        qDebug() << *idx << *(idx + 1) << *(idx + 2) << *(idx + 3);
//        idx += 4;
//    }


    m_indexAttribute->setCount(indexCount);
    m_indexBuffer->setData(m_indexData);
}

void ThickLineGeometry::finishStrip()
{
    thick_line_stride_t * adjAfter = reinterpret_cast<thick_line_stride_t *>(m_vertexData.data()
                                                                     + m_vertexData.size()
                                                                     - sizeof(thick_line_stride_t));
    thick_line_stride_t * firstPoint = adjAfter - 2;
    thick_line_stride_t * secondPoint = adjAfter - 1;
    // calculate adjacent point for 2 previously added points for accurate rendering
    adjAfter->x = 2 * secondPoint->x - firstPoint->x;
    adjAfter->y = 2 * secondPoint->y - firstPoint->y;
    adjAfter->z = 2 * secondPoint->z - firstPoint->z;
    adjAfter->r = 0; adjAfter->g = 0; adjAfter->b = 0; adjAfter->t = 0;
}

