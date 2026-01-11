import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickFBO 1.0

import Main 1.0
import GlobalEnums 1.0

Item {
    id: rect

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        MaterialBrowser {
            id: matbrowser
            anchors.fill: parent
        }

        ViewportComponentShader {
            anchors.fill: parent
            borderActive: true
            view3d: matbrowser
        }
    }

    property real height_pct: 0
    property real width_pct: 0

    property real leftMargin: {
        return (root.width / 3) * 2;
    }
    property real rightMargin: 32
    property real topMargin: root.viewportMenuHeight + 24
    property real bottomMargin: 32

    // fully reactive bindings
    x: leftMargin
    y: topMargin
    width: root.width - leftMargin - rightMargin
    height: root.height - topMargin - bottomMargin

    function calcProportionalSize() {
        rect.width_pct = 100 / (root.width / rect.leftMargin);
        rect.height_pct = 100 / (root.height / rect.bottomMargin);
    }

    // left-edge horizontal drag
    MouseArea {
        id: dragLeftHandle
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 20
        anchors.margins: -5
        cursorShape: Qt.SizeHorCursor

        drag.target: parent
        drag.axis: Drag.XAxis

        onPositionChanged: {
            const newX = Math.max(32, Math.min(parent.x, root.width - rect.rightMargin - 50))
            rect.leftMargin = newX
            rect.calcProportionalSize();
        }
    }

    // bottom-edge vertical drag
    MouseArea {
        id: dragBottomHandle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 20
        anchors.margins: -5
        cursorShape: Qt.SizeVerCursor

        property real startMouseY: 0
        property real startHeight: 0

        onPressed: (mouseEvent) => {
            startMouseY = mapToItem(rect.parent, Qt.point(mouseEvent.x, mouseEvent.y)).y
            startHeight = rect.height
        }

        onPositionChanged: (mouseEvent) => {
            const mouseY = mapToItem(rect.parent, Qt.point(mouseEvent.x, mouseEvent.y)).y
            const dy = mouseY - startMouseY
            const newHeight = Math.max(50, startHeight + dy)
            rect.bottomMargin = root.height - rect.topMargin - newHeight

            rect.height_pct = 100 / (root.height / rect.bottomMargin);
            rect.calcProportionalSize();
        }
    }

    // lower-left corner drag (horizontal + vertical)
    MouseArea {
        id: dragHandleLowerLeft
        z: dragLeftHandle.z + dragBottomHandle.z + 1
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -6
        anchors.leftMargin: -6

        width: 24
        height: 24
        cursorShape: Qt.SizeBDiagCursor

        property real startMouseX: 0
        property real startMouseY: 0
        property real startLeftMargin: 0
        property real startHeight: 0

        onPressed: (mouseEvent) => {
            const pos = mapToItem(rect.parent, Qt.point(mouseEvent.x, mouseEvent.y))
            startMouseX = pos.x
            startMouseY = pos.y
            startLeftMargin = rect.leftMargin
            startHeight = rect.height
        }

        onPositionChanged: (mouseEvent) => {
            const pos = mapToItem(rect.parent, Qt.point(mouseEvent.x, mouseEvent.y))
            const dx = pos.x - startMouseX
            const dy = pos.y - startMouseY

            // horizontal
            rect.leftMargin = Math.max(32, startLeftMargin + dx)

            // vertical
            const newHeight = Math.max(50, startHeight + dy)
            rect.bottomMargin = root.height - rect.topMargin - newHeight

            rect.calcProportionalSize();
        }
    }
}