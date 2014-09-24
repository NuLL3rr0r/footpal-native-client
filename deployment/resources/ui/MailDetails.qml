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

        property int nameWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.6 : root.width * 0.4
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
            text: qsTr("MAIL_DETAILS") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    Rectangle {
        id: headerRectangle;
        gradient: Gradient {
            GradientStop { color: "#66ffffff"; position: 0.0 }
            GradientStop { color: "#aaffffff"; position: 0.4 }
            GradientStop { color: "#aaffffff"; position: 0.6 }
            GradientStop { color: "#66ffffff"; position: 1.0 }
        }
        border.color: "white";
        border.width: 1;
        radius: root.height * 0.015;
        height: root.height * 0.2;
        anchors.top: topBar.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: root.height * 0.01;

        Item {
            anchors.fill: parent;
            anchors.margins: root.height * 0.02;

            Image {
                id: contactPicture
                height: parent.height
                width: height
                source: "qrc:///img/ic_contact.png"
            }

            Text {
                id: titleText
                font.pixelSize: parent.height * 0.4
                text: "List of Devices"
                wrapMode: Text.WordWrap
                width: parent.width
                anchors.top: parent.top
                anchors.left: contactPicture.right
                anchors.leftMargin: root.height * 0.02
            }

            Text {
                id: contactNameText
                font.pixelSize: parent.height * 0.25
                text: "Majid Sadeghi Alavijeh"
                anchors.bottom: contactPicture.bottom
                anchors.left: contactPicture.right
                anchors.leftMargin: root.height * 0.02
            }

            Text {
                id: dateTimeText
                font.pixelSize: parent.height * 0.2
                text: "2014/09/23 at 09:54"
                anchors.bottom: contactPicture.bottom
                anchors.right: parent.right
                anchors.leftMargin: root.height * 0.02
            }
        }
    }

    Rectangle {
        id: contentRectangle;
        gradient: Gradient {
            GradientStop { color: "#66ffffff"; position: 0.0 }
            GradientStop { color: "#aaffffff"; position: 0.4 }
            GradientStop { color: "#aaffffff"; position: 0.6 }
            GradientStop { color: "#66ffffff"; position: 1.0 }
        }
        border.color: "white";
        border.width: 1;
        radius: root.height * 0.015;
        anchors.top: headerRectangle.bottom;
        anchors.bottom: bottomBar.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: root.height * 0.01;

        Item {
            anchors.fill: parent;
            anchors.margins: root.height * 0.02;

            Text {
                id: contentText
                width: parent.width
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
                font.pixelSize: Math.floor((parent.height / (lineCount + 1)) * 0.75);
                text: "Hi! I've assembled a list of the devices we need to develop the project further. Please check it out and share " +
                      "your thoughts with me.\n\nSincerely,\nMajid";
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
                defaultImage: "qrc:///img/btn_bar_reply.png"
                pressedImage: "qrc:///img/btn_bar_reply_pressed.png"
                onSignal_clicked: {
                    pageLoader.setSource("qrc:///ui/ComposeMail.qml");
                }
            }

            ExtButton {
                id: buttonForward
                height: parent.height;
                width: height;
                anchors.left: buttonReply.right
                anchors.leftMargin: 5
                defaultImage: "qrc:///img/btn_bar_forward.png"
                pressedImage: "qrc:///img/btn_bar_forward_pressed.png"
                onSignal_clicked: {
                    pageLoader.setSource("qrc:///ui/ComposeMail.qml");
                }
            }
        }
    }
}

