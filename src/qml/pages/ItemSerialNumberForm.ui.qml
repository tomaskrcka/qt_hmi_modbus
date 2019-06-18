import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import cz.nkgroup 1.0

Item {
    width: 600
    height: 380

    property alias sn2TF: sn2TF
    property alias tankSN: tankSN

    Button {
        id: okBtn
        x: 326
        y: 190
        text: qsTr("Dále")
    }

    TextField {
        id: sn2TF
        x: 174
        y: 129
        width: 252
        height: 40
        text: qsTr("")
        focus: true
    }

    Text {
        id: text1
        x: 208
        y: 45
        text: qsTr("Zadejte seriové číslo")
        font.pixelSize: 20
    }

    Connections {
        target: okBtn
        onClicked: {
            if (sn2TF.text.length == 0) {
                messageDialog.visible = true
            } else {
                qmlConnection.setTankSN(tankIndex, sn2TF.text)
                stack.push(Qt.resolvedUrl("ChooseTypeOfItem.qml"))
            }
        }
    }

    Connections {
        target: sn2TF
        onAccepted: {
            if (sn2TF.text.length == 0) {
                messageDialog.visible = true
            } else {
                qmlConnection.setTankSN(tankIndex, sn2TF.text)
                stack.push(Qt.resolvedUrl("ChooseTypeOfItem.qml"))
            }
        }
    }

    Text {
        id: tankSN
        x: 262
        y: 86
        text: qsTr("Text")
        font.pixelSize: 20
    }
}
