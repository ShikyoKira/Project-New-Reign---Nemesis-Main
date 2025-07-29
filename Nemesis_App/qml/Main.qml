import QtQuick.Controls
import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtMultimedia
import QtQuick.Window
import QtQuick.Shapes

Window {
    id: window
    visible: true
    title: "Nemesis Unlimited Behavior Engine"
    flags: Qt.FramelessWindowHint | Qt.Window
    color: "transparent"

    height: 600
    width: modNameWidth + authorWidth + priorityWidth + 20
    minimumHeight: 600
    minimumWidth: modNameWidth + authorWidth + priorityWidth

    Item {
        id: theme

        property color gradientTop: "#2C2C2C"
        property color gradientMiddle: "#1C1C1C"
        property color gradientBottom: "#0E0E0E"
        
        property real blurOpacity: 0.75
    }

    property int edges: 0
    property int edgeOffest: 20
    property int modNameWidth: 540
    property int authorWidth: 100
    property int priorityWidth: 50
    property string dataDirectory
    property string stageDirectory
    property string enginePath
    property bool isLoading: true
    property int heightOffset: height

    Item {
        id: loader

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 20
        }

        width: 700
        height: 100
        clip: true
        z: 10
        opacity: 0.8

        Image {
            id: logo

            anchors {
                top: parent.top
                centerIn: parent
            }

            source: "qrc:/resources/logo.png"
            width: 200
            height: 200
            fillMode: Image.PreserveAspectFit
            opacity: 0
            z: 12
        }

        Rectangle {
            anchors {
                left: logo.right
                verticalCenter: parent.verticalCenter

                leftMargin: -80
            }

            width: 300
            height: 80
            z: 11
            clip: true
            color: "transparent"

            Image {
                id: logo_text

                anchors {
                    fill: parent
                    leftMargin: -800
                }

                source: "qrc:/resources/logo-text.png"
                fillMode: Image.PreserveAspectFit
                opacity: 1

            }
        }

        state: "PENDING"

        states: [
            State {
                name: "PENDING"
            },
            State {
                name: "START"

                PropertyChanges {
                    target: logo
                    opacity: 1
                    height: 100
                    anchors.horizontalCenterOffset: -140
                }
            }
        ]

        transitions: [
            Transition {
                from: "PENDING"
                to: "START"

                SequentialAnimation {
                    NumberAnimation {
                        target: logo
                        properties: "opacity, height"
                        duration: 300
                    }

                    PauseAnimation {
                        duration: 100
                    }

                    ParallelAnimation {
                        NumberAnimation {
                            target: logo
                            property: "anchors.horizontalCenterOffset"
                            duration: 300
                        }

                        PropertyAnimation {
                            target: logo_text
                            property: "anchors.leftMargin"
                            to: 15
                            duration: 300
                        }
                    }

                    PropertyAnimation {
                        target: window
                        property: "heightOffset"
                        to: 0
                        duration: 300
                    }

                    ParallelAnimation {
                        PropertyAnimation {
                            target: topRightCorner
                            property: "opacity"
                            to: theme.blurOpacity
                        }

                        PropertyAnimation {
                            target: contentZone
                            property: "opacity"
                            to: 1
                            duration: 250
                        }
                    }
                }
            }
        ]
    }

    Component.onCompleted: {
        dataDirectory = appConfig.getDataDirectory();
        stageDirectory = appConfig.getStageDirectory();
        enginePath = dataDirectory + "/nemesis_engine/Nemesis_Engine" + (Qt.platform.os === "windows" ? ".exe" : "");
        minimumHeight = appConfig.getHeight();
        minimumWidth = appConfig.getWidth();
        modNameWidth = appConfig.getModNameWidth();
        authorWidth = appConfig.getAuthorWidth();
        priorityWidth = appConfig.getPriorityWidth();
        heightOffset = Math.max(minimumHeight, height);

        modHandler.dataReady.connect(function(data) {
            listModel.clear();

            for(let item of data) {
                listModel.append(item);
                modNameWidth = Math.max(modNameWidth, item["name"].length * 14);
                authorWidth = Math.max(authorWidth, item["author"].length * 14);
            }
        });

        modHandler.populateModel(dataDirectory + "/nemesis_engine/mods", appConfig.isDevMode());
        loader.state = "START";

        if (!appConfig.isDevMode()) {
            preloadEngine();
        }
    }

    function preloadEngine() {
        let args = [
                "-p"
                , appConfig.getPlatform()
                , "-pi"
                , "-pr"
            ];

        if (!dataDirectory.isEmpty()) {
            args.push("-d");
            args.push(dataDirectory);
        }

        if (!stageDirectory.isEmpty()) {
            args.push("-o");
            args.push(stageDirectory);
        }

        appLauncher.launchProgram(enginePath, args, true);
    }

    function setEdges(x, y) {
        edges = 0;

        if(x < edgeOffest) edges |= Qt.LeftEdge;

        if(x > (width - edgeOffest))  edges |= Qt.RightEdge;

        if(y < edgeOffest) edges |= Qt.TopEdge;

        if(y > (height - edgeOffest)) edges |= Qt.BottomEdge;
    }

    MouseArea {
        id: resizeArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton

        cursorShape: {
            return !containsMouse ? Qt.ArrowCursor:
                   edges == 3 || edges == 12 ? Qt.SizeFDiagCursor :
                   edges == 5 || edges == 10 ? Qt.SizeBDiagCursor :
                   edges & 9 ? Qt.SizeVerCursor :
                   edges & 6 ? Qt.SizeHorCursor : Qt.ArrowCursor;
        }

        onPositionChanged: setEdges(mouseX, mouseY);

        onPressed: {
            setEdges(mouseX, mouseY);

            if(!edges || !containsMouse) return;

            startSystemResize(edges);
        }
    }

    MouseArea {
        id: topLeftCorner

        anchors {
            top: parent.top
            left: parent.left
            margins: 5
        }

        width: 15
        height: 15
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        cursorShape: Qt.SizeFDiagCursor
        z: 1

        onPressed: {
            if(!containsMouse) return;

            startSystemResize(Qt.TopEdge | Qt.LeftEdge);
        }
    }

    MouseArea {
        id: topRightCorner

        anchors {
            top: parent.top
            right: parent.right
        }

        width: 50
        height: 50
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        cursorShape: Qt.PointingHandCursor
        z: 1
        opacity: 0

        onEntered: {
            if (contentZone.opacity != 1) return;

            topRightCorner.opacity = 1;
        }

        onExited: {
            if (contentZone.opacity != 1) return;

            topRightCorner.opacity = 0.8;
        }

        onPressed: {
            if(!containsMouse) return;

            Qt.callLater(Qt.quit);
        }

        Image {
            anchors.fill: parent
            source: "qrc:/resources/exit-button.png"
            z: 2
        }
    }

    MouseArea {
        id: bottomLeftCorner

        anchors {
            bottom: parent.bottom
            left: parent.left
            margins: 5
        }

        width: 15
        height: 15
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        cursorShape: Qt.SizeBDiagCursor
        z: 1

        onPressed: {
            if(!containsMouse) return;

            startSystemResize(Qt.BottomEdge | Qt.LeftEdge);
        }
    }

    MouseArea {
        id: bottomRightCorner

        anchors {
            bottom: parent.bottom
            right: parent.right
            margins: 5
        }

        width: 15
        height: 15
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        cursorShape: Qt.SizeFDiagCursor
        z: 1

        onPressed: {
            if(!containsMouse) return;

            startSystemResize(Qt.BottomEdge | Qt.RightEdge);
        }
    }

    Rectangle {
        id: windowBackground

        anchors {
            fill: parent
            topMargin: 7.5
            leftMargin: 7.5
            rightMargin: 7.5
            bottomMargin: window.heightOffset + 7.5
        }

        z: 0
        color: "#1F1F21"
        opacity: 0.95
        radius: 20

        MouseArea {
            id: dragArea
            anchors.fill: parent

            property var mousePressedPosition: Qt.point(0, 0)

            onPressed: {
                mousePressedPosition = Qt.point(mouseX, mouseY);
            }

            onPositionChanged: {
                if (!dragArea.pressed) return;

                window.x += mouseX - mousePressedPosition.x;
                window.y += mouseY - mousePressedPosition.y;
            }
        }
    }

    FocusScope {
        id: focusScope
        anchors.fill: parent
        focus: true

        property bool shiftHeld: false
        property int lastToggledIndex: -1

        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Shift) {
                shiftHeld = true;
                console.log("Shift key pressed");
            }
        }

        Keys.onReleased: (event) => {
            if (event.key === Qt.Key_Shift) {
                shiftHeld = false;
                console.log("Shift key released");
            }
        }
    }

    Rectangle {
        id: contentZone

        anchors.fill: parent
        anchors.topMargin: 100

        color: "transparent"
        z: 1
        opacity: 0

        ColumnLayout {
            id: columnLayout

            anchors {
                fill: parent
                margins: 30
            }

            Gradient {
                id: rowBlockGradient

                GradientStop { position: 0.0; color: theme.gradientTop }  // Top light color
                GradientStop { position: 0.5; color: theme.gradientMiddle }  // Middle color
                GradientStop { position: 1.0; color: theme.gradientBottom }  // Bottom darker color
            }

            Component {
                id: dragDelegate

                MouseArea {
                    id: dragArea

                    property bool held: false
                    required property bool checked
                    required property string display_name
                    required property string name
                    required property string author
                    required property string site
                    required property string mod_code

                    anchors {
                        left: parent?.left
                        right: parent?.right
                        rightMargin: scrollBar.width + 2
                    }

                    enabled: button.enabled
                    height: content.height
                    hoverEnabled: true
                    preventStealing: true
                    scrollGestureEnabled: false
                    opacity: theme.blurOpacity

                    onEntered: {
                        opacity = 1;
                    }

                    onExited: {
                        opacity = theme.blurOpacity;
                    }

                    onPressed: {
                        held = true;
                        listView.dragItem = content;
                    }

                    onReleased: {
                        held = false;
                        listView.dragItem = null;
                    }

                    drag {
                        target: held ? content : undefined
                        axis: Drag.YAxis
                        smoothed: false
                    }

                    onDoubleClicked: {
                        Qt.openUrlExternally(dragArea.site);
                        held = false;
                    }

                    ToolTip {
                        visible: dragArea.containsMouse
                        delay: 500
                        text: dragArea.site
                    }

                    Rectangle {
                        id: content

                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }

                        width: dragArea.width
                        height: 50
                        radius: 15
                        gradient: rowBlockGradient
                        opacity: held ? 0.5 : 1.0

                        border.width: 1
                        border.color: held ? "white" : "grey"

                        Drag.active: dragArea.held
                        Drag.source: dragArea
                        Drag.hotSpot.x: width / 2
                        Drag.hotSpot.y: height / 2


                        states: State {
                            when: dragArea.held

                            ParentChange {
                                target: content
                                parent: contentZone
                            }

                            AnchorChanges {
                                target: content
                                anchors {
                                    horizontalCenter: undefined
                                    verticalCenter: undefined
                                }
                            }
                        }

                        RowLayout {
                            id: row
                            anchors {
                                left: parent.left
                                leftMargin: 20
                            }

                            width: parent.width
                            height: parent.height

                            NemesisCheckBox  {
                                id: checkbox

                                Layout.alignment: Qt.AlignVCenter

                                preventStealing: true
                                enabled: button.enabled && appConfig.isDevMode()
                                checked: dragArea.checked
                                width: 20
                                height: 20
                                z: 15

                                onToggled: (checked) => {
                                    var curIndex = dragArea.DelegateModel.itemsIndex;
                                    var itemData;

                                    if (focusScope.shiftHeld) {
                                        if (focusScope.lastToggledIndex < curIndex) {
                                            for (var i = focusScope.lastToggledIndex + 1; i < curIndex; ++i) {
                                                itemData = visualModel.items.get(i);
                                                listModel.setProperty(itemData.model.index, "checked", checked);
                                                console.log(itemData.model.mod_code + ": " + itemData.model.checked.toString());
                                            }
                                        } else if (focusScope.lastToggledIndex > curIndex) {
                                            for (var k = curIndex + 1; k < focusScope.lastToggledIndex; ++k) {
                                                itemData = visualModel.items.get(k);
                                                listModel.setProperty(itemData.model.index, "checked", checked);
                                                console.log(itemData.model.mod_code + ": " + itemData.model.checked.toString());
                                            }
                                        }
                                    }

                                    itemData = visualModel.items.get(curIndex);
                                    listModel.setProperty(itemData.model.index, "checked", checked);
                                    console.log(itemData.model.mod_code + ": " + itemData.model.checked.toString());
                                    focusScope.lastToggledIndex = curIndex;
                                }
                            }

                            Text  {
                                font.pixelSize: 20

                                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                                Layout.preferredWidth: window.modNameWidth
                                Layout.fillWidth: true

                                text: dragArea.name
                                color: "white"
                                leftPadding: 20
                            }

                            Row {
                                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                                Layout.rightMargin: 50

                                height: Math.max(authorText.height, indexText.height)

                                Text  {
                                    id: authorText

                                    anchors.verticalCenter: parent.verticalCenter

                                    font.pixelSize: 20

                                    text: dragArea.author
                                    color: "white"
                                }

                                Text  {
                                    id: indexText

                                    font.pixelSize: 20

                                    text: dragArea.DelegateModel.itemsIndex + 1
                                    color: "white"
                                    width: window.priorityWidth
                                    horizontalAlignment: Text.AlignRight
                                }
                            }
                        }
                    }

                    DropArea {
                        anchors {
                            fill: parent
                            margins: 10
                        }

                        onEntered: (drag) => {
                            visualModel.items.move(
                                    drag.source.DelegateModel.itemsIndex,
                                    dragArea.DelegateModel.itemsIndex);
                        }
                    }
                }
            }

            DelegateModel {
                id: visualModel

                model: ListModel {
                    id: listModel
                }

                delegate: dragDelegate
            }

            ScrollView {
                id: scrollView

                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 200
                Layout.preferredHeight: 200

                ScrollBar.vertical.visible: false

                ListView {
                    id: listView

                    anchors.fill: parent

                    clip: true

                    model: visualModel
                    spacing: 2

                    property QtObject dragItem

                    // Required to force drag update on scroll
                    onContentYChanged: {
                        if (dragItem == undefined) return;

                        dragItem.Drag.hotSpot.y += 1;
                        dragItem.Drag.hotSpot.y -= 1;
                    }

                    NemesisScrollBar {
                        id: scrollBar

                        opacity: 0.75
                        interactivelyAppear: true
                    }
                }
            }

            Rectangle {
                id: startZone

                Layout.alignment: Qt.AlignHCenter
                Layout.margins: 20

                property int transitionTime: 250

                width: button.width
                height: button.height

                color: "transparent"
                state: "DONE"

                function start() {
                    if (!button.enabled) return;

                    startZone.state = "PROCESSING";
                    outputArea.text = "";

                    console.log("========= START ===========");
                    
                    if (!appConfig.isDevMode()) {
                        let mods = [];
                        
                        for (var i = 0; i < visualModel.count; ++i) {
                            var item = visualModel.items.get(i);
                            console.log(item.model.mod_code + ": " + item.model.checked.toString());

                            if (!item.model.checked) continue;

                            mods.push(item.model.mod_code);
                        }
                    
                        console.log("preloaded");
                        appLauncher.writeToProgram(mods);
                    } else {
                        let args = [
                                "-p"
                                , appConfig.getPlatform()
                                , "-pi"
                            ];
                            
                        if (!dataDirectory.isEmpty()) {
                            args.push("-d");
                            args.push(dataDirectory);
                        }

                        if (!stageDirectory.isEmpty()) {
                            args.push("-o");
                            args.push(stageDirectory);
                        }
                        
                        args.push("-m");

                        for (var i = 0; i < visualModel.count; ++i) {
                            var item = visualModel.items.get(i);
                            console.log(item.model.mod_code + ": " + item.model.checked.toString());

                            if (!item.model.checked) continue;

                            args.push(item.model.mod_code);
                        }
                    
                        appLauncher.launchProgram(enginePath, args, false);
                    }

                    console.log("========= END ===========");
                }

                function finish() {
                    delayFinisher1.start();
                    delayFinisher2.start();
                }

                Timer {
                    id: delayFinisher1
                    interval: 800
                    running: false
                    repeat: false
                    onTriggered: {
                        outputArea.text += progressBarFill.completeText;
                    }
                }

                Timer {
                    id: delayFinisher2
                    interval: 1300
                    running: false
                    repeat: false
                    onTriggered: {
                        progressLoader.stop();
                        startZone.state = "DONE";
                        progressBarFill.width = Qt.binding(() => 0);
                    }
                }

                NemesisButton {
                    id: button

                    width: 150

                    buttonText: "START"
                    textBold: true
                    textPixelSize: 20
                    hoverEnabled: true
                    enabled: true
                    opacity: theme.blurOpacity
                    borderColor: buttonBorderColor

                    property bool effective: true
                    property color buttonBorderColor: "white"

                    onEntered: {
                        opacity = 1;
                    }

                    onExited: {
                        opacity = theme.blurOpacity;
                    }

                    onClicked: {
                        if (!effective) return;

                        startZone.start();
                        progressLoader.start();
                    }

                    Rectangle {
                        id: proxyBorder
                        z: -1
                        radius: 10

                        anchors {
                            fill: parent
                            margins: -1
                        }

                        color: buttonBorderColor
                        opacity: 0
                    }
                }

                states: [
                    State {
                        name: "DONE"

                        PropertyChanges {
                            target: progressBarText
                            opacity: 1
                        }

                        PropertyChanges {
                            target: progressBarFill
                            opacity: 0
                        }

                        PropertyChanges {
                            target: button
                            visible: true
                        }
                        
                        PropertyChanges {
                            target: progressBar
                            width: button.width
                            radius: 10
                        }

                        PropertyChanges {
                            target: progressBarOverlay
                            anchors.margins: 0
                        }

                        PropertyChanges {
                            target: innerProgressBlock
                            anchors.margins: -1
                        }
                    },
                    State {
                        name: "PROCESSING"

                        PropertyChanges {
                            target: progressBarText
                            opacity: 0
                        }

                        PropertyChanges {
                            target: progressBarFill
                            opacity: 1
                            width: 100
                            completed: false
                            completeText: ""
                        }

                        PropertyChanges {
                            target: progressBar
                            width: scrollView.width
                            visible: true
                            radius: 30
                        }

                        PropertyChanges {
                            target: button
                            enabled: false
                            buttonBorderColor: "transparent"
                            effective: false
                        }
                        
                        PropertyChanges {
                            target: proxyBorder
                            opacity: 0
                        }

                        PropertyChanges {
                            target: progressBarOverlay
                            anchors.margins: 5
                        }

                        PropertyChanges {
                            target: innerProgressBlock
                            anchors.margins: -5
                        }
                    }
                ]

                transitions: [
                    Transition {
                        from: "PROCESSING"
                        to: "DONE"

                        SequentialAnimation {
                            NumberAnimation {
                                target: progressBarText
                                property: "opacity"
                                duration: startZone.transitionTime
                            }
                        }

                        SequentialAnimation {
                            NumberAnimation {
                                target: progressBar
                                property: "width"
                                duration: startZone.transitionTime
                            }

                            PropertyAction {
                                target: progressBar
                                property: "visible"
                                value: false
                            }

                            PauseAnimation {
                                duration: 400
                            }

                            PropertyAnimation {
                                target: proxyBorder
                                property: "opacity"
                                to: 1
                                duration: 100
                            }
                            
                            PropertyAction {
                                target: proxyBorder
                                property: "opacity"
                                value: 0
                            }

                            PropertyAction {
                                target: button
                                property: "buttonBorderColor"
                                value: progressBar.border.color
                            }

                            PropertyAction {
                                target: button
                                property: "effective"
                                value: true
                            }
                        }
                    },
                    Transition {
                        from: "DONE"
                        to: "PROCESSING"
                        
                        PropertyAction {
                            target: proxyBorder
                            property: "opacity"
                            value: 0
                        }

                        SequentialAnimation {
                            NumberAnimation {
                                target: progressBarText
                                property: "opacity"
                                duration: startZone.transitionTime
                            }

                            PropertyAction {
                                target: button
                                property: "visible"
                                value: true
                            }
                        }

                        NumberAnimation {
                            target: progressBar
                            property: "width"
                            duration: startZone.transitionTime
                        }
                    }
                ]

                Rectangle {
                    id: progressBar

                    anchors.centerIn: button

                    width: button.width
                    height: button.height
                    color: button.buttonColor
                    gradient: button.gradient
                    visible: false
                    radius: 10

                    Behavior on radius {
                        NumberAnimation {
                            duration: startZone.transitionTime
                        }
                    }

                    Rectangle {
                        id: innerProgressBlock

                        anchors.fill: parent
                        anchors.margins: -1

                        Behavior on anchors.margins {
                            NumberAnimation {
                                duration: startZone.transitionTime
                            }
                        }

                        clip: true
                        color: "white"
                        opacity: 0
                        radius: progressBar.radius
                        z: 1

                        property color solidColor: "#245818"
                        property real maxOpacity: 0.8
                        property real minOpacity: 0.0
                        property real fadeLength: 0.3
                        property real timing: 0

                        Rectangle {
                            id: progressBarOverlay

                            anchors.fill: parent
                            anchors.margins: 0

                            Behavior on anchors.margins {
                                NumberAnimation {
                                    duration: startZone.transitionTime
                                }
                            }

                            color: button.buttonColor
                            gradient: button.gradient
                            radius: progressBar.radius
                            z: 3
                        }

                        Rectangle {
                            id: progressLoading

                            anchors.horizontalCenter: parent

                            height: innerProgressBlock.height
                            width: innerProgressBlock.fadeLength * innerProgressBlock.width

                            x: ((innerProgressBlock.timing * (1 + innerProgressBlock.fadeLength)) - innerProgressBlock.fadeLength) * innerProgressBlock.width
                            z: 2

                            gradient: Gradient {
                                orientation: Gradient.Horizontal

                                GradientStop {
                                    position: 0.0
                                    color: "transparent"
                                }
                                GradientStop {
                                    position: 1.0
                                    color: innerProgressBlock.solidColor
                                }
                            }
                        }

                        ShaderEffect {
                            anchors.fill: parent

                            z: 3

                            property color cornerColor: progressBarOverlay.color
                            property real radius: innerProgressBlock.radius
                            property real itemWidth: width
                            property real itemHeight: height
                            property real backgroundOpacity: windowBackground.opacity

                            fragmentShader: "qrc:/shaders/ProgressBarCorners.frag.qsb"
                            vertexShader: "qrc:/shaders/ProgressBarCorners.vert.qsb"
                        }

                        SequentialAnimation {
                            id: progressLoader
                            loops: Animation.Infinite

                            PropertyAction {
                                target: innerProgressBlock
                                property: "timing"
                                value: 0
                            }

                            NumberAnimation {
                                target: innerProgressBlock
                                property: "timing"
                                to: 1.0
                                duration: 1500
                                easing.type: Easing.InOutQuint;
                            }

                            PauseAnimation {
                                duration: 200
                            }

                            SequentialAnimation {
                                loops: 2
                                running: true

                                PropertyAction {
                                    target: innerProgressBlock
                                    property: "timing"
                                    value: 0
                                }

                                NumberAnimation {
                                    target: innerProgressBlock
                                    property: "timing"
                                    to: 1.0
                                    duration: 1000
                                    easing.type: Easing.InOutQuint;
                                }
                            }

                            PauseAnimation {
                                duration: 200
                            }
                        }
                    }

                    Component.onCompleted: {
                        innerProgressBlock.opacity = 1;
                        progressBarFill.opacity = 1;
                    }

                    Rectangle {
                        id: progressBarFill

                        anchors {
                            left: parent.left
                            top: parent.top
                            bottom: parent.bottom
                        }

                        property bool completed: false
                        property string completeText

                        opacity: 0
                        width: 0
                        radius: innerProgressBlock.radius
                        z: 4
                        color: "transparent"

                        ShaderEffect {
                            id: fillShaderEffect

                            anchors {
                                fill: parent
                                leftMargin: 7.5
                                rightMargin: 7.5
                                topMargin: 7.5
                                bottomMargin: 7.5
                            }

                            property color solidColor: "#5DEF3D"
                            property real maxOpacity: 0.8
                            property real minOpacity: 0.3
                            property real fadeLength: 0.2 + 0.2 * (progressBarFill.width / progressBar.width)
                            property real widthRatio: progressBarFill.width / progressBar.width
                            property real itemWidth: width
                            property real itemHeight: height
                            property real radius: 17.5

                            fragmentShader: "qrc:/shaders/ProgressBarFill.frag.qsb"
                            vertexShader: "qrc:/shaders/ProgressBarFill.vert.qsb"
                        }

                        Behavior on width {
                            NumberAnimation {
                                duration: 1000
                                easing.type: Easing.InOutQuint
                            }
                        }
                    }

                    Text {
                        id: progressBarText

                        anchors.centerIn: progressBar

                        text: button.buttonText
                        color: button.buttonTextColor
                        z: 3

                        font {
                            pixelSize: 20
                            bold: button.textBold
                        }
                    }

                    Text {
                        anchors.centerIn: progressBar

                        text: (progressBarFill.width / progressBar.width * 100).toFixed(2) + "%"
                        color: "white"
                        z: 3
                        opacity: startZone.state == "DONE" ? 0.0 : 1.0
                        visible: startZone.state != "DONE"

                        font {
                            pixelSize: 20
                            bold: button.textBold
                        }

                        Behavior on opacity {
                            NumberAnimation {
                                duration: 500
                            }
                        }
                    }
                }
            }

            ScrollView {
                id: outputView

                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 200
                Layout.preferredHeight: 200
                opacity: 0.8

                ScrollBar.vertical.visible: false

                TextArea {
                    id: outputArea

                    color: "white"
                    readOnly: true
                    topPadding: 15
                    leftPadding: 20
                    rightPadding: 20
                    bottomPadding: 10
                    wrapMode: TextEdit.WordWrap

                    font.pixelSize: 15

                    background: Rectangle {
                        color: "#080808"
                        radius: 20
                    }

                    onTextChanged: {
                        outputArea.cursorPosition = outputArea.length;
                    }
                }
            }

            Connections {
                target: appLauncher

                onOutputReceived: (output) => {
                    if (progressBarFill.completed) {
                        progressBarFill.completeText += output;
                        return;
                    }

                    outputArea.text += output;
                }

                onErrorReceived: (error) => {
                    outputArea.text += error;
                }

                onFinishedReceived: {
                    startZone.finish();

                    if (!appConfig.isDevMode()) {
                        preloadEngine();
                    }
                }

                onProgressUp: (step, max) => {
                    progressBarFill.completed = step / max == 1;
                    progressBarFill.width = Qt.binding(() => progressBar.width * step / max);
                }
            }
        }
    }
}
