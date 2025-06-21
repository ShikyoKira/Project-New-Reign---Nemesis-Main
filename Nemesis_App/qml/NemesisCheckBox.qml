import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: customCheckBox

    property bool checked: false
    property bool preventStealing: false
    property bool propagateComposedEvents: false
    property color fillColor: "white"
    property color borderColor: "white"
    property color disabledFillColor: "grey"
    property color disabledBorderColor: "grey"
    property int borderWidth: 1

    signal toggled(bool checked)

    Rectangle {
        id: box

        border.color: customCheckBox.enabled ? borderColor : disabledBorderColor
        border.width: borderWidth

        width: customCheckBox.width
        height: customCheckBox.height
        radius: 4
        color: "transparent"

        Rectangle {
            id: fillBox
            color: customCheckBox.enabled ? fillColor : disabledFillColor
            anchors.fill: parent
            anchors.margins: Math.max(4, borderWidth * 2)
            visible: checked
        }

        MouseArea {
            anchors.fill: parent

            preventStealing: customCheckBox.preventStealing
            propagateComposedEvents: customCheckBox.propagateComposedEvents

            onClicked: {
                customCheckBox.toggled(!customCheckBox.checked);
            }
        }
    }
}
