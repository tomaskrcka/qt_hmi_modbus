import QtQuick 2.4
import cz.nkgroup 1.0

FinalPageForm {
    Component.onCompleted: {
        progg.visible = false;
        if (qmlConnection.getNumOfTanks() < 2) {
            group2.visible = false
        }
    }

    Timer {
        id: finalPageTimer
        interval: 500; repeat: true; running: true
        onTriggered:
        {
            if (qmlConnection.stored()) {
                if (qmlConnection.default_position()) {
                    console.debug("ttt")
                    finalPageTimer.running = false
                    logout();
                } else {
                    finalPageTimer.running = false
                    stack.push(Qt.resolvedUrl("DefaultPosPage.qml"))
                }
            }
        }
    }
}
