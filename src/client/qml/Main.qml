import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickFBO 1.0

Rectangle {
    id: root
    anchors.fill: parent

    color: "#232a4e"

    property string bgColor: root.color
    property bool fullscreenMode: false
    property int splitterWidth: 10

    focus: true
    Keys.onPressed: function(event) {
        if (event.key === Qt.Key_F) {
            fullscreenMode = !fullscreenMode
            event.accepted = true
        }
    }

    Rectangle {
        id: test
        anchors.fill: parent
        color: "transparent"

        // Ratios instead of absolute pixels
        property real vRatio: 0.5
        property real hRatio: 0.5

        readonly property real vSplit: width * vRatio
        readonly property real hSplit: height * hRatio

        // Top-left
        Rectangle {
            id: mainView
            x: 0
            y: 0
            width: fullscreenMode ? root.width : test.vSplit
            height: fullscreenMode ? root.height : test.hSplit

            QuickFBO {
                id: view3d
                anchors.fill: parent
                anchors.rightMargin: !fullscreenMode ? root.splitterWidth / 2 : 0
                anchors.bottomMargin: !fullscreenMode ? root.splitterWidth / 2 : 0
            }

            ShaderEffect {
                id: view3dshader
                anchors.fill: view3d
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

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: parent.borderActive = true
                    onExited: parent.borderActive = false

                    propagateComposedEvents: true

                    onClicked: (mouseEvent) => {
                        console.log("Mouse in parent:", mouseEvent.x, mouseEvent.y)
                    }
                }
            }

            Text {
                text: "Perspective\n\nObjects: 4\nVertices: 8\nEdges: 16\nFaces: 6\nTriangles: 18"
                color: "white"
                font.pointSize: 14
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 12
                anchors.topMargin: 6
            }
        }

        // Top-right
        Rectangle {
            visible: !fullscreenMode
            x: test.vSplit
            y: 0
            width: test.width - test.vSplit
            height: test.hSplit
            color: "green"
        }

        // Bottom-left
        Rectangle {
            visible: !fullscreenMode
            x: 0
            y: test.hSplit
            width: test.vSplit
            height: test.height - test.hSplit
            color: "blue"
        }

        // Bottom-right
        Rectangle {
            visible: !fullscreenMode
            x: test.vSplit
            y: test.hSplit
            width: test.width - test.vSplit
            height: test.height - test.hSplit
            color: "yellow"
        }

        // Vertical splitter
        Rectangle {
            visible: !fullscreenMode
            x: test.vSplit - 5
            y: 0
            width: root.splitterWidth
            height: test.height
            color: root.bgColor

            MouseArea {
                anchors.fill: parent
                anchors.margins: -14
                cursorShape: Qt.SizeHorCursor
                drag.target: parent
                drag.axis: Drag.XAxis
                onPositionChanged: {
                    var x = Math.max(50, Math.min(test.width - 50, parent.x + 5))
                    test.vRatio = x / test.width
                }
            }
        }

        // Horizontal splitter
        Rectangle {
            visible: !fullscreenMode
            x: 0
            y: test.hSplit - 5
            width: test.width
            height: root.splitterWidth
            color: root.bgColor

            MouseArea {
                anchors.fill: parent
                anchors.margins: -14
                cursorShape: Qt.SizeVerCursor
                drag.target: parent
                drag.axis: Drag.YAxis
                onPositionChanged: {
                    var y = Math.max(50, Math.min(test.height - 50, parent.y + 5))
                    test.hRatio = y / test.height
                }
            }
        }

        // Center drag handle
        Item {
            visible: !fullscreenMode
            width: 20
            height: 20
            x: test.vSplit - width / 2
            y: test.hSplit - height / 2

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.SizeAllCursor
                drag.target: parent
                drag.axis: Drag.XAndYAxis
                onPositionChanged: {
                    var x = Math.max(50, Math.min(test.width - 50, parent.x + width / 2))
                    var y = Math.max(50, Math.min(test.height - 50, parent.y + height / 2))
                    test.vRatio = x / test.width
                    test.hRatio = y / test.height
                }
            }
        }
    }

    // MouseArea {
    //     anchors.fill: parent
    //     onClicked: {
    //         if(view3dshader.state === "normal") {
    //             view3dshader.state = "blurred";
    //         } else {
    //             view3dshader.state = "normal";
    //         }
    //     }
    // }
}
