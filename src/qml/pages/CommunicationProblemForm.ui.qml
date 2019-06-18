import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    width: 600
    height: 380

    Text {
        id: text1
        x: 160
        y: 178
        text: qsTr("Problém v komunikaci s PLC")
        wrapMode: Text.WrapAnywhere
        font.pixelSize: 21
    }

    Text {
        id: text2
        x: 239
        y: 209
        text: qsTr("Nutný zásah technika")
        font.pixelSize: 12
        wrapMode: Text.WrapAnywhere
    }

    Image {
        id: image
        x: 154
        y: 32
        width: 292
        height: 121
        source: "../resources/db_error.jpg"
    }
}
