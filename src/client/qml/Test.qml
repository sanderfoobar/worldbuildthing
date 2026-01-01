import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs


Rectangle {
    id: root

    property int default_thumb_meta_height: 32
    property int default_thumb_spacing: 4

    property string backgroundColor: "grey"
    property string borderColor: "#fdd663"
    property string areaColor: "#4d431d"

    visible: true
    color: backgroundColor
    anchors.fill: parent

    WheelHandler {
        onWheel: function(event){
            var down = event.angleDelta.y < 0;
            var delta = 256 + root.default_thumb_meta_height + root.default_thumb_spacing;
            if(down) {
                if(bla.contentY + delta < (bla.contentHeight - 256))
                    bla.contentY += delta;
            } else {
                if(bla.contentY - delta >= 0)
                    bla.contentY -= delta;
            }
        }
    }

    Flickable {
        id: bla
        anchors.fill: parent
        contentWidth: width
        interactive: false
        contentHeight: flow.implicitHeight

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn
            contentItem: Rectangle {
                implicitWidth: 48
                color: "green"
            }
        }

        Flow {
            id: flow
            width: parent.width
            spacing: root.default_thumb_spacing

            Repeater {
                model: TextureModel
                delegate: ColumnLayout {
                    property int thumb_size: 256

                    width: thumb_size
                    height: thumb_size + root.default_thumb_meta_height
                    spacing: 0

                    // Rectangle {
                    //     color: "red"
                    //     Layout.preferredHeight: parent.height - root.default_thumb_meta_height
                    //     Layout.preferredWidth: parent.width
                    // }

                    Image {
                        // Component.onCompleted: { console.log(index);}
                        Layout.preferredHeight: parent.height - root.default_thumb_meta_height
                        Layout.preferredWidth: parent.width
                        //color: index % 2 === 0 ? "green" : "blue"
                        source: "image://textureProvider/" + thumb;
                        fillMode: Image.PreserveAspectFit

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                let _size = parent.parent.thumb_size;
                                if(_size !== 256)
                                    parent.parent.thumb_size = 256;
                                else
                                    parent.parent.thumb_size = 256 * 2;
                            }
                        }
                    }

                    Rectangle {
                        Layout.preferredHeight: root.default_thumb_meta_height
                        Layout.preferredWidth: parent.width
                        color: "white"

                        Text {
                            color: "black"
                            text: name
                            font.pointSize: 16
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.leftMargin: 6
                            anchors.left: parent.left
                        }
                    }
                }
            }
        }
    }

    function get_random_color2() {
        var r = function () { return Math.floor(Math.random()*256) };
        return "#" + r() + r() + r();
    }

    Component.onCompleted: {
    }
}
