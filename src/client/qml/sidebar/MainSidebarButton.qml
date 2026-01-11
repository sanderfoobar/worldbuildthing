import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.VectorImage

Rectangle {
    id: root
    color: "transparent"
    property string icon: ""
    Layout.preferredWidth: root.sidebarWidth
    Layout.preferredHeight: root.sidebarWidth
    property bool mouseHovering: false
    property bool checked: false

    signal clicked();

    Rectangle {
        anchors.fill: parent
        anchors.margins: 12
        color: "transparent"
        border.width: root.active || root.checked
        border.color: "#22a0ff";
        radius: 6

        VectorImage {
            anchors.fill: parent
            anchors.margins: 10
            preferredRendererType: VectorImage.CurveRenderer
            source: icon;
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onEntered: root.mouseHovering = true
        onExited: root.mouseHovering = false
        onClicked: root.clicked()
    }
}