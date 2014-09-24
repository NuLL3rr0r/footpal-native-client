/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;
import "custom"


Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";

    Component.onCompleted: {
    }

    Component.onDestruction: {
    }

    QtObject {
        id: privates

        property int textFieldHeight: root.height * 0.05
        property int textFieldPixelSize: root.height * 0.025
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
                pageLoader.setSource("qrc:///ui/Mailbox.qml")
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("COMPOSE_MAIL") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    Column {
        id: headerColumn
        anchors.top: topBar.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: 5;
        spacing: 5;

        TextField {
            id: toTextField;
            style: textFieldStyle;
            width: parent.width;
            height: privates.textFieldHeight;
            font.pixelSize: privates.textFieldPixelSize
            placeholderText: qsTr("TO") + UiEngine.EmptyLangString;
            focus: true;
        }

        TextField {
            id: subjectTextField;
            style: textFieldStyle;
            width: parent.width;
            height: privates.textFieldHeight;
            font.pixelSize: privates.textFieldPixelSize
            placeholderText: qsTr("SUBJECT") + UiEngine.EmptyLangString;
        }

        TextField {
            id: ccTextField;
            style: textFieldStyle;
            width: parent.width;
            height: privates.textFieldHeight;
            font.pixelSize: privates.textFieldPixelSize
            placeholderText: qsTr("CC") + UiEngine.EmptyLangString;
        }

        TextField {
            id: bccTextField;
            style: textFieldStyle;
            width: parent.width;
            height: privates.textFieldHeight;
            font.pixelSize: privates.textFieldPixelSize
            placeholderText: qsTr("BCC") + UiEngine.EmptyLangString;
        }

        Text {
            text: qsTr("CONTENT") + UiEngine.EmptyLangString + ":";
            font.pixelSize: privates.textFieldPixelSize;
            color: "white"
        }
    }

    TextArea {
        id: contentTextField;
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: headerColumn.bottom
        anchors.bottom: bottomBar.top
        anchors.margins: 5

        property int lineChars: (width / font.pixelSize);
        property bool firstLine: true;
        property int charCount: 0
        font.pixelSize: Math.min(height / lineCount, privates.textFieldPixelSize);
        onTextChanged: {
//            var len = text.length
//            var a = lineCount > 2 ? 2 : 0;
//            var b = (firstLine && lineCount > 1) ? 2 : 0;
//            console.log(a + " " + b + " " + lineCount);
            charCount++;
            console.log(charCount);
            if (charCount > lineChars) {
                charCount = -1;
                console.log("Append");
                append("");
            }
        }
    }

    Bar {
        id: bottomBar
        anchors.bottom: parent.bottom

        Item {
            anchors.fill: parent
            anchors.margins: 5

            ExtButton {
                id: buttonReply
                height: parent.height;
                width: height;
                anchors.left: parent.left
                defaultImage: "qrc:///img/btn_bar_send.png"
                pressedImage: "qrc:///img/btn_bar_send_pressed.png"
                onSignal_clicked: {
                    //  TODO: Send the email
                }
            }
        }
    }
}

