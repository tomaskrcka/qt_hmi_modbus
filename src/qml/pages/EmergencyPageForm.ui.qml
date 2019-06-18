import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    width: 600
    height: 380

    Text {
        id: text1
        x: 171
        y: 98
        text: qsTr("Nouzové vyjetí")
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        font.pixelSize: 24
    }

    Text {
        id: text2
        x: 85
        y: 211
        text: qsTr("Čekejte na vyjetí do výchozí pozice")
        font.pixelSize: 19
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    }

    Image {
        id: image
        x: 40
        y: 91
        width: 100
        height: 100
        source: "../resources/12_emergency.png"
    }
}
