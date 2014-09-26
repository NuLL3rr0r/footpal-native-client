/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;
import "custom"
import "scripts/ws.js" as WS


Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";
    state: "public"

    Component.onCompleted: {
    }

    Component.onDestruction: {
    }

    QtObject {
        id: privates
        property int firstNameWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.2 : root.width * 0.3
        property int lastNameWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.3 : root.width * 0.4
        property int textFieldHeight: root.height * 0.075
    }

    Bar {
        id: topBar

        ExtButton {
            height: parent.height * 0.8;
            width: height;
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: (parent.height - height) / 2;
            defaultImage: "qrc:///img/btn_bar_return.png"
            pressedImage: "qrc:///img/btn_bar_return_pressed.png"
            onSignal_clicked: {
                pageLoader.setSource("qrc:///ui/Home.qml")
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("MY_PROFILE") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    Image {
        id: profilePicture
        height: root.height * 0.2
        width: height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topBar.bottom
        anchors.margins: root.height * 0.05
        source: "qrc:///img/ic_contact.png"
    }

    Row {
        id: nameRow
        anchors.top: profilePicture.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: root.height * 0.05
        spacing: root.width * 0.025

        TextField {
            id: firstNameText
            width: privates.firstNameWidth
            height: privates.textFieldHeight
            font.pixelSize: height / 2
            text: "Morteza"
            style: textFieldStyle
        }

        TextField {
            id: lastNameText
            width: privates.lastNameWidth
            height: privates.textFieldHeight
            font.pixelSize: height / 2
            text: "Sabetraftar"
            style: textFieldStyle
        }
    }

    TextField {
        id: emailText
        width: nameRow.width
        height: privates.textFieldHeight
        font.pixelSize: height / 2
        text: "morteza.sabetraftar@gmail.com"
        anchors.top: nameRow.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: root.height * 0.05
        style: textFieldStyle
    }
}

