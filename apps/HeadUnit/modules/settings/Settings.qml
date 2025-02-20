import QtQuick 2
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2
import QtMultimedia
// import QtBluetooth

Item{
    id:settings
    signal colorValueChanged(int value)

    Text{
        id:ambientlighting
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 150
        text: "Ambient Lighting:"
        color: myColor
        font {
            family: "Inter"
            pixelSize: 35
        }
    }

    Slider {
        id: colorSlider
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 225
        width: 500
        height: 20
        from: 0
        to: 360
        value: 180

        onValueChanged: settings.colorValueChanged(value)

        background: Rectangle {
            x: colorSlider.leftPadding
            y: colorSlider.topPadding + colorSlider.availableHeight / 2 - height / 2
            width: colorSlider.availableWidth
            height: 20
            implicitWidth: 700
            implicitHeight: 10
            radius: 10
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "red" }
                GradientStop { position: 0.33; color: "green" }
                GradientStop { position: 0.66; color: "blue" }
                GradientStop { position: 1.0; color: "red" }
            }
        }

        handle: Rectangle {
            x: colorSlider.leftPadding + colorSlider.visualPosition * (colorSlider.availableWidth - width)
            y: colorSlider.topPadding + colorSlider.availableHeight / 2 - height / 2
            width: 30
            height: 30
            radius: 15
            color: Qt.hsva(colorSlider.value / 360, 1, 1, 1)
            border.color: "white"
        }
    }

    Row{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 300
        spacing: 20

        Text{
            id:bluetooth
            text: "Bluetooth:"
            color: myColor
            font {
                family: "Inter"
                pixelSize: 35
                // italic: true
            }
        }

        Switch {
            id: bluetoothswitch
            indicator: Rectangle {
                implicitWidth: 48
                implicitHeight: 26
                x: bluetoothswitch.leftPadding
                y: parent.height / 2 - height / 2
                radius: 13
                color: bluetoothswitch.checked ? myColor : "#262626"
                border.color: bluetoothswitch.checked ? myColor : "#595959"

                Rectangle {
                    x: bluetoothswitch.checked ? parent.width - width : 0
                    width: 26
                    height: 26
                    radius: 13
                    color: bluetoothswitch.down ? "#262626" : "#404040"
                    border.color: bluetoothswitch.checked ? (bluetoothswitch.down ? myColor : myColor) : "#595959"
                }
            }

            contentItem: Text {
                text: bluetoothswitch.text
                font: bluetoothswitch.font
                opacity: enabled ? 1.0 : 0.3
                color: bluetoothswitch.down ? "#17a81a" : "#21be2b"
                verticalAlignment: Text.AlignVCenter
                leftPadding: bluetoothswitch.indicator.width + bluetoothswitch.spacing
            }
        }
    }
}
