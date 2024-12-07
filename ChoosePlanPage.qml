import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Item {
    id: root

    property int selectedPlan: 0

    RowLayout {
        id: topBar
        height: 38
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        ToolButton {
            id: signupBackButton
            icon.source: "qrc:/images/previous.svg"
            // icon.color: hovered ? "black" : "white"


            contentItem: Row {
                spacing: 5

                Image {
                    source: "qrc:/images/previous.svg"
                    width: 28
                    height: 28
                    sourceSize {
                        width: 32
                        height: 32
                    }
                    anchors.margins: 4
                    anchors.verticalCenter: parent.verticalCenter

                    layer.enabled: true
                    layer.effect: MultiEffect {
                        brightness: 1.0
                        colorization: 1.0
                        colorizationColor: signupBackButton.hovered ? "black" : "white"
                    }
                }

                Text {
                    text: qsTr("Geri dön")
                    anchors.verticalCenter: parent.verticalCenter
                    color: signupBackButton.hovered ? "black" : "white"
                    font.pixelSize: 14
                }
            }

            onClicked: {
                stackView.pop()
            }
        }
    }

    Item {
        width: parent.width
        height: parent.height - topBar.height
        anchors {
            top: topBar.bottom
            left: parent.left
            right: parent.right
        }

        clip: true

        ColumnLayout {
            id: columnItem
            width: root.width
            spacing: 20

            RowLayout {
                width: parent.width
                spacing: 40
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 40

                Rectangle {
                    id: denemeChoseBox
                    width: 200
                    height: 280
                    radius: 8

                    color: selectedPlan === 1 ? "#81D4FA" : "white"
                    border.color : "black"
                    border.width : 0

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10

                        Text {
                            text: qsTr("3 ay ücretsiz")
                            Layout.alignment: Qt.AlignHCenter
                            font.pointSize: 16
                            font.family: "Monospace"
                        }
                        Text {
                            text: qsTr("deneme")
                            Layout.alignment: Qt.AlignHCenter
                            font.pointSize: 16
                            font.family: "Monospace"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent

                        hoverEnabled: true

                        onEntered: {
                            parent.border.width = 2
                        }
                        onExited: {
                            parent.border.width = 0
                        }

                        onClicked: {
                            if(selectedPlan != 1){
                                selectedPlan = 1
                            }
                            else{
                                selectedPlan = 0
                            }
                        }
                    }
                }

                Rectangle {
                    width: 200
                    height: 280
                    radius: 8

                    color: selectedPlan === 2 ? "#81D4FA" : "white"
                    border.color : "black"
                    border.width: 0
                    clip: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10
                        anchors.margins: 10

                        Item {
                            width: parent.width
                            Layout.preferredHeight: 60
                        }

                        Text {
                            text: qsTr("Aylık plan")
                            Layout.alignment: Qt.AlignHCenter
                            font.pointSize: 16
                            font.family: "Monospace"
                        }

                        Text {
                            text: qsTr("₺199") + " + " + qsTr("KDV")
                            Layout.alignment: Qt.AlignHCenter
                            font.pointSize: 22
                            font.family: "Monospace"
                        }

                        Item {
                            width: parent.width
                            Layout.fillHeight: true
                        }
                    }

                    MouseArea {
                        anchors.fill: parent

                        hoverEnabled: true

                        onEntered: {
                            parent.border.width = 2
                        }
                        onExited: {
                            parent.border.width = 0
                        }

                        onClicked: {
                            if(selectedPlan != 2){
                                selectedPlan = 2
                            }
                            else{
                                selectedPlan = 0
                            }
                        }
                    }
                }
                Rectangle {
                    width: 200
                    height: 280
                    radius: 8
                    color: selectedPlan === 3 ? "#81D4FA" : "white"
                    border.color : "black"
                    border.width: 0
                    clip: true

                    Rectangle {
                        width: 180
                        height: 30
                        x: -40
                        y: 30
                        rotation: -45
                        color: "red"
                        Text {
                            anchors.centerIn: parent
                            text: "En iyi seçim"
                            color: "white"
                            font.family: "Monospace"
                            font.pointSize: 14
                            // font.bold: true
                        }
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10
                        anchors.margins: 10

                        Item {
                            width: parent.width
                            Layout.preferredHeight: 60
                        }
                        Text {
                            text: qsTr("Yıllık plan")
                            Layout.alignment: Qt.AlignHCenter
                            font.pointSize: 16
                            font.family: "Monospace"
                        }

                        Text {
                            text: qsTr("₺1999") + " + " + qsTr("KDV")
                            Layout.alignment: Qt.AlignHCenter
                            font.pointSize: 22
                            font.family: "Monospace"
                        }

                        Item {
                            width: parent.width
                            Layout.fillHeight: true
                        }
                    }

                    MouseArea {
                        anchors.fill: parent

                        hoverEnabled: true

                        onEntered: {
                            parent.border.width = 2
                        }
                        onExited: {
                            parent.border.width = 0
                        }

                        onClicked: {
                            if(selectedPlan != 3) {
                                selectedPlan = 3
                            }
                            else {
                                selectedPlan = 0
                            }
                        }
                    }
                }

                Rectangle {
                    width: 200
                    height: 280
                    radius: 8
                    color: selectedPlan === 4 ? "#81D4FA" : "white"
                    border.color : "black"
                    border.width: 0
                    clip: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10

                        Text {
                            text: qsTr("Ömür boyu lisans")
                            Layout.alignment: Qt.AlignHCenter
                            font.pointSize: 16
                            font.family: "Monospace"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent

                        hoverEnabled: true

                        onEntered: {
                            parent.border.width = 2
                        }
                        onExited: {
                            parent.border.width = 0
                        }

                        onClicked: {
                            if(selectedPlan != 4) {
                                selectedPlan = 4
                            }
                            else {
                                selectedPlan = 0
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignHCenter
                Text {
                    height: 200
                    text : "Seçiminizi yapın ve kaydolun."
                    Layout.preferredWidth: 600
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 36
                    color: "white"
                    Layout.alignment: Qt.AlignHCenter
                    padding: 20
                    font.family: "Monospace"
                }

                ToolButton {
                    id: contiuneButton
                    icon.source: "qrc:/images/previous.svg"
                    // icon.color: hovered ? "black" : "white"
                    Layout.alignment: Qt.AlignHCenter
                    width: 150
                    height: 75
                    visible: selectedPlan !== 0 ? true : false

                    contentItem: Row {
                        spacing: 5

                        Text {
                            text: qsTr("Devam et")
                            anchors.verticalCenter: parent.verticalCenter
                            color: contiuneButton.hovered ? "black" : "white"
                            font.pixelSize: 18
                            font.bold: true
                        }

                        Image {
                            source: "qrc:/images/right-arrow.svg"
                            width: 28
                            height: 28
                            sourceSize {
                                width: 32
                                height: 32
                            }
                            anchors.margins: 4

                            layer.enabled: true
                            layer.effect: MultiEffect {
                                brightness: 1.0
                                colorization: 1.0
                                colorizationColor: contiuneButton.hovered ? "black" : "white"
                            }
                        }
                    }

                    onClicked: {
                        stackView.push(signupPage)
                    }
                }
            }
        }
    }
}

