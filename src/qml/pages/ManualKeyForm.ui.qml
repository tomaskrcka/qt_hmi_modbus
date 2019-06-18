import QtQuick 2.4

Item {
    width: 600
    height: 380

    Image {
        id: image
        x: 160
        y: 111
        width: 185
        height: 159
        source: "../resources/12_emergency.png"
    }

    Text {
        id: text1
        x: 181
        y: 55
        text: qsTr("Manuální režim")
        font.pixelSize: 21
    }
}
