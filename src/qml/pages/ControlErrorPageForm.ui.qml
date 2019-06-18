import QtQuick 2.4

Item {
    width: 600
    height: 380

    Text {
        id: text1
        x: 61
        y: 179
        color: "#da1c1c"
        text: qsTr("Vzduchový válec není ve výchozí pozici")
        font.family: "Tahoma"
        font.pixelSize: 18
    }

    Text {
        id: text2
        x: 118
        y: 226
        text: "Nutno ručně nastavit do výchozí pozice"
        font.pixelSize: 12
    }

    Image {
        id: image
        x: 61
        y: 51
        width: 188
        height: 113
        source: "../resources/error-large.jpg"
    }
}
