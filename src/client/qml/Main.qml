import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickFBO 1.0

import Main 1.0
import GlobalEnums 1.0

Rectangle {
    id: root
    anchors.fill: parent
    color: "#232a4e"

    readonly property int fullscreen: 0
    readonly property int quad: 1
    readonly property int split21: 2

    property int layoutMode: root.quad
    property int previousLayoutMode: root.quad
    property int splitterWidth: 10

    property int viewportMenuHeight: 40

    focus: true

    Pane {
        anchors.fill: parent
        focusPolicy: Qt.ClickFocus
    }

    function switchMode(mode) {
        previousLayoutMode = mode;
        if(mode == Editor.OBJECT)
            root.layoutMode = root.quad
        else if (mode == Editor.MATERIAL)
            root.layoutMode = root.fullscreen
    }

    Connections {
        target: ctx
        function onEditorModeChanged(mode) { root.switchMode(mode); }
    }

    onWidthChanged: {
        // scale material overlay proportionally
        if(rect.width_pct > 10)
            rect.leftMargin = root.width / (100 / rect.width_pct);
    }

    onHeightChanged: {
        // scale material overlay proportionally
        if(rect.height_pct > 10)
            rect.bottomMargin = root.height / (100 / rect.height_pct);
    }

    Keys.onPressed: function(e) {
        var targetMode = -1
        if (e.key === Qt.Key_F) targetMode = root.fullscreen
        if (e.key === Qt.Key_2) targetMode = root.quad
        if (e.key === Qt.Key_3) targetMode = root.split21

        if (targetMode >= 0) {
            if (root.layoutMode === targetMode) {
                // toggle back to previous
                var temp = root.layoutMode
                root.layoutMode = root.previousLayoutMode
                root.previousLayoutMode = temp
            } else {
                // store current as previous and switch
                root.previousLayoutMode = root.layoutMode
                root.layoutMode = targetMode
            }
            e.accepted = true
        }
    }

    ViewportMaterialOverlay {
        id: rect
        z: 1337
        clip: true
        visible: ctx.editorMode == Editor.MATERIAL

    }

    Rectangle {
        id: grid
        focus: true
        anchors.fill: parent
        color: "transparent"

        property real vRatio: 0.5
        property real hRatio: 0.5

        readonly property real vSplit: width * vRatio
        readonly property real hSplit: height * hRatio

        // Top-left
        Rectangle {
            x: 0
            y: 0
            width: root.layoutMode === root.fullscreen ? grid.width :
                    root.layoutMode === root.split21 ? grid.vSplit : grid.vSplit
            height: root.layoutMode === root.fullscreen ? grid.height :
                    root.layoutMode === root.split21 ? grid.hSplit : grid.hSplit
            color: "transparent"

            Item {
                id: view3d
                anchors.fill: parent
                anchors.rightMargin: !root.layoutMode === root.fullscreen ? root.splitterWidth / 2 : 0
                anchors.bottomMargin: !root.layoutMode === root.fullscreen ? root.splitterWidth / 2 : 0

                QuickFBO {
                    id: fbo
                    anchors.fill: parent
                    mirrorVertically: true
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

                        Item {
                            Layout.preferredWidth: 8
                            Layout.fillHeight: true
                        }

                        // full item
                        Rectangle {
                            color: "white"
                            Layout.preferredHeight: 16
                            Layout.preferredWidth: 16
                            Layout.alignment: Qt.AlignVCenter
                            radius: 16
                        }

                        Item {
                            Layout.preferredWidth: 4
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
        }

        // Top-right
        Rectangle {
            visible: root.layoutMode === root.quad
            x: grid.vSplit
            y: 0
            width: grid.width - grid.vSplit
            height: grid.hSplit
            color: "red"
        }

        // Bottom-left
        Rectangle {
            visible: root.layoutMode !== root.fullscreen
            x: 0
            y: root.layoutMode === root.split21 ? grid.hSplit : grid.hSplit
            width: root.layoutMode === root.split21 ? grid.vSplit : grid.vSplit
            height: root.layoutMode === root.split21 ? grid.height - grid.hSplit : grid.height - grid.hSplit
            color: "blue"
        }

        // Bottom-right
        Rectangle {
            visible: root.layoutMode === root.quad
            x: grid.vSplit
            y: grid.hSplit
            width: grid.width - grid.vSplit
            height: grid.height - grid.hSplit
            color: "yellow"
        }

        // Right column (Split21)
        Rectangle {
            id: split21
            visible: root.layoutMode === root.split21
            x: grid.vSplit
            y: 0
            width: grid.width - grid.vSplit
            height: grid.height
            color: "green"

            MaterialBrowser {
                anchors.fill: parent
            }
        }

        // Vertical splitter
        Rectangle {
            visible: root.layoutMode !== root.fullscreen
            x: grid.vSplit - root.splitterWidth / 2
            y: 0
            width: root.splitterWidth
            height: root.layoutMode === root.split21 ? grid.height : grid.height
            color: root.color

            MouseArea {
                anchors.fill: parent
                anchors.margins: -14
                cursorShape: Qt.SizeHorCursor
                drag.target: parent
                drag.axis: Drag.XAxis
                onPositionChanged: {
                    var x = Math.max(50, Math.min(grid.width - 50, parent.x + root.splitterWidth / 2))
                    grid.vRatio = x / grid.width
                }
            }
        }

        // Horizontal splitter
        Rectangle {
            visible: root.layoutMode !== root.fullscreen && root.layoutMode !== root.split21
            x: 0
            y: grid.hSplit - root.splitterWidth / 2
            width: grid.width
            height: root.splitterWidth
            color: root.color

            MouseArea {
                anchors.fill: parent
                anchors.margins: -14
                cursorShape: Qt.SizeVerCursor
                drag.target: parent
                drag.axis: Drag.YAxis
                onPositionChanged: {
                    var y = Math.max(50, Math.min(grid.height - 50, parent.y + root.splitterWidth / 2))
                    grid.hRatio = y / grid.height
                }
            }
        }

        // Center XY handle (only for Quad)
        Item {
            visible: root.layoutMode === root.quad
            width: 20
            height: 20
            x: grid.vSplit - width / 2
            y: grid.hSplit - height / 2

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.SizeAllCursor
                drag.target: parent
                drag.axis: Drag.XAndYAxis
                onPositionChanged: {
                    var x = Math.max(50, Math.min(grid.width - 50, parent.x + width / 2))
                    var y = Math.max(50, Math.min(grid.height - 50, parent.y + height / 2))
                    grid.vRatio = x / grid.width
                    grid.hRatio = y / grid.height
                }
            }
        }
    }
}
