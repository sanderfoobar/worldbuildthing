import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ShaderEffect {
    id: view3dshader
    property var view3d
    layer.enabled: true
    property var src: view3d
    property int radius: 8
    property real deviation: 4
    property var pixelStep: Qt.vector2d(1/view3d.width, 1/view3d.height)

    property color bgColorQML: root.color
    property var bgColor: Qt.rgba(bgColorQML.r, bgColorQML.g, bgColorQML.b, bgColorQML.a)

    property bool borderActive: false
    property color borderColorQML: "#22a0ff"
    property var borderColor: Qt.rgba(borderColorQML.r, borderColorQML.g, borderColorQML.b, borderColorQML.a)

    property int resolutionx: view3d.width
    property int resolutiony: view3d.height
    property real blurStrength: 0.0

    state: "normal"
    states: [
        State {
            name: "blurred"
            PropertyChanges { target: view3dshader; blurStrength: 1.0 }
        },
        State {
            name: "normal"
            PropertyChanges { target: view3dshader; blurStrength: 0.0 }
        }
    ]

    Behavior on blurStrength {
        NumberAnimation { duration: 400; easing.type: Easing.InOutQuad }
    }

    fragmentShader: "qrc:/qml/viewport_main.frag.qsb"
    visible: true

    onStateChanged: {
        console.log("state:", state);
    }

    // MouseArea {
    //     anchors.fill: parent
    //     hoverEnabled: true
    //     onEntered: parent.borderActive = true
    //     onExited: parent.borderActive = false
    //
    //     propagateComposedEvents: true
    //
    //     onClicked: (mouseEvent) => {
    //         console.log("Mouse in parent:", mouseEvent.x, mouseEvent.y)
    //     }
    // }

    // Rectangle {
    //     z: 349
    //     height: 38
    //     width: parent.width
    //     color: "red"
    // }
}