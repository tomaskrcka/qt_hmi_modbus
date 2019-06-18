import QtQuick 2.4
import cz.nkgroup 1.0

LoadingPageForm {
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

    QmlConnection {
        id: qmlConnection
    }

    Timer {
        id: timer
        interval: 100; running: true; repeat: true
        onTriggered: {
            if (qmlConnection.appStatus() > 0) {
                timer.running = false;
                if (!qmlConnection.connError()) {
                    if (qmlConnection.appStatus() === 1) {
                        //OK
                        stack.pop()
                        stack.push(Qt.resolvedUrl("LoginPage.qml"))
                    } else {
                        stack.pop()
                        stack.push(Qt.resolvedUrl("ErrorDBConnPage.qml"))
                    }
                }
            }
        }
    }
}
