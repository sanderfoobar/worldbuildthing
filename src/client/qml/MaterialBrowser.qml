import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import Main 1.0

ColumnLayout {
    spacing: 0

    Rectangle {
        Layout.preferredHeight: column.implicitHeight + 24
        Layout.fillWidth: true
        color: "#101428"

        ColumnLayout {
            id: column
            anchors.fill: parent
            anchors.topMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 8

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                QTextField {
                    Layout.fillWidth: true

                    focus: true
                    Keys.onPressed: function (event) {
                        if (event.key === Qt.Key_Tab) {
                            event.accepted = true
                        }
                    }
                }

                QButton { text: "Button 1" }
                QButton { text: "Button 2" }
            }

            Flow {
                Layout.fillWidth: true
                spacing: 8
                flow: Flow.LeftToRight
                Repeater {
                    model: 10
                    QCheckBox {
                        text: "Option " + (index + 1)
                        focusPolicy: Qt.ClickFocus
                    }
                }
            }
        }
    }

    Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        color: "transparent"

        Rectangle {
            anchors.fill: parent
            Layout.preferredWidth: parent.width - 20
            clip: true

            color: "#99000000"

            Flickable {
                id: root
                anchors.fill: parent

                contentWidth: parent.width
                interactive: false
                contentHeight: flow.implicitHeight

                property int default_thumb_meta_height: 32
                property int default_thumb_spacing: 4

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                    contentItem: Rectangle {
                        implicitWidth: 48
                        color: "green"
                        border.width: 2
                        border.color: "#4D92B6"
                    }
                }

                Flow {
                    id: flow
                    width: parent.width - (root.anchors.margins * 2)
                    spacing: root.default_thumb_spacing

                    property int target_thumb: 256
                    property real min_thumb_factor: 0.7
                    property real max_thumb_factor: 1.3

                    property int columns: Math.max(1, Math.floor((width + spacing) / (target_thumb * min_thumb_factor + spacing)))
                    property int thumb_size: Math.min(target_thumb * max_thumb_factor,
                        Math.max(target_thumb * min_thumb_factor,
                            Math.floor((width - (columns - 1) * spacing) / columns)
                        )
                    )

                    Repeater {
                        model: 1000
                        delegate: Rectangle {
                            color: Qt.rgba(Math.random(), Math.random(), Math.random(), 1)
                            width: flow.thumb_size
                            height: flow.thumb_size + root.default_thumb_meta_height

                            Text {
                                anchors.centerIn: parent
                                color: "white"
                                font.pointSize: 14
                                text: parent.width + " × " + parent.height
                            }
                        }
                    }

                    onWidthChanged: {
                        columns = Math.max(1, Math.floor((width + spacing) / (target_thumb * min_thumb_factor + spacing)))
                        thumb_size = Math.min(target_thumb * max_thumb_factor,
                            Math.max(target_thumb * min_thumb_factor,
                                Math.floor((width - (columns - 1) * spacing) / columns)
                            )
                        )
                    }
                }
            }
        }
    }

    Item {
        Layout.fillWidth: true
        Layout.preferredHeight: 2
    }
}
