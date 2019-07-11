import QtQuick 2.1 as QQ2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0



import tech.vhrd.cam 1.0 as V

Entity {
    id: root

    property alias thickline: thickline

    KeyboardDevice { id: keyboard1 }

    KeyboardHandler {
        id: input
        sourceDevice: keyboard1
        focus: true

        onTabPressed: {
            background.animateColors = !background.animateColors
        }

        onSpacePressed: {
            wave.wireframe = !wave.wireframe
        }

        onDeletePressed: {
            console.log("smth_new();")
            thickline.call_smth_new();
        }
    }

    RenderSettings {
        id: renderSettings
        activeFrameGraph: WaveForwardRenderer {
            camera: mainCamera
        }
        pickingSettings.pickMethod: PickingSettings.TrianglePicking
    }

    components: [ renderSettings, input, inputSettings ]

    // Event Source will be set by the Qt3DQuickWindow
    InputSettings { id: inputSettings }

    BasicCamera {
        id: mainCamera
        viewCenter: Qt.vector3d( 0.5, 0.5, 0.0 )
        position: Qt.vector3d( 0.5, 0.5, 3.0 )
    }

    OrbitCameraController {
        camera: mainCamera
        linearSpeed: 2.0
    }

//    Background {
//        id: background
//        layer: renderSettings.activeFrameGraph.backgroundLayer
//    }

    CoordinatePlane {
        id: coordinatePlane
        layer: renderSettings.activeFrameGraph.waveLayer
    }

    Line {
        id: line
        layer: renderSettings.activeFrameGraph.waveLayer
    }

//    ThickLine {
//        id: thickline
//        layer: renderSettings.activeFrameGraph.waveLayer
//    }


    V.ThickLine {
        id: thickline
        layer: renderSettings.activeFrameGraph.waveLayer
    }


//    UI {
//        //layer: renderSettings.activeFrameGraph.uiLayer
//    }
}
