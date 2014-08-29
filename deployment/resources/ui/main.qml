import QtQuick 2.3;
import QtQuick.Controls 1.1;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;
import "custom";

ApplicationWindow {
    visible: true;
    width: 640;
    height: 480;
    title: qsTr("APP_TITLE");
    color: "#203070";

    property Component buttonStyle: ButtonStyle {
        id: buttonStyle;
        background: Rectangle {
            implicitHeight: 20;
            implicitWidth: 100;
            color: control.pressed ? "darkGray" : "lightGray";
            antialiasing: true;
            border.color: "gray";
            radius: height / 4;
        }
    }

    property Component textFieldStyle: TextFieldStyle {
        background: Rectangle {
            implicitWidth: 100;
            implicitHeight: 40;
            color: "#f0f0f0";
            antialiasing: true;
            border.color: "gray";
            radius: height / 4;
        }
    }

    Component.onCompleted: {
        pageLoader.setSource("Splash.qml");
    }

    Loader {
        id: pageLoader;
        anchors.fill: parent;
        anchors.centerIn: parent;
    }

    function showToast(text, duration) {
        toast.timoutInterval = duration;
        toast.notificationText = "";
        toast.notificationText = text;
    }

    Toast {
        id: toast;
        anchors.fill: parent;
        anchors.centerIn: parent;
        fontPath: FontPath;
    }
}
