import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.VectorImage

Rectangle {
    id: root
    color: "#101428"
    anchors.fill: parent
    property int sidebarWidth: 76

    Rectangle {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 1
        color: "#22a0ff"
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        MainSidebarButton {
            Layout.preferredWidth: root.sidebarWidth
            Layout.preferredHeight: root.sidebarWidth

            icon: "qrc:/icons_blender/properties.svg"
            checked: window.sidebarItem === "properties" || mouseHovering === true

            onClicked: {
                window.mainSidebarClicked("properties");
            }
        }

        MainSidebarButton {
            Layout.preferredWidth: root.sidebarWidth
            Layout.preferredHeight: root.sidebarWidth

            icon: "qrc:/icons_blender/outliner.svg"
            checked: window.sidebarItem === "itemtree" || mouseHovering === true

            onClicked: {
                window.mainSidebarClicked("itemtree");
            }
        }

        MainSidebarButton {
            Layout.preferredWidth: root.sidebarWidth
            Layout.preferredHeight: root.sidebarWidth

            icon: "qrc:/icons_blender/file_folder_large.svg"

            onClicked: {

            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}