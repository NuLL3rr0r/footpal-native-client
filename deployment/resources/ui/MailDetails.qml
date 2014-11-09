/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import QtWebKit 3.0;
import ScreenTypes 1.0;
import "custom"
import "scripts/settings.js" as Settings
import "scripts/mail.js" as Mail


Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: Settings.globalBgColor;

    Component.onCompleted: {
        // TODO: load the selected message into the page
        contentView.loadHtml(Mail.text);
        privates.isInitialized = true;
    }

    Component.onDestruction: {
    }

    QtObject {
        id: privates

        property bool isInitialized: false;
        property int nameWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.6 : root.width * 0.4;
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
                text: Mail.subject
                wrapMode: Text.WordWrap
                width: parent.width
                anchors.top: parent.top
                anchors.left: contactPicture.right
                anchors.leftMargin: root.height * 0.02
            }

            Text {
                id: contactNameText
                font.pixelSize: parent.height * 0.25
                text: Mail.from
                anchors.bottom: contactPicture.bottom
                anchors.left: contactPicture.right
                anchors.leftMargin: root.height * 0.02
            }

            Text {
                id: dateTimeText
                font.pixelSize: parent.height * 0.2
                text: Mail.date + " at " + Mail.time
                anchors.bottom: contactPicture.bottom
                anchors.right: parent.right
                anchors.leftMargin: root.height * 0.02
            }
        }
    }

    WebView {
        id: contentView;
        anchors.top: headerRectangle.bottom;
        anchors.bottom: bottomBar.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: root.height * 0.01;
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
                    Mail.composeMode = "reply";
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
                    Mail.composeMode = "forward";
                    pageLoader.setSource("qrc:///ui/ComposeMail.qml");
                }
            }
        }
    }
}

