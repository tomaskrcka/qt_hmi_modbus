import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import cz.nkgroup 1.0

Item {
    width: 600
    height: 380

    property alias pressure1Rec: pressure1Rec
    property alias pressure2Rec: pressure2Rec
    property alias pressure1TF: pressure1TF
    property alias pressure2TF: pressure2TF
    property alias textpress2TF: textpress2TF
    property alias sn1TF: sn1TF
    property alias name1TF: name1TF
    property alias name2TF: name2TF
    property alias sn2TF: sn2TF
    property alias timeTF: timeTF
    property alias instTF: instructionText
    property alias textTimeLbl: textTimeLbl
    property alias startBtn: startBtn

    Rectangle {
        id: pressure1Rec
        x: 58
        y: 107
        width: 103
        height: 48
        color: "#ffffff"
        border.color: "#0c0101"

        Text {
            id: pressure1TF
            x: 27
            y: 13
            text: qsTr("000,0")
            font.pixelSize: 18
        }
    }

    Rectangle {
        id: timeRec
        x: 188
        y: 195
        width: 106
        height: 42
        color: "#ffffff"
        border.color: "#0c0101"

        Text {
            id: timeTF
            x: 48
            y: 8
            width: 11
            height: 22
            text: qsTr("0")
            visible: true
            font.pixelSize: 18
        }
    }

    Rectangle {
        id: pressure2Rec
        x: 58
        y: 195
        width: 103
        height: 42
        color: "#ffffff"
        border.color: "#0c0101"
        Text {
            id: pressure2TF
            x: 27
            y: 9
            text: qsTr("000,0")
            visible: true
            font.pixelSize: 18
        }
    }

    Rectangle {
        id: rectangle3
        x: 183
        y: 94
        width: 340
        height: 74
        color: "#ffffff"
        border.color: "#0c0101"

        Text {
            id: name1TF
            x: 8
            y: 14
            text: qsTr("Nazev vyrobek 1>")
            font.pixelSize: 12
        }

        Text {
            id: sn1TF
            x: 149
            y: 14
            text: qsTr("Seriove cislo 1")
            font.pixelSize: 12
        }

        Text {
            id: name2TF
            x: 8
            y: 45
            width: 101
            height: 15
            text: qsTr("Nazev vyrobek 2>")
            font.pixelSize: 12
        }

        Text {
            id: sn2TF
            x: 149
            y: 45
            text: qsTr("Seriove cislo 2")
            font.pixelSize: 12
        }
    }

    Text {
        id: text7
        x: 65
        y: 77
        text: qsTr("Tlak v nádrži 1: ")
        font.pixelSize: 12
    }

    Text {
        id: textpress2TF
        x: 65
        y: 174
        text: qsTr("Tlak v nádrži 2: ")
        font.pixelSize: 12
    }

    Text {
        id: textTimeLbl
        x: 188
        y: 174
        text: qsTr("Zbývající čas (sec): ")
        font.pixelSize: 12
    }

    Button {
        id: startBtn
        x: 318
        y: 195
        width: 205
        height: 42
        text: qsTr("Potvrdit zadání ")
    }

    Connections {
        target: startBtn
        onClicked: {
            timer2.running = true
            startBtn.enabled = false
            instructionText.visible = true
            qmlConnection.confirmCycle()
        }
    }

    Text {
        id: instructionText
        x: 128
        y: 31
        color: "#ff0909"
        text: qsTr("Držte tlačítko 1 pro ponoření")
        visible: false
        font.pixelSize: 19
    }
}
