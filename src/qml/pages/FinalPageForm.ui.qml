import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import cz.nkgroup 1.0

Item {
    width: 600
    height: 380

    property alias group2: group2
    property alias confirmBtn: confirmBtn
    property alias progg: busyIndicator

    ButtonGroup {
        buttons: group1.children
    }

    GroupBox {
        id: group1
        x: 46
        y: 20
        width: 338
        height: 79
        visible: true
        title: qsTr("Nádrž 1:")

        Button {
            id: tank1OKbtn
            x: 26
            y: -7
            text: qsTr("OK")
            checked: false
            checkable: true
        }

        Button {
            id: tank1NOKBtn
            x: 180
            y: -7
            text: qsTr("NOK")
            autoExclusive: false
            highlighted: false
            spacing: -1
            font.family: "Arial"
            font.pointSize: 12
            checked: true
            checkable: true
        }
    }

    GroupBox {
        id: group2
        x: 46
        y: 105
        width: 338
        height: 80
        title: qsTr("Nádrž 2: ")

        Button {
            id: tank2OKBtn
            x: 29
            y: -9
            text: qsTr("OK")
            checkable: true
        }

        Button {
            id: tank2NOKBtn
            x: 180
            y: -9
            text: qsTr("NOK")
            checked: true
            checkable: true
        }
    }

    Connections {
        target: tank1OKbtn
        onClicked: {
            if (!tank1OKbtn.checked) {
                tank1NOKBtn.checked = true
                tank1OKbtn.checked = false
            } else {
                tank1NOKBtn.checked = false
            }
        }
    }

    Connections {
        target: tank1NOKBtn
        onClicked: {
            if (!tank1NOKBtn.checked) {
                tank1OKbtn.checked = true
                tank1NOKBtn.checked = false
            } else {
                tank1OKbtn.checked = false
            }
        }
    }

    Connections {
        target: tank2OKBtn
        onClicked: {
            if (!tank2OKBtn.checked) {
                tank2NOKBtn.checked = true
                tank2OKBtn.checked = false
            } else {
                tank2NOKBtn.checked = false
            }
        }
    }

    Connections {
        target: tank2NOKBtn
        onClicked: {
            if (!tank2NOKBtn.checked) {
                tank2OKBtn.checked = true
                tank2NOKBtn.checked = false
            } else {
                tank2OKBtn.checked = false
            }
        }
    }

    Button {
        id: confirmBtn
        x: 284
        y: 191
        text: qsTr("Potvrdit")
    }

    Connections {
        target: confirmBtn
        onClicked: {
            busyIndicator.visible = true
            //qmlConnection.tankOneStatus(tank1OKbtn.checked)
            //qmlConnection.tankTwoStatus(tank2OKBtn.checked)
            qmlConnection.confirmEnd(tank1OKbtn.checked, tank2OKBtn.checked)
        }
    }

    BusyIndicator {
        id: busyIndicator
        x: 89
        y: 191
        enabled: false
    }
}
