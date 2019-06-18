import QtQuick 2.4

ManualKeyForm {
    Timer {
        id: timer
        interval: 1000; running: true; repeat: true
        onTriggered: {
            if (!qmlConnection.keyManual()) {
                timer.running = false;
                logout();
            }
        }
    }

    Component.onCompleted: {
        logoutTBn.visible = false
    }
}
