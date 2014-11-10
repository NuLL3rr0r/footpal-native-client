/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 * @author  Mohammad S. Babaei <info@babaei.net>
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

        property variant targetClient: ({});
        property bool isInitialized: false;
        property variant initializedTo: ({});
        property int maxSummary: 50;
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.15 : 100;
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5;
        property int loadIndex: 0;
        property int loadSize: 1;
        property int mailCount: 0;
        property bool isInDetail: false;
        property string navigatoTo: ""; // To prevent race-condition with message fetching, this is used to schedule disconnection after fetching.

        property variant idToIndex: ({"\"1584243002.894505385.1415110596157.JavaMail.root@sjmas02.marketo.org": 0});
        property string mailJSON: "{                                                                                                     "+
                                  "    \"data\":                                                                                         "+
                                  "    [                                                                                                 "+
                                  "        {                                                                                             "+
                                  "            \"message_id\": \"1584243002.894505385.1415110596157.JavaMail.root@sjmas02.marketo.org\", "+
                                  "            \"from\":                                                                                 "+
                                  "            [                                                                                         "+
                                  "                {                                                                                         "+
                                  "                     \"email\": \"sales@lisasoft.com\",                                                    "+
                                  "                     \"name\": \"JulietLin\"                                                               "+
                                  "                }                                                                                        "+
                                  "            ]," +
                                  "            \"subject\": \"How to reduce your IT Spend with Postgres\",                               "+
                                  "            \"date\": \"2:4:11:2014:8:16:36:+0330\",                                                       "+
                                  "            \"to\":                                                                                   "+
                                  "            [                                                                                         "+
                                  "                {                                                                                     "+
                                  "                    \"name\": \"\",                                                                   "+
                                  "                    \"email\": \"arcananemous@gmail.com\",                                            "+
                                  "                    \"type\": \"TO\"                                                                  "+
                                  "                }                                                                                     "+
                                  "            ],                                                                                        "+
                                  "            \"headers\":                                                                              "+
                                  "            {                                                                                         "+
                                  "                \"Reply-To\": \"edbmarketing@enterprisedb.com\"                                       "+
                                  "            },                                                                                        "+
                                  "            \"text\": \"<HTML><BODY test=\\\"2323232 fff\\\">FFFJFJFJJF</BODY></HTML>\""+
                                  "        } "+
                                  "    ]     "+
                                  "}         ";

    }

    Component.onCompleted: {
        if(Mail.isMailboxCached) {
            Mail.isMailboxCached = false;
            privates.isInitialized = Mail.cache_isInitialzed;
            privates.initializedTo = Mail.cache_initializedTo;
            privates.maxSummary    = Mail.cache_maxSummary;
            privates.itemHeight    = Mail.cache_itemHeight;
            privates.itemSpacing   = Mail.cache_itemSpacing;
            privates.loadSize      = Mail.cache_loadSize;
            privates.loadIndex     = Mail.cache_loadIndex;
            privates.mailCount     = Mail.cache_mailCount;
            privates.idToIndex     = Mail.cache_idToIndex;
            privates.mailJSON      = Mail.cache_mailJSON;
            privates.targetClient  = Mail.cache_targetClient;

            console.log("CACHE UNLOADED");

            if(JSON.stringify(privates.initializedTo) !== JSON.stringify(Mail.currentMailAccount)) {
                privates.loadIndex = 0;
                privates.isInitialized = true;
                privates.initializedTo = Mail.currentMailAccount;
                privates.idToIndex = {};
                privates.mailJSON = "{\"data\":[]}";
                console.log("loading from " + privates.loadIndex + ", " + privates.loadSize);
                privates.targetClient.workerThread_disconnect();
                loadInbox(privates.loadIndex, privates.loadSize);
            } else {
                privates.targetClient.workerThread_autoFetchEnable(
                            privates.loadIndex,
                            privates.loadSize,
                            privates.mailCount
                            );
            }

        } else {
            if (Mail.currentMailAccount.protocol === "imap" || Mail.currentMailAccount.protocol === "1") {
                privates.targetClient = ImapClient;
            } else {    //  protocol is pop3
                privates.targetClient = Pop3Client;
            }
            privates.loadIndex = 0;
            privates.isInitialized = true;
            privates.initializedTo = Mail.currentMailAccount;
            privates.idToIndex = {};
            privates.mailJSON = "{\"data\":[]}";

            if(!privates.targetClient.workerThread_isEnabled()) {
                privates.targetClient.workerThread_enable();
            }

            console.log("loading from " + privates.loadIndex + ", " + privates.loadSize);
            loadInbox(privates.loadIndex, privates.loadSize);
            privates.isInitialized = true;
        }


        privates.targetClient.onSignal_ConnectCompleted.connect(onConnectCallback);
        privates.targetClient.onSignal_DisconnectCompleted.connect(onDisconnectCallback);
        privates.targetClient.onSignal_GetMessageCountCompleted.connect(onGetMessageCountCallback);
        privates.targetClient.onSignal_FetchAsJsonCompleted.connect(onFetchAsJsonCallback);
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
        json: privates.mailJSON;
        query: "$.data[*]";
    }

    ListView {
        anchors.top: topBar.bottom
        anchors.bottom: bottomBar.top
        anchors.bottomMargin: 5
        width: parent.width
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
                    source: "qrc:///img/ic_email.png"
                }
                Text {
                    id: titleText
                    anchors.left: contactPicture.right
                    anchors.right: dateText.left
                    anchors.top: parent.top
                    anchors.margins: 5
                    text: model.subject
                    font.pixelSize: parent.height * 0.2

                    onTextChanged: {
                        var maxChars = ((parent.width * 0.5) / font.pixelSize) - 4;
                        console.log("Max Chars: " + maxChars);
                        console.log("Text Length: " + text.length);
                        if (text.length > maxChars) {
                            text = text.substring(0, maxChars) + "...";
                            console.log("Text New Length: " + text.length);
                        }
                    }
                }
                Text {
                    id: senderText
                    anchors.left: contactPicture.right
                    anchors.top: titleText.bottom
                    anchors.margins: 5
                    text: extractFrom(model);
                    font.pixelSize: parent.height * 0.15
                }
                Text {
                    id: summaryText
                    anchors.left: contactPicture.right
                    anchors.top: senderText.bottom
                    anchors.margins: 5
                    text: extractSummary(model.text)
                    font.pixelSize: parent.height * 0.15
                }
                Text {
                    id: dateText
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 5
                    text: extractDate(model.date)
                    font.pixelSize: parent.height * 0.15
                }
                Text {
                    id: timeText
                    anchors.right: parent.right
                    anchors.top: dateText.bottom
                    anchors.margins: 5
                    text: extractTime(model.date)
                    font.pixelSize: parent.height * 0.15
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(privates.navigatoTo === "") {

                            privates.targetClient.workerThread_autoFetchDisable();

                            Mail.isMailboxCached     = true;
                            Mail.cache_isInitialzed  = privates.isInitialized;
                            Mail.cache_initializedTo = privates.initializedTo;
                            Mail.cache_maxSummary    = privates.maxSummary;
                            Mail.cache_itemHeight    = privates.itemHeight;
                            Mail.cache_itemSpacing   = privates.itemSpacing;
                            Mail.cache_loadIndex     = privates.loadIndex;
                            Mail.cache_loadSize      = privates.loadSize;
                            Mail.cache_mailCount     = privates.mailCount;
                            Mail.cache_idToIndex     = privates.idToIndex;
                            Mail.cache_mailJSON      = privates.mailJSON;
                            Mail.cache_targetClient  = privates.targetClient;


                            Mail.selectedMessageId = model.message_id;
                            Mail.text = model.text;
                            Mail.from = extractFrom(model);
                            Mail.date = extractDate(model.date);
                            Mail.time = extractTime(model.date);
                            Mail.subject = model.subject;

                            privates.targetClient.onSignal_ConnectCompleted.disconnect(onConnectCallback);
                            privates.targetClient.onSignal_DisconnectCompleted.disconnect(onDisconnectCallback);
                            privates.targetClient.onSignal_GetMessageCountCompleted.disconnect(onGetMessageCountCallback);
                            privates.targetClient.onSignal_FetchAsJsonCompleted.disconnect(onFetchAsJsonCallback);

                            privates.navigatoTo = "qrc:///ui/MailDetails.qml";
                            privates.isInDetail = true;
                            pageLoader.setSource("qrc:///ui/MailDetails.qml");
                        }
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

    function extractDate(datetime) {
        var v = datetime.split(":");
        return v[3] + "/" + v[1] + "/" + v[2];
    }

    function extractTime(datetime) {
        var v = datetime.split(":");
        return v[4] + ":" + v[5] + ":" + v[6];
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

    function extractSummary(body) {
        body = removeHtml(body);
        var toCopy = privates.maxSummary;
        if(toCopy > body.length) {
            toCopy = body.length;
        }
        var result =  body.substr(0, toCopy);
        return result;
    }

    function extractFrom(model) {

        var all = JSON.parse(privates.mailJSON);
        for(var i = 0; i < all.data.length; ++i) {
            if(all.data[i].message_id === model.message_id) {
                var from = all.data[i].from[0];
                var result = "From: " + from.name + " <" + from.email + ">";
                return result;
            }
        }
        return "";
    }

    function loadInbox(start, size) {
        privates.loadIndex = start;
        privates.loadSize = size;

        var account = Mail.currentMailAccount;
        privates.targetClient.SetHost(account.readHost);
        privates.targetClient.setPort(account.readPort);
        privates.targetClient.setSecurityType(Mail.strToSecurityType(account.readSecurity));
        privates.targetClient.SetUsername(account.username);
        privates.targetClient.SetPassword(account.password);

        privates.targetClient.workerThread_connect();
    }

    function onConnectCallback(succeeded)
    {
        if (succeeded) {
            console.log("Connected successfully. Fetching now...");
            privates.targetClient.workerThread_getMessageCount();
        } else {
        }
    }

    function onDisconnectCallback()
    {
        if(privates === null) {
            return;
        }
        console.log("Disconnected successfully.");
        if(privates.doReconnect) {
            loadInbox(0, privates.loadSize);
        }
    }

    function onGetMessageCountCallback(count) {
        console.log("GetMessageCount successfuly : " + count);
        privates.mailCount = count;
        privates.targetClient.workerThread_autoFetchEnable(
                    privates.loadIndex, privates.loadSize,
                    privates.mailCount
                    );
    }

    function onFetchAsJsonCallback(msg) {
        console.log("FetchAsJson successfuly!");

        if(msg.indexOf("[") < 0) {
            //error had occured! no data was fetched!!
            //repeat last fetched

            //privates.targetClient.fetchAsJson(privates.loadIndex, Math.min(privates.mailCount - privates.loadIndex, privates.loadSize));

            //end execution, no need to advance.
            return;
        }

        console.log(privates.loadIndex);

        var inner = msg.substring(msg.indexOf("[") + 1, msg.lastIndexOf("]"));
        var msgObject = JSON.parse(msg).data;
        for(var i = 0; i < msgObject.length; ++i) {
            privates.idToIndex[msgObject[i].message_id] = privates.loadIndex + i;
        }
        var toInsert = privates.mailJSON.lastIndexOf("]");
        privates.mailJSON = privates.mailJSON.substr(0, toInsert) +
                (privates.loadIndex > 0 ? "," : "") + inner +
                privates.mailJSON.substr(toInsert, privates.mailJSON.length - toInsert);

        privates.loadIndex += privates.loadSize;

        if(privates.loadIndex > 10) {
            privates.targetClient.workerThread_autoFetchDisable();
        }
    }
}

