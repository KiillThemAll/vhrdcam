cmake_minimum_required(VERSION 3.1)

project(3dplay LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick Qml 3DQuick 3DRender 3DCore 3DExtras 3DQuickExtras 3DInput REQUIRED)
find_package(ZeroMQ REQUIRED)
#find_package(MsgPackQtTargets)

include_directories(${EIGEN_DIR})
add_executable(${PROJECT_NAME} "main.cpp"
    "thicklinemesh.cpp"
    "thicklinegeometry.cpp"
    "thinlinegeometry.cpp"
    "thinlinemesh.cpp"
    "units.cpp"
    "thickline.cpp"
    "geometryplayground.cpp"
    "d2entityrenderer.cpp"
    "vhapi/2dentities/pointentity.cpp"
    "vhapi/2dentities/lineentity.cpp"
    "vhapi/2dentities/markerentity.cpp"
    "vhapi/2dentities/groupentity.cpp"
    "vhapi/2dentities/shapeentity.cpp"
    "vhapi/d2entityrenderervhapi.cpp"
    "vhapi/2dentities/curveentity.cpp"
    "vhapi/d2entityrendererentitiesioportvhapi.cpp"
    "vhapi/rxport.cpp"
    "vhapi/2dentities/polylineentity.cpp"
    "vhapi/dxfio.cpp"
    "vhapi/zeromqinterface.cpp"
    "vhapi/zeromqinterfaceworker.cpp"
    "vhapi/txport.cpp"
    "vhapi/dxfioentitiestxport.cpp"
    "3rdparty/qmsgpack/src/msgpack.cpp"
    "3rdparty/qmsgpack/src/msgpackstream.cpp"
    "3rdparty/qmsgpack/src/private/pack_p.cpp"
    "3rdparty/qmsgpack/src/private/unpack_p.cpp"
    "3rdparty/qmsgpack/src/private/qt_types_p.cpp"
    "3rdparty/qmsgpack/src/stream/time.cpp"
    "entityplayground.cpp"
    "vhapi/entityplaygroundvhapi.cpp"
    "qml.qrc")
#target_include_directories(${PROJECT_NAME} PUBLIC ${ZMQ_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PUBLIC ${ZMQ_LIBRARIES} PRIVATE Qt5::Core Qt5::Quick Qt5::Qml Qt5::3DRender Qt5::3DCore Qt5::3DExtras Qt5::3DQuickExtras Qt5::3DInput Qt5::3DQuick)
