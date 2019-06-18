import QtQuick 2.4

SecurityPageForm {

    Timer {
        id: checkerTimer
        interval: 500; repeat: true; running: true
        onTriggered:
        {
            if (!qmlConnection.security()) {
                checkerTimer.stop()
                mainWindows.logout()
            }
        }
    }

    Component.onCompleted: {
        logoutTBn.visible = false
    }

}
