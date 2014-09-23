/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;
import "custom"
import "utils"

Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";

    QtObject {
        id: privates

        property bool isInitialized: false;
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.2 : 100;
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5;
        property string testJSON: "{ \"inbox\":" +
                                  "{ \"email\": [" +
                                  "{ \"picture\": \"qrc:///img/ic_contact.png\", \"sender\": \"Majid Sadeghi Alavijeh\"," +
                                  "\"title\": \"List of Devices\", \"summary\": \"Hi! I've assembled a\"," +
                                  "\"date\": \"2014/09/22\", \"time\": \"14:27\" }," +
                                  "{ \"picture\": \"qrc:///img/ic_contact.png\", \"sender\": \"Mohammad Sadegh Babaei\"," +
                                  "\"title\": \"Today's Meeting\", \"summary\": \"Hi! Any thoughts on \"," +
                                  "\"date\": \"2014/09/23\", \"time\": \"09:54\" } ] } }";
    }

    Component.onCompleted: {
        privates.isInitialized = true;
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
                pageLoader.setSource("qrc:///ui/MailServers.qml")
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("INBOX") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    JSONListModel {
        id: jsonModel;
        json: privates.testJSON;
        query: "$.inbox.email[*]";
    }

    ListView {
        anchors.top: topBar.bottom
        anchors.bottom: bottomBar.top
        anchors.bottomMargin: 5
        width: parent.width
        spacing: privates.itemSpacing
        model: jsonModel.model;
        delegate: Component {
            Rectangle {
                gradient: Gradient {
                    GradientStop { color: "#66ffffff"; position: 0.0 }
                    GradientStop { color: "#aaffffff"; position: 0.4 }
                    GradientStop { color: "#aaffffff"; position: 0.6 }
                    GradientStop { color: "#66ffffff"; position: 1.0 }
                }
                width: parent.width
                height: privates.itemHeight

                Image {
                    id: contactPicture
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 5
                    width: height
                    source: model.picture
                }
                Text {
                    id: titleText
                    anchors.left: contactPicture.right
                    anchors.top: parent.top
                    anchors.margins: 5
                    text: model.title
                    font.pixelSize: parent.height * 0.2
                }
                Text {
                    id: senderText
                    anchors.left: contactPicture.right
                    anchors.top: titleText.bottom
                    anchors.margins: 5
                    text: "From: " + model.sender
                    font.pixelSize: parent.height * 0.15
                }
                Text {
                    id: summaryText
                    anchors.left: contactPicture.right
                    anchors.top: senderText.bottom
                    anchors.margins: 5
                    text: model.summary + "..."
                    font.pixelSize: parent.height * 0.15
                }
                Text {
                    id: dateText
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 5
                    text: model.date
                    font.pixelSize: parent.height * 0.15
                }
                Text {
                    id: timeText
                    anchors.right: parent.right
                    anchors.top: dateText.bottom
                    anchors.margins: 5
                    text: model.time
                    font.pixelSize: parent.height * 0.15
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // TODO: remeber contact's ID in order to retrieve it's information in the ContactProfile page

//                        pageLoader.setSource("qrc:///ui/ContactProfile.qml")
                    }
                }
            }
        }
    }

    Bar {
        id: bottomBar
        anchors.bottom: parent.bottom
        z: 2

        Row {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            ExtButton {
                id: buttonNewMailServer
                height: parent.height * 0.8;
                width: height;
                anchors.verticalCenter: parent.verticalCenter
                defaultImage: "qrc:///img/btn_bar_new_mail.png"
                pressedImage: "qrc:///img/btn_bar_new_mail_pressed.png"
                onSignal_clicked: {

                }
            }
        }
    }
}

