import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import cz.nkgroup 1.0


CamErrorPageForm {
    id: camErrorForm
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
            if (stack.currentItem === camErrorForm) {
                if (key === Qt.Key_Return) {
                    usercodetmp = qmlConnection.userExists(userCode)
                    if (usercodetmp > 0) {
                        userCode = ""
                        continueBtn.visible = true

                    } else {
                        console.debug(userCode)
                        incorrectUserLbl.visible = true
                        timer.start()
                        userCode = ""
                    }

                } else if (key !== Qt.Key_Shift) {
                    userCode += String.fromCharCode(key)
                }
            }
        }
    }
/*
    Connections {
        target: tryAgainBtn
        onClicked: {
            qmlConnection.loginUser("123");
            userCodeGlobal = "Debug user";
            logoutTBn.visible = true
            stack.push(Qt.resolvedUrl("ChooseNumberOfItems.qml"))
        }
    }
*/
    Connections {
        target: continueBtn
        onClicked: {
            qmlConnection.camErrorSet(false)
            stack.pop()

        }
    }
}
