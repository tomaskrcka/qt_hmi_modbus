import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    width: 600
    height: 380

    property alias restartBtn: restartBtn

    Button {
        id: restartBtn
        x: 250
        y: 274
        text: qsTr("Restart")
    }

    Text {
        id: text1
        x: 158
        y: 216
        text: qsTr("Problém ve spojení s databází")
        wrapMode: Text.WrapAnywhere
        font.pixelSize: 21
    }

    Text {
        id: text2
        x: 191
        y: 247
        text: qsTr("Zkontrolujte nastavení a připojení k síti")
        font.pixelSize: 12
        wrapMode: Text.WrapAnywhere
    }

    Image {
        id: image
        x: 152
        y: 70
        width: 292
        height: 121
        source: "../resources/db_error.jpg"
    }
}
