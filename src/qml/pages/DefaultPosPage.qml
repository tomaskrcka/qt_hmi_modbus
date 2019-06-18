import QtQuick 2.4
import cz.nkgroup 1.0

DefaultPosPageForm {
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

    QmlConnection {
        id: qmlConnection
    }

    Timer {
        id: timerdef
        interval: 100; running: true; repeat: true
        onTriggered: {
            if ((qmlConnection.default_position()) && (qmlConnection.stored())) {
                timerdef.stop()
                mainWindows.logout()
            }
        }
    }
}
