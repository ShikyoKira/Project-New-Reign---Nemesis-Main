import QtQuick 2

Rectangle {
    id: scrollZone

    property bool interactivelyAppear: false
    property int fadeDelay: 1000
    property QtObject target: parent

    anchors {
        right: target.right
        margins: radius
    }

    width: Math.min(0.01 * target.width, 10) * 1.2
    height: target.height
    opacity: scrollZone.interactivelyAppear ? 0 : 1
    color: "transparent"

    Behavior on opacity {
        OpacityAnimator {
            duration: 300
        }
    }

    Timer {
        id: fadeOutTimer
        interval: scrollZone.fadeDelay
        repeat: false
        onTriggered: scrollZone.opacity = 0
    }

    HoverHandler {
        id: hoverHandler

        acceptedDevices: PointerDevice.Mouse
        enabled: scrollZone.interactivelyAppear

        onHoveredChanged: {
            if (hovered) {
                scrollZone.opacity = 1;
                fadeOutTimer.stop();
                return;
            }

            fadeOutTimer.restart();
        }
    }

    Rectangle {
        id: scrollBackground

        anchors {
            centerIn: scrollZone.Center
        }

        width: scrollZone.width / 1.2
        height: scrollZone.height
        radius: 0.5 * width
        color: "#101010"

        MouseArea {
            id: backgroundMouse

            anchors.fill: scrollBackground

            drag.target: scrollBar
            drag.axis: Drag.YAxis
            drag.minimumY: 0
            drag.maximumY: scrollZone.target.height - scrollBar.height

            onPressed: {
                scrollZone.target.contentY = Math.min(
                                                Math.max(0, mouseY - scrollBar.height / 2)
                                                , scrollZone.height - scrollBar.height
                                            ) * scrollZone.target.contentHeight / scrollZone.target.height;
                scrollBar.color = "dark grey";
            }

            onPositionChanged: {
                scrollZone.target.contentY = scrollBar.y * scrollZone.target.contentHeight / scrollZone.target.height;
            }

            onReleased: {
                scrollBar.color = "grey";
            }

        }
    }

    Rectangle {
        id: scrollBar

        anchors {
            centerIn: scrollZone.Center
            margins: radius
        }

        color: "grey"
        width: Math.min(0.01 * scrollZone.target.width, 10) * 1.2
        radius: 0.5 * width
        height: scrollZone.target.height / scrollZone.target.contentHeight * scrollZone.target.height
        y: scrollZone.target.contentY / scrollZone.target.contentHeight * scrollZone.target.height
        visible: scrollZone.target.height < scrollZone.target.contentHeight

        onYChanged: {
            if (!scrollZone.interactivelyAppear) return;

            scrollZone.opacity = 1;

            if (hoverHandler.hovered) return;

            fadeOutTimer.restart();
        }

        MouseArea {
            id: scrollBarMouse

            anchors.fill: scrollBar

            drag.target: scrollBar
            drag.axis: Drag.YAxis
            drag.minimumY: 0
            drag.maximumY: scrollZone.target.height - scrollBar.height

            onPressed: {
                scrollBar.color = "dark grey";
            }

            onPositionChanged: {
                scrollZone.target.contentY = scrollBar.y * scrollZone.target.contentHeight / scrollZone.target.height;
            }

            onReleased: {
                scrollBar.color = "grey";
            }
        }
    }
}
