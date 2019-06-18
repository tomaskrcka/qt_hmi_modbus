import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: item1
    width: 600
    height: 380
    property alias continueBtn: continueBtn
    property alias tryAgainBtn: tryAgainBtn
    focus: true

    property alias incorrectUserLbl: incorrectUserLbl

    Label {
        id: mainLbl
        x: 177
        y: 94
        width: 305
        height: 32
        color: "#2b2626"
        text: qsTr("Problém s ukládáním snímku")
        font.bold: true
        font.pointSize: 22
        font.family: "Arial"
        renderType: Text.NativeRendering
        fontSizeMode: Text.HorizontalFit
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

    Label {
        id: label1
        x: 163
        y: 132
        width: 275
        height: 34
        text: qsTr("Načtěte své uživatelské číslo pro pokračování")
        font.bold: false
        font.weight: Font.Black
        textFormat: Text.AutoText
        font.family: "Verdana"
        font.pointSize: 22
        fontSizeMode: Text.Fit
        renderType: Text.QtRendering
    }

    Label {
        id: incorrectUserLbl
        x: 187
        y: 174
        width: 225
        height: 32
        color: "#8c2323"
        text: qsTr("Neplatné uživatelské číslo nebo nízká úroveň")
        visible: false
        font.family: "Arial"
        renderType: Text.NativeRendering
        font.pointSize: 24
        fontSizeMode: Text.HorizontalFit
    }

    Button {
        id: continueBtn
        x: 163
        y: 289
        text: qsTr("OK, pokračovat")
        spacing: 1
        font.capitalization: Font.MixedCase
        visible: false
    }

    Button {
        id: tryAgainBtn
        x: 302
        y: 289
        text: qsTr("Uložit znovu")
        visible: false
        font.capitalization: Font.MixedCase
        spacing: 1
    }
}
