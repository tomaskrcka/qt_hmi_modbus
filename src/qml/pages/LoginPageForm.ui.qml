import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: item1
    width: 600
    height: 380
    focus: true

    property alias incorrectUserLbl: incorrectUserLbl
    property alias loginBtn: loginBtn

    Label {
        id: labelMagic
        x: 213
        y: 94
        width: 175
        height: 32
        text: qsTr("Přihlášení uživatele")
        font.pointSize: 22
        font.family: "Arial"
        renderType: Text.NativeRendering
        fontSizeMode: Text.HorizontalFit
    }

    Switch {
        id: switch1
        x: 223
        y: 275
        text: qsTr("Zadání ručně")
        visible: false
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 65
    }

    TextField {
        id: textField
        x: 140
        y: 229
        width: 321
        height: 40
        text: qsTr("")
        visible: false
    }

    Connections {
        target: switch1
        onClicked: {
            userNumLbl.visible = switch1.checked
            textField.visible = switch1.checked
            textField.text = ""
        }
    }

    Label {
        id: userNumLbl
        x: 44
        y: 209
        text: qsTr("Zadejte už. číslo")
        visible: false
    }

    Label {
        id: label1
        x: 163
        y: 132
        width: 275
        height: 34
        text: qsTr("Načtěte své uživatelské číslo")
        font.weight: Font.Black
        textFormat: Text.AutoText
        font.family: "Verdana"
        font.pointSize: 22
        fontSizeMode: Text.Fit
        renderType: Text.QtRendering
    }

    Button {
        id: magicBtn
        x: 303
        y: 113
        text: qsTr("R")
        opacity: 0
        clip: false
        visible: true
    }

    Connections {
        target: magicBtn
        onClicked: stack.push(Qt.resolvedUrl("ErrorDBConnPage.qml"))
    }

    Label {
        id: incorrectUserLbl
        x: 187
        y: 174
        width: 225
        height: 32
        color: "#8c2323"
        text: qsTr("Neplatné uživatelské číslo")
        visible: false
        font.family: "Arial"
        renderType: Text.NativeRendering
        font.pointSize: 24
        fontSizeMode: Text.HorizontalFit
    }

    Button {
        id: loginBtn
        x: 90
        y: 285
        text: qsTr("Debug login")
        visible: false
    }
}
