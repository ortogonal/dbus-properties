import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

import Org.Appliance 1.0
import Org.Example 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Example {
        id: example
    }

    Appliance {
        id: appliance
    }

    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(example.red/255, example.green/255, example.blue/255, 1.0)
    }

    Text {
        id: tickLabel
        anchors.centerIn: parent
        text: example.tick + " - " + appliance.running

        MouseArea {
            anchors.fill: parent
            onClicked: appliance.start()
        }
    }

}
