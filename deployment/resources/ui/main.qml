import QtQuick 2.2;
import QtQuick.Controls 1.1;
import "custom";

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    function showToast(text, duration) {
        toast.timoutInterval = duration;
        toast.notificationText = "";
        toast.notificationText = text;
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Button {
        text: qsTr("Say Hello World!");
        anchors.centerIn: parent;
        onClicked: {
            UiEngine.notify("Footpal", "Hello World!");
            UiEngine.showToast("Hello World!");
        }
    }

    Toast {
        id: toast;
        anchors.fill: parent;
        anchors.centerIn: parent;
        fontPath: FontPath;
    }
}
