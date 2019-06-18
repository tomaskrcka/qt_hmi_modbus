import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import cz.nkgroup 1.0


LoginPageForm {
    id: loginForm
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

    QmlConnection {
        id: qmlConnection
    }

    Timer {
        id: timer
        interval: 2000; running: false; repeat: false
        onTriggered: incorrectUserLbl.visible = false
    }

    Timer {
        id: timer2
        interval: 1000; running: true; repeat: true
        onTriggered: {
            if (qmlConnection.switchManual()) {
                incorrectUserLbl.text = "Nelze začít cyklus - aktivní manuální režim"
                incorrectUserLbl.visible = true
            } else {
                incorrectUserLbl.visible = false
                incorrectUserLbl.text = "Neplatné uživatelské číslo"
            }
        }
    }

    Connections {
        target: stack
        property var userCode: String("")
        property var usercodetmp: 0
        onKeyReceived:
        {
            if (stack.currentItem === loginForm) {
                if (qmlConnection.switchManual()) {
                    incorrectUserLbl.text = "Nelze začít cyklus - aktivní manuální režim"
                    incorrectUserLbl.visible = true
                } else {
                    if ((key === Qt.Key_Return) || (key == Qt.Key_Enter)) {
                        usercodetmp = qmlConnection.loginUser(userCode)
                        if (usercodetmp > 0) {
                            userCodeGlobal = usercodetmp;
                            logoutTBn.visible = true
                            userCode = ""
                            stack.push(Qt.resolvedUrl("ChooseNumberOfItems.qml"))
                        } else {
                            console.debug(userCode)
                            incorrectUserLbl.visible = true
                            timer.start()
                            userCode = ""
                        }

                    } else if ((key >= Qt.Key_0) && (key <= Qt.Key_9)) {
                        // qml: %1%%2%%3%
                        userCode += String.fromCharCode(key)
                    }
                }
            }
        }
    }

    Connections {
        target: loginBtn
        onClicked: {
            qmlConnection.loginUser("123");
            userCodeGlobal = "Debug user";
            logoutTBn.visible = true
            stack.push(Qt.resolvedUrl("ChooseNumberOfItems.qml"))
        }
    }
}
