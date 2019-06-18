import QtQuick 2.4

ControlErrorPageForm {
    Timer {
        id: checkerTimer
        interval: 500; repeat: true; running: true
        onTriggered:
        {
            if (qmlConnection.controlEnded() && (qmlConnection.controlOK())) {
                checkerTimer.stop()
                stack.push(Qt.resolvedUrl("MainProcessPage.qml"))
            }
        }
    }

}
