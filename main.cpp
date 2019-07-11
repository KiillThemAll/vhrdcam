#include "units.h"
#include "thicklinemesh.h"
#include "thinlinemesh.h"
#include "thickline.h"
#include "geometryplayground.h"

#include "vhapi/2dentities/lineentity.hpp"
#include "vhapi/2dentities/polylineentity.h"
#include "vhapi/2dentities/groupentity.h"
#include "vhapi/2dentities/markerentity.hpp"
#include "vhapi/2dentities/curveentity.h"
#include "vhapi/2dentities/shapeentity.h"
#include "d2entityrenderer.h"

#include "vhapi/zeromqinterface.h"
#include "vhapi/zeromqinterfaceworker.h"
#include "vhapi/dxfio.h"
#include "vhapi/dxfioentitiestxport.h"
#include "entityplayground.h"
#include "vhapi/entityplaygroundvhapi.h"

#include <Qt3DQuickExtras/qt3dquickwindow.h>
#include <Qt3DQuick/QQmlAspectEngine>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QOpenGLContext>
#include <QQuickView>

#include <QDebug>

//#include <iostream>
//#include <Eigen/Dense>
//using Eigen::Matrix;
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);



    QGuiApplication app(argc, argv);

    qmlRegisterSingletonType<Units>("units", 1, 0, "Units", qobject_units_provider);
    qmlRegisterType<ThickLineMesh>("tech.vhrd.cam", 1, 0, "ThickLineMesh");
    qmlRegisterType<ThinLineMesh>("tech.vhrd.cam", 1, 0, "ThinLineMesh");
    qmlRegisterType<ThickLine>("tech.vhrd.cam", 1, 0, "ThickLine");

    QSurfaceFormat format;
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        format.setVersion(3, 2);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);

    GeometryPlayground *playground = new GeometryPlayground();
    //qRegisterMetaType<PointEntity>("PointEntity");
    QMetaType::registerConverter(&LineEntity::toString);
    QMetaType::registerConverter(&PointEntity::toString);
    QMetaType::registerConverter(&PolylineEntity::toString);
    QMetaType::registerConverter(&GroupEntity::toString);
    QMetaType::registerConverter(&MarkerEntity::toString);
    QMetaType::registerConverter(&CurveEntity::toString);
    QMetaType::registerConverter(&ShapeEntity::toString);
    QMetaType::registerEqualsComparator<PointEntity>();

    D2EntityRenderer *d2EntityRenderer = new D2EntityRenderer;
    playground->rxport = d2EntityRenderer->m_entitiesPort;

    // interface which should be magically made available to blocks that refer to it
    ZeroMQInterface *zmqInterface = new ZeroMQInterface;
    QObject::connect(&app, &QCoreApplication::aboutToQuit,
                     zmqInterface, &ZeroMQInterface::stop);
    DxfIO *dxfio = new DxfIO; // remote Block
    // should be dynamic QObject of worker?
    QObject::connect(zmqInterface->worker(), &ZeroMQInterfaceWorker::syncFromDxfIOReceived,
                    dxfio, &DxfIO::syncReceived);
    QObject::connect(zmqInterface->worker(), &ZeroMQInterfaceWorker::multipartFromDxfIOReceived,
                    dxfio, &DxfIO::multipartReceived);
    QObject::connect(dxfio, &DxfIO::serialized,
                     zmqInterface->worker(), &ZeroMQInterfaceWorker::sendBlockToDxfIO);

    DxfIOEntitiesTxPort *p = qobject_cast<DxfIOEntitiesTxPort *>(dxfio->txports()["entities"]);
    qDebug() << "found tx port of dxfio" << p;
    if (p)
        QObject::connect(p, &DxfIOEntitiesTxPort::substreamItemReceived,
                         d2EntityRenderer->m_entitiesPort, &D2EntityRendererEntitiesIOPortVHAPI::substreamItemReceived);
    EntityPlayground *entityPlayground = new EntityPlayground;
    QObject::connect(p, &DxfIOEntitiesTxPort::substreamItemReceived,
                    entityPlayground->api(), &EntityPlaygroundVhApi::rxentitiesSubstreamItemReceived);
    QObject::connect(entityPlayground->api(), &EntityPlaygroundVhApi::txentitiesSubstreamItemReceived,
                     d2EntityRenderer->m_entitiesPort, &D2EntityRendererEntitiesIOPortVHAPI::substreamItemReceived);

    QQuickView view;
    view.engine()->rootContext()->setContextProperty("_window", &app);
    view.engine()->rootContext()->setContextProperty("playground", playground);
    view.engine()->rootContext()->setContextProperty("d2renderer", d2EntityRenderer);
    view.engine()->rootContext()->setContextProperty("dxfio", dxfio);
    view.engine()->rootContext()->setContextProperty("zmq", zmqInterface);

    view.setFormat(format);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

//    QQmlApplicationEngine engine;
//    engine.rootContext()->setContextProperty("_window", &app);
//    const QUrl url(QStringLiteral("qrc:/main.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);
//    engine.load(url);

    return app.exec();

//    QGuiApplication app(argc, argv);
//    Qt3DExtras::Quick::Qt3DQuickWindow view;
//    view.resize(1366, 768);

//    qmlRegisterType<ThickLineMesh>("tech.vhrd.cam", 1, 0, "ThickLineMesh");
//    qmlRegisterType<ThinLineMesh>("tech.vhrd.cam", 1, 0, "ThinLineMesh");
//    qmlRegisterType<ThickLine>("tech.vhrd.cam", 1, 0, "ThickLine");

//    // Expose the window as a context property so we can set the aspect ratio
//    view.engine()->qmlEngine()->rootContext()->setContextProperty("_window", &view);
//    view.setSource(QUrl("qrc:/main3donly.qml"));
//    view.show();

//    return app.exec();
}
