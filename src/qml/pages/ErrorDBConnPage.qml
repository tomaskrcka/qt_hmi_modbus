import QtQuick 2.4
import cz.nkgroup 1.0


ErrorDBConnPageForm {
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

    QmlConnection {
        id: qmlConnection
    }

    Component.onCompleted: {
        logoutTBn.visible = false
    }

    restartBtn {
        onPressed: {
            qmlConnection.restartApp()
        }
    }

}
