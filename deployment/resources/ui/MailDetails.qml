/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
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
        contentTextField.text = removeHtml(Mail.text);
        privates.isInitialized = true;
    }

    Component.onDestruction: {
    }

    QtObject {
        id: privates

        property bool isInitialized: false;
        property int nameWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.6 : root.width * 0.4;
        property int textFieldHeight: root.height * 0.05
        property int textFieldMaxPixelSize: root.height * 0.04
        property int textFieldMinPixelSize: root.height * 0.02
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
            anchors.margins: 5

            Text {
                id: titleText
                font.pixelSize: parent.height * 0.2
                text: Mail.subject
                width: parent.width
                anchors.top: parent.top
                anchors.leftMargin: 5
                elide: Text.ElideRight
            }

            Text {
                id: contactNameText
                font.pixelSize: parent.height * 0.15
                text: Mail.from
                width: parent.width * 0.5
                anchors.top: titleText.bottom
                anchors.topMargin: 5
                anchors.leftMargin: 5
                elide: Text.ElideRight
            }

            Text {
                id: dateTimeText
                font.pixelSize: parent.height * 0.15
                text: Mail.date + " at " + Mail.time
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: root.height * 0.02

            }
        }
    }

    Rectangle {
        id: contentTextField;
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: headerRectangle.bottom
        anchors.bottom: bottomBar.top
        anchors.margins: 5

        Flickable {
            id: flickArea
            anchors.fill: parent
            contentWidth: textContent.width; contentHeight: textContent.height
            flickableDirection: Flickable.VerticalFlick
            clip: true

            Text {
                id: textContent
                text: Mail.text
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5
                wrapMode: Text.WordWrap
            }
        }

        //        property int lineChars: (width / (font.pixelSize * 0.8));
        //        property bool firstLine: true;
        //        property int charCount: 0
        //        font.pixelSize: Math.max(Math.min(height / lineCount, privates.textFieldMaxPixelSize),
        //                                 privates.textFieldMinPixelSize);
        //        onTextChanged: {
        //            charCount++;
        //            console.log(charCount);
        //            if (charCount > lineChars) {
        //                charCount = -1;
        //                console.log("Append");
        //                append("");
        //            }
        //        }
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

    function removeHtml(text) {

        var newtext = text;
        if(newtext.toLowerCase().search(/<(\s[^<>])*body[^<>]*>/i) >= 0) {
            newtext = newtext.substring(text.toLowerCase().search(/<(\s[^<>])*body[^<>]*>/i), text.toLowerCase().search(/<(\s[^<>])*\/body(\s[^<>])*>/i));
        } else {
        }

        newtext = newtext.replace(/&nbsp;/ig, " ");
        newtext = newtext.replace(/(\r\n|\n|\r)/g," ");
        newtext = newtext.replace(/<[^<>]*>/igm, "");
        newtext = newtext.replace(/<(\s[^<>])*\/[^<>]*>/igm, "");
        newtext = newtext.replace(/^\s*/igm, "");
        if(newtext === text)
            return text;
        else
            return removeHtml(newtext);
    }
}

