import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickFBO 1.0

import Main 1.0

Rectangle {
    id: root
    anchors.fill: parent

    color: "#232a4e"

    property string bgColor: root.color
    property bool fullscreenMode: true
    property int splitterWidth: 10

    focus: true
    Keys.onPressed: function(event) {
        if (event.key === Qt.Key_F) {
            fullscreenMode = !fullscreenMode
            event.accepted = true
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "red"


    }

    Rectangle {
        id: test
        visible: false
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

            Item {
                id: view3d
                anchors.fill: parent
                anchors.rightMargin: !fullscreenMode ? root.splitterWidth / 2 : 0
                anchors.bottomMargin: !fullscreenMode ? root.splitterWidth / 2 : 0

                QuickFBO {
                    id: fbo
                    anchors.fill: parent
                    mirrorVertically: true
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.AllButtons

                    onPressed: (mouseEvent) => {
                        if (mouseEvent.button === Qt.RightButton)
                            console.log("Right button pressed at", mouseEvent.x, mouseEvent.y)
                    }

                    onReleased: (mouseEvent) => {
                        if (mouseEvent.button === Qt.RightButton)
                            console.log("Right button released at", mouseEvent.x, mouseEvent.y)
                    }
                }

                Rectangle {
                    height: 40
                    width: parent.width
                    color: "#4D000000"
                    z: fbo.z + 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 8
                        spacing: 10

                        // full item
                        Rectangle {
                            color: "white"
                            Layout.preferredHeight: 28
                            Layout.preferredWidth: 64
                            Layout.alignment: Qt.AlignVCenter
                        }

                        Item {
                            Layout.preferredWidth: 40
                            Layout.fillHeight: true
                        }


                        ViewportMenuBarTextButton {
                            text: "View"
                            Layout.preferredHeight: 26
                            Layout.preferredWidth: textItem.implicitWidth + 22
                        }

                        ViewportMenuBarTextButton {
                            text: "Select"
                            Layout.preferredHeight: 26
                            Layout.preferredWidth: textItem.implicitWidth + 22
                        }

                        ViewportMenuBarTextButton {
                            text: "Add"
                            Layout.preferredHeight: 26
                            Layout.preferredWidth: textItem.implicitWidth + 22
                        }

                        ViewportMenuBarTextButton {
                            text: "Object"
                            Layout.preferredHeight: 26
                            Layout.preferredWidth: textItem.implicitWidth + 22
                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }
                }
            }

            // ColumnLayout {
            //     id: view3d
            //     anchors.fill: parent
            //     anchors.rightMargin: !fullscreenMode ? root.splitterWidth / 2 : 0
            //     anchors.bottomMargin: !fullscreenMode ? root.splitterWidth / 2 : 0
            //     spacing: 0
            //
            //     Rectangle {
            //         color: "green"
            //         Layout.preferredHeight: 38
            //         Layout.fillWidth: true
            //     }
            //
            //     QuickFBO {
            //         Layout.fillWidth: true
            //         Layout.fillHeight: true
            //         mirrorVertically: true
            //     }
            // }

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

            Text {
                text: "Perspective\n\nObjects: 4\nVertices: 8\nEdges: 16\nFaces: 6\nTriangles: 18"
                color: "white"
                font.pointSize: 14
                anchors.top: parent.top
                anchors.topMargin: 50
                anchors.left: parent.left
                anchors.margins: 12
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
