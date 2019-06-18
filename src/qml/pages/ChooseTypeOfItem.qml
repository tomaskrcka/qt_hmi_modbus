import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import cz.nkgroup 1.0

ChooseTypeOfItemForm {
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

    MessageDialog {
        id: messageDialog
        property var msg: "";
        title: "Potvrzení zadání"
        text: "Povrdit zadání \n\n " + msg
        standardButtons: StandardButton.Yes | StandardButton.No
        modality: Qt.NonModal
        onYes: {
            qmlConnection.setTankProgram(tankIndex, msg);

            if (tankIndex < numOfItemsActual) {
                tankIndex++;
                stack.push(Qt.resolvedUrl("ItemSerialNumber.qml"))
            } else {
                stack.push(Qt.resolvedUrl("MainProcessPage.qml"))
            }
        }
        onNo: {
            stackLayout.forceActiveFocus()
        }
    }

    StackLayout {
        id: stackLayout
        width: parent.width
        focus: true
        Item {
            id: homeTab
            GridView {
                cellWidth: 120
                cellHeight: 70
                x: 20
                y: 70
                width: 700
                height: 300
                model: myModel

                delegate: Button {
                    width: 100
                    height: 50
                    property var tt: modelData.name
                    Text {
                        text: modelData.name
                        font.pointSize:12
                        clip: true
                        wrapMode: Text.WordWrap
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.fill: parent
                    }

                    onClicked: {
                        messageDialog.msg = tt
                        messageDialog.visible = true
                    }
                }
            }
        }
    }
}
