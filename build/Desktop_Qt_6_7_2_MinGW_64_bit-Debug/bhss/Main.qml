import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: rootWindow
    width: 1200
    minimumWidth: 1200
    maximumWidth: 1200
    height: 680
    minimumHeight: 680
    maximumHeight: 680
    visible: true
    title: qsTr("BHSS - Login")
    color: "#1EA7F4"

    StackView {
        id: stackView

        initialItem: loginPage

        anchors.fill: parent
    }

    Component {
        id: loginPage
        LoginPage {}
    }

    Component {
        id: signupPage
        SignUpPage {}
    }

    Component {
        id:choosePlanPage
        ChoosePlanPage{}
    }
}
