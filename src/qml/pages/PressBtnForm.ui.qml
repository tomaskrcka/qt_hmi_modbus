import QtQuick 2.4

Item {
    width: 600
    height: 380

    property alias timerLbl: timerLbl

    Text {
        id: text1
        x: 202
        y: 89
        text: qsTr("Pro ukončení procesu")
        font.pixelSize: 20
    }

    Text {
        id: timerLbl
        x: 293
        y: 178
        width: 69
        height: 24
        text: qsTr("0")
        font.pixelSize: 15
    }

    Text {
        id: timerLbl1
        x: 155
        y: 178
        width: 46
        height: 24
        text: qsTr("Doba do konce:")
        font.pixelSize: 15
    }

    Text {
        id: text2
        x: 180
        y: 129
        text: qsTr("stiskněte a držte obě tlačítka")
        font.pixelSize: 20
    }
}
