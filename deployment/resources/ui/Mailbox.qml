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
import "scripts/settings.js" as Settings
import "scripts/mail.js" as Mail

Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: Settings.globalBgColor;

    QtObject {
        id: privates

        property bool isInitialized: false;
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.2 : 100;
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5;
        property int loadIndex: 0;
        property int loadSize: 10;
        property string testJSON: "{ \"data\": [" +
                                  "{ \"id\":\"1\", \"picture\": \"qrc:///img/ic_email.png\", \"sender\": \"Majid Sadeghi Alavijeh\"," +
                                  "\"title\": \"List of Devices\", \"summary\": \"Hi! I've assembled a\"," +
                                  "\"date\": \"2014/09/22\", \"time\": \"14:27\" }," +
                                  "{ \"id\":\"2\", \"picture\": \"qrc:///img/ic_email.png\", \"sender\": \"Majid Sadeghi Alavijeh\"," +
                                  "\"title\": \"List of Devices\", \"summary\": \"Hi! I've assembled a\"," +
                                  "\"date\": \"2014/09/22\", \"time\": \"14:27\" }," +
                                  "{ \"id\":\"3\", \"picture\": \"qrc:///img/ic_email.png\", \"sender\": \"Majid Sadeghi Alavijeh\"," +
                                  "\"title\": \"List of Devices\", \"summary\": \"Hi! I've assembled a\"," +
                                  "\"date\": \"2014/09/22\", \"time\": \"14:27\" }," +
                                  "{ \"id\":\"4\", \"picture\": \"qrc:///img/ic_email.png\", \"sender\": \"Majid Sadeghi Alavijeh\"," +
                                  "\"title\": \"List of Devices\", \"summary\": \"Hi! I've assembled a\"," +
                                  "\"date\": \"2014/09/22\", \"time\": \"14:27\" }," +
                                  "{ \"id\":\"5\", \"picture\": \"qrc:///img/ic_email.png\", \"sender\": \"Majid Sadeghi Alavijeh\"," +
                                  "\"title\": \"List of Devices\", \"summary\": \"Hi! I've assembled a\"," +
                                  "\"date\": \"2014/09/22\", \"time\": \"14:27\" }," +
                                  "{ \"id\":\"6\", \"picture\": \"qrc:///img/ic_email.png\", \"sender\": \"Majid Sadeghi Alavijeh\"," +
                                  "\"title\": \"List of Devices\", \"summary\": \"Hi! I've assembled a\"," +
                                  "\"date\": \"2014/09/22\", \"time\": \"14:27\" }," +
                                  "{ \"id\":\"7\", \"picture\": \"qrc:///img/ic_email.png\", \"sender\": \"Majid Sadeghi Alavijeh\"," +
                                  "\"title\": \"List of Devices\", \"summary\": \"Hi! I've assembled a\"," +
                                  "\"date\": \"2014/09/22\", \"time\": \"14:27\" }," +
                                  "{ \"id\":\"8\", \"picture\": \"qrc:///img/ic_email.png\", \"sender\": \"Majid Sadeghi Alavijeh\"," +
                                  "\"title\": \"List of Devices\", \"summary\": \"Hi! I've assembled a\"," +
                                  "\"date\": \"2014/09/22\", \"time\": \"14:27\" }," +
                                  "{ \"id\":\"9\", \"picture\": \"qrc:///img/ic_email.png\", \"sender\": \"Majid Sadeghi Alavijeh\"," +
                                  "\"title\": \"List of Devices\", \"summary\": \"Hi! I've assembled a\"," +
                                  "\"date\": \"2014/09/22\", \"time\": \"14:27\" }," +
                                  "{ \"id\":\"10\", \"picture\": \"qrc:///img/ic_email.png\", \"sender\": \"Mohammad Sadegh Babaei\"," +
                                  "\"title\": \"Today's Meeting\", \"summary\": \"Hi! Any thoughts on \"," +
                                  "\"date\": \"2014/09/23\", \"time\": \"09:54\" } ] }";
    }

    Component.onCompleted: {
        loadInbox(0, privates.loadSize);
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
        query: "$.data[*]";
    }

    ListView {
        anchors.top: topBar.bottom
        anchors.bottom: bottomBar.top
        anchors.bottomMargin: 5
        width: parent.width
//        spacing: privates.itemSpacing
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
                        Mail.selectedMessageId = model.id;
                        pageLoader.setSource("qrc:///ui/MailDetails.qml")
                    }
                }
            }
        }
        onMovementEnded: {
            if (atYEnd) {
                console.log("end of list!");
                loadInbox(privates.loadIndex, privates.loadSize);
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
                id: buttonComposeMail
                height: parent.height * 0.8;
                width: height;
                anchors.verticalCenter: parent.verticalCenter
                defaultImage: "qrc:///img/btn_bar_new_mail.png"
                pressedImage: "qrc:///img/btn_bar_new_mail_pressed.png"
                onSignal_clicked: {
                    pageLoader.setSource("qrc:///ui/ComposeMail.qml");
                }
            }

            ExtButton {
                id: buttonSettings
                height: parent.height * 0.8;
                width: height;
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                defaultImage: "qrc:///img/btn_bar_settings.png"
                pressedImage: "qrc:///img/btn_bar_settings_pressed.png"
                onSignal_clicked: {
                    Mail.newServerMode = "edit";
                    pageLoader.setSource("qrc:///ui/AddMailServer.qml");
                }
            }
        }
    }

    function loadInbox(start, size) {
        var messages;
        if (Mail.currentMailAccount.protocol === "imap") {
            messages = loadImap(start, size);
        } else {    //  protocol is pop3
            messages = loadPop3(start, size);
        }

        privates.loadIndex += privates.loadSize;

        var newJson = [];
        var i = 0;

        if (jsonModel.json.length > 0) {
            var oldJson = JSON.parse(jsonModel.json);
            for (i = 0; i < oldJson.data.length; i++) {
                newJson.push(oldJson.data[i]);
            }
        }

        var tempJson = JSON.parse(messages);
        for (i = 0; i < tempJson.data.length; i++) {
            newJson.push(tempJson.data[i]);
        }

        jsonModel.json = JSON.stringify(newJson);

        // TODO:    1. read jsonModel.json into Temp
        //          2. append new messages to Temp
        //          3. assign Temp to jsonModel.json
        //          4. set listview position to the item at privates.loadIndex
    }

    function loadImap(start, size) {
        var account = Mail.currentMailAccount;
        ImapClient.SetHost(account.readHost);
        ImapClient.setPort(account.readPort);
        ImapClient.setSecurityType(Mail.strToSecurityType(account.readSecurity));
        ImapClient.SetUsername(account.username);
        ImapClient.SetPassword(account.password);
        ImapClient.Connect();
        var count = ImapClient.getMessageCount();
        console.log(count);
        var messages = ImapClient.fetchAsJson(start, Math.min(count - start, size));
        console.log(messages);
        return messages;
    }

    function loadPop3(start, size) {
        var account = Mail.currentMailAccount;
        Pop3Client.SetHost(account.readHost);
        Pop3Client.setPort(account.readPort);
        Pop3Client.setSecurityType(Mail.strToSecurityType(account.readSecurity));
        Pop3Client.SetUsername(account.username);
        Pop3Client.SetPassword(account.password);
        Pop3Client.Connect();
        var count = Pop3Client.getMessageCount();
        console.log(count);
        var messages = Pop3Client.fetchAsJson(start, Math.min(count - start, size));
        console.log(messages);
        return messages;
    }
}

