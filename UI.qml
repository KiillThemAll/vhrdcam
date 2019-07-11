import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Extras 2.12

import QtQuick 2.6
import QtQuick.Scene2D 2.12
import QtQuick.Controls 2.5


Entity {
    id: uiroot

    property Layer layer: null

    components: [planeMesh, planeTransorm, planeMaterial, planePicker, layer]

    PlaneMesh {
        id: planeMesh
        width: 0.5
        height: 0.5
        mirrored: true
    }

    Transform {
        id: planeTransorm
        translation: Qt.vector3d(0.5, 0.0, 0.0)
        rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
    }

    ObjectPicker {
        id: planePicker
        hoverEnabled: true
        dragEnabled: true
    }

    TextureMaterial {
        id: planeMaterial
        texture: offscreenTexture
    }

    Scene2D {
        id: qmlTexture
        output: RenderTargetOutput {
            attachmentPoint: RenderTargetOutput.Color0
            texture: Texture2D {
                id: offscreenTexture
                width: 640
                height: 480
                format: Texture.RGBA8_UNorm
                generateMipMaps: true
                magnificationFilter: Texture.Linear
                minificationFilter: Texture.LinearMipMapLinear
                wrapMode {
                    x: WrapMode.ClampToEdge
                    y: WrapMode.ClampToEdge
                }

            }
        }

        entities: [uiroot]
        mouseEnabled: true

        Item {
            width: offscreenTexture.width
            height: offscreenTexture.height
            //color: "white"

            enabled: true

            Button {
                text: "Test"
                anchors.centerIn: parent
            }

//            ProgressBar {
//                indeterminate: true
//            }
        }
    }
}
