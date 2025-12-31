import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    property alias textItem: textItem
    property alias text: textItem.text
    property var textColor: "#c7cce7"
    property var textActive: "#fdfdfd"
    property bool active: false

    radius: active ? 2 : 0
    color: active ? "#393e5e" : "transparent"

    Text {
        id: textItem
        text: "test1235555"
        color: parent.active ? parent.textActive : parent.textColor
        font.pointSize: 15
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: parent.active = true
        onExited: parent.active = false
    }
}