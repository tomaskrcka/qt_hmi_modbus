import QtQuick 2.4
import cz.nkgroup 1.0

MainProcessPageForm {
    QmlConnection {
        id: qmlConnection
    }

    States {
        id: states
    }

    Timer {
        id: analogTimer
        interval: 100; running: true; repeat: true
        onTriggered:
        {
            var press = qmlConnection.getPressure().toFixed(2)

            if (qmlConnection.getNumOfTanks() === 2) {
                var press2 = qmlConnection.getPressure2().toFixed(2)
                pressure2TF.text = press2
            }

            pressure1TF.text = press
        }
    }

    Timer {
        id: timer2
        interval: 1000; repeat: true;
        onTriggered:
        {
            timeTF.text = qmlConnection.remainingTime().toString()
        }
    }

    Timer {
        id: checkerTimer
        interval: 500; repeat: true; running: true
        onTriggered:
        {
            if (!qmlConnection.controlEnded()) {
                instTF.text = "Kontrola systému";
            }

            if (qmlConnection.controlEnded() && (qmlConnection.controlOK())) {
                instTF.text = "Stiskněte tlačítko 1 pro ponořováni";
            }

            if (qmlConnection.waitingStart()) {
                instTF.text = "Proces začne za 30 sekund";
                logoutTBn.visible = false;
            }
/*
            if (qmlConnection.waitingPress()) {
                instTF.text = "Čekání na dosažení tlaku";
            }
*/
            if (qmlConnection.procedureStarted()) {
                instTF.text = "Byl spuštěn proces tlakování - čekejte";
                textTimeLbl.text = "Doba ponoření (sec):";
            }

            if (qmlConnection.controlEnded() && (!qmlConnection.controlOK())) {
                checkerTimer.stop()
                stack.push(Qt.resolvedUrl("ControlErrorPage.qml"))
            } else if (qmlConnection.btnControl()) {
                checkerTimer.stop()
                stack.push(Qt.resolvedUrl("PressBtn.qml"))
            } else if (!qmlConnection.logged()) {
                checkerTimer.stop()
                mainWindows.logout()
            } else if (qmlConnection.switchManual())  {
                instTF.text = "Nelze začít cyklus - aktivní manuální režim"
                instTF.visible = true
            }
        }
    }

    Component.onCompleted: {
        if (qmlConnection.getNumOfTanks() < 2) {
            pressure2Rec.visible = false
            textpress2TF.visible = false
            name2TF.visible = false
            sn2TF.visible = false
        }
        checkerTimer.start()

        name1TF.text = qmlConnection.getProgram(0);
        sn1TF.text = qmlConnection.getSN(0);

        if (qmlConnection.getNumOfTanks() === 2) {
            name2TF.text = qmlConnection.getProgram(1);
            sn2TF.text = qmlConnection.getSN(1);
        }
    }

}
