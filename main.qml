import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Scene3D 2.0

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

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
            text: "Draw smth"
            onClicked: playground.onDrawSmthClicked()
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 80
            text: "Draw +"
            onClicked: playground.onDrawPlusClicked()
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 140
            text: "Load"
            onClicked: dxfio.load()
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 200
            text: "Stop 0mq"
            onClicked: zmq.stop()
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 260
            text: "Focus"
            onClicked: {
                scene3d.focus = true
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
        width: parent.width * 0.8
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
