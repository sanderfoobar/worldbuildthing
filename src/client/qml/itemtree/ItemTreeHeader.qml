import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.VectorImage

Rectangle {
    id: root
    color: "#101428"
    anchors.fill: parent

    Text {
        anchors.fill: parent
        text: window.sidebarTitle
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 12
        color: "white"
    }
}