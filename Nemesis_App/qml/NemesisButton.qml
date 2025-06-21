import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: customButton

    property bool hoverEnabled: false

    property string buttonText: "Press me"
    property int textPixelSize: 20
    property bool textBold: false

    property color buttonColor: "#1C1C1C"
    property color borderColor: "white"

    property color buttonTextColor: "white"
    property color buttonTextColor2: "grey"

    readonly property QtObject gradient: buttonBackground.gradient

    signal clicked
    signal entered
    signal exited

    width: 150
    height: 50

    Rectangle {
        id: buttonBackground
        anchors.fill: parent
        border.color: customButton.borderColor
        radius: 10

        function adjustColor(color, offset) {
            return Qt.rgba(
                        Math.max(0, Math.min(255, Math.round(color.r * 255) + offset)) / 255,
                        Math.max(0, Math.min(255, Math.round(color.g * 255) + offset)) / 255,
                        Math.max(0, Math.min(255, Math.round(color.b * 255) + offset)) / 255,
                        color.a
                        )
        }

        property int offsetToFirst: 16
        property int offsetToLast: -14

        gradient: Gradient {
            GradientStop { position: 0.0; color: buttonBackground.adjustColor(customButton.buttonColor, buttonBackground.offsetToFirst) }
            GradientStop { position: 0.5; color: customButton.buttonColor }
            GradientStop { position: 1.0; color: buttonBackground.adjustColor(customButton.buttonColor, buttonBackground.offsetToLast) }
        }

        Text {
            id: buttonText

            anchors.centerIn: parent

            font {
                bold: customButton.textBold
                pixelSize: customButton.textPixelSize
            }

            text: customButton.buttonText
            color: customButton.buttonTextColor
        }

        // Depress effect when the button is pressed
        states: [
            State {
                name: "pressed"
                when: buttonMouseArea.pressed

                PropertyChanges {
                    target: buttonText
                    color: customButton.buttonTextColor2
                }

                PropertyChanges {
                    target: buttonBackground
                    offsetToFirst: -14
                    offsetToLast: 16
                }
            },
            State {
                name: "released"
                when: !buttonMouseArea.pressed

                PropertyChanges {
                    target: buttonText
                    color: customButton.buttonTextColor
                }

                PropertyChanges {
                    target: buttonBackground
                    offsetToFirst: 16
                    offsetToLast: -14
                }
            }
        ]
    }

    Rectangle {
        id: mask

        anchors.fill: parent

        z: 2
        color: parent.enabled ? "transparent" : "black"
        radius: 10
        opacity: 0.6
    }

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
        hoverEnabled: customButton.hoverEnabled

        onEntered: {
            if (!parent.enabled) return;

            customButton.entered();
        }

        onExited: {
            if (!parent.enabled) return;

            customButton.exited();
        }

        onClicked: {
            if (!parent.enabled) return;

            customButton.clicked();
        }
    }
}
