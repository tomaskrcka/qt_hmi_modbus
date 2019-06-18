import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    width: 600
    height: 380

    BusyIndicator {
        id: busyIndicator
        x: 208
        y: 180
        width: 183
        height: 135
    }

    Text {
        id: text1
        x: 192
        y: 116
        text: qsTr("Přípojování k databázi")
        font.pixelSize: 22
    }
}
