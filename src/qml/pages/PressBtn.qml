import QtQuick 2.4
import cz.nkgroup 1.0


PressBtnForm {
    Timer {
        id: checkerTimer
        interval: 500; repeat: true; running: true
        onTriggered:
        {
            timerLbl.text = qmlConnection.remainingTime().toString()
            if (qmlConnection.procedureEnded()) {
                checkerTimer.stop()
                stack.push(Qt.resolvedUrl("FinalPage.qml"))
            }
        }
    }
}
