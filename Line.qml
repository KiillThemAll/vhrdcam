import Qt3D.Core 2.0
import Qt3D.Render 2.12
import Qt3D.Extras 2.0

import tech.vhrd.cam 1.0

Entity {
    id: root
    //property alias wireframe: material.wireframe
    property real x: 0.0
    property real y: 0.0
    property real z: 0.0
    property real scale: 1.0
    property real theta: 0.0
    property real phi: 0.0
    property color tint: Qt.rgba( 0.0, 0.0, 0.0, 1.0 )
    property real intensity: 0.3
    property Layer layer: null

    components: [ transform, thinline, thinlineMaterial, layer ]

    Transform {
        id: transform
        //rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
        translation: Qt.vector3d(-0.2, 0.0, 0.0)
    }

    GoochMaterial {
        id: material
        //ambient: "red"
        cool: "blue"
        warm: "red"
        /*shininess: 0.99
        diffuse: "cyan"
        specular: "green"*/
    }

    Material {
        id: thinlineMaterial
        effect: Effect {
            techniques: [
                Technique {
                    graphicsApiFilter {
                        api: GraphicsApiFilter.OpenGL
                        profile: GraphicsApiFilter.CoreProfile
                        majorVersion: 3
                        minorVersion: 1
                    }

                    filterKeys: [ FilterKey { name: "renderingStyle"; value: "forward" } ]
                    parameters: [
                        Parameter { name: "color1"; value: Qt.vector3d( 1, 0, 0 ) },
                        Parameter { name: "color2"; value: Qt.vector3d( 0, 1, 0 ) }
                    ]

                    renderPasses: [
                        RenderPass {
                            shaderProgram: ShaderProgram {
                                vertexShaderCode:   loadSource("qrc:/shaders/thinline.vert")
                                geometryShaderCode: loadSource("qrc:/shaders/thinline.geom")
                                fragmentShaderCode: loadSource("qrc:/shaders/thinline.frag")
                            }
                            renderStates: [
                                CullFace {
                                    mode: CullFace.NoCulling
                                },
                                LineWidth {
                                    value: 10
                                },
                                PointSize {
                                    value: 10
                                }

                            ]
                        }
                    ]
                }
            ]
        }
    }

    ThickLineMesh {
        id: thinline
    }

//    PlaneMesh {
//        id: mesh
//        width: 1.0
//        height: 1.0
//        meshResolution: Qt.size( 125, 50 )
//    }
}
