import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
// import QtQuick.VirtualKeyboard 2.2
import QtQuick.Dialogs 1.2
import cz.nkgroup 1.0

ApplicationWindow {
    id: mainWindows
    title: qsTr("Modbus")
    width: 800
    height: 480
    visible: true

    property var userCodeGlobal: "N/A"
    property var linerCodeGlobal: "N/A"
    property var numOfItemsActual: 0
    property var tankIndex: 0

    Timer {
        id: connErrorTimer
        interval: 500; repeat: true; running: true
        onTriggered:
        {
            if (qmlConnection.connError()) {
                connErrorTimer.stop()
                stack.push(Qt.resolvedUrl("pages/CommunicationProblem.qml"))
            } else if (qmlConnection.keyManual()) {
                connErrorTimer.stop()
                stack.push(Qt.resolvedUrl("pages/ManualKey.qml"))
            } else if (qmlConnection.security()) {
                connErrorTimer.stop()
                stack.push(Qt.resolvedUrl("pages/SecurityPage.qml"))
            } else if (qmlConnection.emergency()) {
                connErrorTimer.stop()
                stack.push(Qt.resolvedUrl("pages/EmergencyPage.qml"))
            } else if (qmlConnection.loginstate()) {
                connErrorTimer.stop()
                logout();
            }

        }
    }

    QmlConnection {
        id: qmlConnection
    }

    StackView {
        id: stack
        signal keyReceived(int key)
        initialItem:
        {
             Qt.resolvedUrl("pages/LoadingPage.qml")
            //Qt.resolvedUrl("pages/FinalPage.qml")
        }
        anchors.fill: parent
        focus: true
        Keys.onPressed: {
            keyReceived(event.key)
        }
    }

    header: ToolBar {
        id: tabBar
        visible: true
        RowLayout {
            anchors.fill: parent

            Label {
                padding: 10
                text: qsTr("Uživatel: ")
            }
            Label {
                text: userCodeGlobal
            }
            Item { Layout.fillWidth: true }
            BorderImage {
                id: borderImage
            }
        }
    }

    footer: ToolBar {
        id: tabBar2
        visible: true
        RowLayout {
            anchors.fill: parent
            ToolButton {
                id: backTB
                visible: false
                padding: 10
                text: qsTr("Zpět")
                onPressed:
                {
                    // Dialog.visible = true
                }
            }
            Item { Layout.fillWidth: true }
            ToolButton {
                id: logoutTBn
                visible: false
                padding: 10
                text: qsTr("Odhlásit")
                onPressed:
                {
                    logoutDialog.visible = true
                }
            }
        }
    }

    function logout() {
        while (stack.depth > 2) {
            stack.pop()
        }

        stack.forceActiveFocus()
        userCodeGlobal = "N/A"
        logoutTBn.visible = false
        connErrorTimer.start()
        qmlConnection.logout()
        if (qmlConnection.camError()) {
            stack.push(Qt.resolvedUrl("pages/CamErrorPage.qml"))
        }
    }

    MessageDialog {
        id: logoutDialog
        title: "Potvrzení odhlášení"
        text: "Opravdu odhlásit ?"
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            while (stack.depth > 2) {
                stack.pop()
            }

            stack.forceActiveFocus()
            userCodeGlobal = "N/A"
            linerCodeGlobal = ""
            logoutTBn.visible = false
            qmlConnection.logout()

            if (qmlConnection.camError()) {
                stack.push(Qt.resolvedUrl("pages/CamErrorPage.qml"))
            }
        }
        onNo: {
            stack.forceActiveFocus()
        }

    }
}
