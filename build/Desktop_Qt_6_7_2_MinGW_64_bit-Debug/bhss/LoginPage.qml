import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {

    Image {
        z: -1
        anchors.fill: parent
        sourceSize {
            width: parent.width
            height: parent.height
        }

        source: "qrc:/images/login-background.png"
    }

    RowLayout {
        anchors.fill: parent
        ColumnLayout {
            spacing: 10
            anchors.rightMargin: 40

            Layout.alignment: Qt.AlignCenter

            Text {
                text: qsTr("Merhaba")
                font.pixelSize: 32
                color: "white"
                Layout.alignment: Qt.AlignCenter
            }
            Text {
                text: qsTr("Basat Hızlı Satış Sistemi'ne hoş geldiniz.")
                font.pixelSize: 18
                color: "white"
                wrapMode: Text.WordWrap
                Layout.alignment: Qt.AlignCenter
            }

            RowLayout {
                spacing: 10
                Layout.alignment: Qt.AlignHCenter

                ToolButton {
                    icon.source: "qrc:/images/web.svg"
                    icon.color: hovered ? "black" : "white"
                    icon.width: 32
                    icon.height: 32
                }
                ToolButton {
                    icon.source: "qrc:/images/instagram.svg"
                    icon.color: hovered ? "black" : "white"
                    icon.width: 32
                    icon.height: 32
                }
                ToolButton {
                    icon.source: "qrc:/images/facebook.svg"
                    icon.color: hovered ? "black" : "white"
                    icon.width: 32
                    icon.height: 32
                }
                ToolButton {
                    icon.source: "qrc:/images/x.com-logo.svg"
                    icon.color: hovered ? "black" : "white"
                    icon.width: 32
                    icon.height: 32
                }
            }

            ToolButton {
                id: signupButton
                Layout.alignment: Qt.AlignHCenter
                contentItem: Text {
                    text: qsTr("Kayıt ol")
                    color: parent.hovered ? "black" : "white"
                    font.pointSize: 16
                }

                onClicked: {
                    stackView.push(choosePlanPage)

                    rootWindow.setMinimumWidth(1200)
                    rootWindow.setWidth(1200)

                }
            }
        }

        ColumnLayout {

            spacing: 10

            Image {
                source: "qrc:/images/lgn-ui-image.svg"
                sourceSize {
                    width: 200
                    height: 200
                }
            }

            TextField {
                id: userName_TextField
                placeholderText: "Kullanıcı adı"
                font.pointSize: 14
                Layout.minimumWidth: 200
                // Layout.alignment: Qt.AlignHCenter
                // focus: true
                KeyNavigation.tab: password_TextField
            }

            TextField {
                id: password_TextField
                placeholderText: "Şifre"
                echoMode: TextInput.Password
                font.pointSize: 14
                Layout.minimumWidth: 200
                // Layout.alignment: Qt.AlignHCenter
                passwordCharacter: "*"
                KeyNavigation.tab: loginButton
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 10
                Button {
                    id: exitButton
                    text: "Çıkış"
                    Layout.minimumWidth: 60
                    font.pointSize: 14

                    onClicked: Qt.quit()
                }
                Button {
                    id: loginButton
                    text: "Giriş"
                    Layout.minimumWidth: 60
                    font.pointSize: 14
                }
            }
        }
    }

    RowLayout {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 42
        // layoutDirection: Qt.RightToLeft
        spacing: 5

        Item {
            Layout.fillWidth: true
        }

        Image {
            source: "qrc:/images/no-licence.svg"
            sourceSize {
                width: 32
                height: 32
            }
        }

        ToolButton {
            icon.source: "qrc:/images/setting.svg"
            icon.width: 32
            icon.height: 32
        }
    }
}
