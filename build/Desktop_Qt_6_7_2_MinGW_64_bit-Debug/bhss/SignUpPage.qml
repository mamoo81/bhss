import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import SehirlerData

Item {
    id:root

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

        GridLayout {
            // width: parent.width * .6
            anchors.centerIn: parent
            columns: 2
            width: 600

            Text {
                Layout.fillWidth: true
                Layout.column: 0
                Layout.row: 0
                Layout.columnSpan: 2
                Layout.preferredHeight: 100
                text: qsTr("Firma Bilgileri")
                font.pointSize: 28
                color: "white"
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
            }

            TextField {
                id: firmaIsmiTextField
                focus: true
                Layout.preferredHeight: 42
                Layout.fillWidth: true
                placeholderText: qsTr("Firma ismi")
                // Layout.column: 0
                Layout.row: 1
                Layout.columnSpan: 2
                font.pointSize: 16
                verticalAlignment: Text.AlignVCenter

                KeyNavigation.tab: yetkiliTextField
            }
            TextField {
                id: yetkiliTextField
                Layout.preferredHeight: 42
                Layout.fillWidth: true
                placeholderText: qsTr("Firma yetkilisi")
                Layout.column: 0
                Layout.row: 2
                font.pointSize: 16
                verticalAlignment: Text.AlignVCenter

                KeyNavigation.tab: yetkilicepTextField
            }
            TextField {
                id: yetkilicepTextField
                Layout.preferredHeight: 42
                Layout.preferredWidth: parent.width * .3
                placeholderText: qsTr("Yetkili cep no")
                Layout.column: 1
                Layout.row: 2
                font.pointSize: 16
                verticalAlignment: Text.AlignVCenter

                inputMethodHints: Qt.ImhDialableCharactersOnly
                maximumLength: 10

                validator: RegularExpressionValidator {
                    regularExpression: /^5[0-9]{9}$/
                }
            }

            TextField {
                placeholderText: qsTr("Firma adresi")
                Layout.fillWidth: true
                Layout.row: 3
                Layout.columnSpan: 2
                Layout.preferredHeight: 42
                font.pointSize: 16
                verticalAlignment: Text.AlignVCenter
            }

            ComboBox {
                id: iller_comboBox
                Layout.preferredHeight: 42
                Layout.fillWidth: true
                Layout.column: 0
                Layout.row: 4
                font.pointSize: 16

                model: Sehirler.sehirModel
                delegate: ItemDelegate {
                    width: iller_comboBox.width
                    contentItem: Text {
                        text: model.il
                        font.pointSize: 16
                    }
                    highlighted: iller_comboBox.highlightedIndex === index
                }
                // model: ["ADANA", "MERSİN", "YERSİN"]
                textRole: "il"
                valueRole: "plaka"
                // displayText: model[currentIndex].il
            }
            ComboBox {

            }
        }
    }
}
