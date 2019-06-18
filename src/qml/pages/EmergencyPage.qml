import QtQuick 2.4

EmergencyPageForm {

    Timer {
        id: checkerTimer
        interval: 500; repeat: true; running: true
        onTriggered:
        {
            if (!qmlConnection.emergency()) {
                checkerTimer.stop()
                mainWindows.logout()
            }
        }
    }

    Component.onCompleted: {
        logoutTBn.visible = false
    }

}
