import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import cz.nkgroup 1.0

Item {
    width: 600
    height: 380

    QmlConnection {
        id: qmlConnection
    }

    Button {
        id: oneTankBtn
        x: 160
        y: 186
        text: qsTr("Jedna nádrž")
    }

    Button {
        id: twoTanksBtn
        x: 328
        y: 186
        text: qsTr("Dvě nádrže")
    }

    Label {
        id: label
        x: 193
        y: 86
        text: qsTr("Výběr počtu nádrží")
        font.bold: true
        font.pointSize: 23
    }

    Connections {
        target: twoTanksBtn
        onClicked: {
            qmlConnection.setNumOfTanks(2)
            numOfItemsActual = 1
            tankIndex = 0
            stack.push(Qt.resolvedUrl("ItemSerialNumber.qml"))
        }
    }

    Connections {
        target: oneTankBtn
        onClicked: {
            qmlConnection.setNumOfTanks(1)
            numOfItemsActual = 0
            tankIndex = 0
            stack.push(Qt.resolvedUrl("ItemSerialNumber.qml"))
        }
    }

    Text {
        id: text1
        x: 231
        y: 125
        text: qsTr("na tlakování")
        font.bold: true
        font.pixelSize: 23
    }
}
