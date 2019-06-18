import QtQuick 2.4
import QtQuick.Dialogs 1.2
import cz.nkgroup 1.0

ItemSerialNumberForm {

    MessageDialog {
        id: messageDialog
        title: "Chyba zadání"
        text: "Není zadáno seriové číslo"
        modality: Qt.NonModal
    }

    Component.onCompleted: {
        sn2TF.forceActiveFocus()
        if (tankIndex === 1 && qmlConnection.getNumOfTanks() === 2) {
            tankSN.text = "pro nádrž 2";
        } else {
            tankSN.text = "pro nádrž 1";
        }
        sn2TF.forceActiveFocus()
    }
}
