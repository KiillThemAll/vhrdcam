import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Scene3D 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.5

//import units 1.0


Rectangle {
    visible: true
    width: 1366
    height: 768
    color: "#161616"
    //title: qsTr("Hello World")

    Rectangle {
        anchors.top: parent.top; anchors.left: parent.left; anchors.bottom: parent.bottom
        anchors.right: scene3dContainer.left
        anchors.margins: 10
        color: "#232323"

        Column{
            anchors.fill: parent

            FileDialog {
                id: importFileDialog
                title: "Choose import file"
                folder: shortcuts.home
                onAccepted: {
                    entity_playground.clearEngraveObjectsCache()
                    dxfio.load(fileUrl)
                }
            }

            Row{
                anchors.horizontalCenter: parent.horizontalCenter
                //anchors.top: parent.top
                //anchors.topMargin: 20

                Text{
                    text: "~/Desktop/out/ "
                    color: "white"
                    font.pointSize: 12
                }

                FocusScope {
                    width: 180; height: 25
                    Rectangle {
                        anchors.fill: parent
                        color: "lightsteelblue"
                        border.color: "gray"
                    }
                    TextInput {
                        id: exportFileNameInput
                        height: 20
                        anchors.fill: parent
                        anchors.margins: 4
                        focus: true
                        font.pointSize: 12
                        clip: true
                        text: "sum.ngc"
                    }
                }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                //anchors.top: parent.top
                //anchors.topMargin: 80
                text: "Load"
                onClicked: importFileDialog.visible = true
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                //anchors.top: parent.top
                //anchors.topMargin: 140
                text: "Export"
                onClicked: {
                    entity_playground.exportEngraveFile(exportFileNameInput.text)
                }
            }

            ListView{
                id: dxfParserOutputView
                topMargin: 10
                leftMargin: 10
                rightMargin: 10

                width: parent.width
                height: 500
                clip: true
                spacing: 10

                model: ListModel{id: dxfParserOutputModel}
                delegate: Text{
                    text: message
                    font.pointSize: 6
                    color: "white"
                }
                Connections{
                    target:dxf_parser

                    onMsgFromParser: {
                        dxfParserOutputModel.append({"message": msg})
                        dxfParserOutputView.positionViewAtEnd()
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        playground.setTargetThickLine(rootEntity.thickline)
        d2renderer.targetEntity = rootEntity
    }

    Item {
        id: scene3dContainer
        anchors.top: parent.top; anchors.bottom: parent.bottom; anchors.right: parent.right
        width: parent.width * 0.7
        anchors.margins: 10

        Scene3D {
            id: scene3d
            anchors.fill: parent
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

            Scene {
                id: rootEntity
            }


        }

        Component.onCompleted: {
            //rootEntity.camera0.tilt(45)
            //rootEntity.camera0.pan(-45)
            //rootEntity.camera0.roll(45)
        }

        /*property real val: 0
        Keys.onPressed: {
            if (event.key === Qt.Key_Left) {
                rootEntity.camera0.position.x -= 0.1
                rootEntity.camera0.viewCenter.x -= 0.1
            } else if (event.key === Qt.Key_Right) {
                rootEntity.camera0.position.x += 0.1
                rootEntity.camera0.viewCenter.x += 0.1
            } else if (event.key === Qt.Key_Up) {
                rootEntity.camera0.position.y += 0.1
                rootEntity.camera0.viewCenter.y += 0.1
            } else if (event.key === Qt.Key_Down) {
                rootEntity.camera0.position.y -= 0.1
                rootEntity.camera0.viewCenter.y -= 0.1
            } else if (event.key === Qt.Key_Plus) {
                val += 5;
                rootEntity.camera0.roll(val)
            } else if (event.key === Qt.Key_Minus) {
                val -= 5;
                rootEntity.camera0.roll(val)
            }
        }*/

        /*MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onPositionChanged: {
                //var dx =
                //console.info(mouse.x, mouse.y)
            }
        }*/
    }


}
