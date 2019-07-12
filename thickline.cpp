#include "thickline.h"

#include "thicklinemesh.h"
#include "thicklinegeometry.h"

#include <Qt3DCore/QTransform>
#include <Qt3DRender>
using namespace Qt3DRender;

ThickLine::ThickLine(Qt3DCore::QNode *parent) :
    Qt3DCore::QEntity(parent)
{
    m_mesh = new ThickLineMesh(this);

    m_transform = new Qt3DCore::QTransform;
    m_transform->setTranslation(QVector3D(0.0, 0.0, 0.0));
    //m_transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 90.0f));

    m_material = new Qt3DRender::QMaterial(this);
    QEffect *effect = new QEffect();
    QTechnique *technique = new QTechnique();

    technique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
    technique->graphicsApiFilter()->setMajorVersion(3);
    technique->graphicsApiFilter()->setMinorVersion(2);
    technique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

    QFilterKey *filterKey = new QFilterKey();
    filterKey->setName("renderingStyle");
    filterKey->setValue("forward");
    technique->addFilterKey(filterKey);

    //technique->addParameter(new QParameter("thickness", 10));
    technique->addParameter(new QParameter("winScale", QVector2D(1366, 768)));
    technique->addParameter(new QParameter("miterLimit", 0.95f));

    QRenderPass *renderPass = new QRenderPass();
    QShaderProgram *shaderProgram = new QShaderProgram;
    shaderProgram->setVertexShaderCode(QShaderProgram::loadSource(
        QUrl(QStringLiteral("qrc:/shaders/thickline.vert"))));
    shaderProgram->setGeometryShaderCode(QShaderProgram::loadSource(
        QUrl(QStringLiteral("qrc:/shaders/thickline.geom"))));
    shaderProgram->setFragmentShaderCode(QShaderProgram::loadSource(
        QUrl(QStringLiteral("qrc:/shaders/thickline.frag"))));
    renderPass->setShaderProgram(shaderProgram);

    QCullFace *cullFace = new QCullFace;
    cullFace->setMode(QCullFace::NoCulling);
    renderPass->addRenderState(cullFace);

    technique->addRenderPass(renderPass);

    effect->addTechnique(technique);
    m_material->setEffect(effect);

    addComponent(m_mesh);
    addComponent(m_transform);
    addComponent(m_material);
}

ThickLine::~ThickLine()
{
}

void ThickLine::call_smth_new()
{
    //m_mesh->geometry()->smth_new();
}

void ThickLine::setLayer(QLayer *layer)
{
    addComponent(layer);
    emit layerChanged();
}

QLayer *ThickLine::layer() const
{
    return nullptr;
}

ThickLineGeometry *ThickLine::geometry() const
{
    return m_mesh->geometry();
}
