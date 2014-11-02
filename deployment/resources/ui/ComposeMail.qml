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
        SmtpClient.onSignal_ConnectCompleted.connect(onConnectCallback);
        SmtpClient.onSignal_DisconnectCompleted.connect(onDisconnectCallback);
        SmtpClient.onSignal_SendCompleted.connect(onSendCallback);
    }

    Component.onDestruction: {
        SmtpClient.onSignal_ConnectCompleted.disconnect(onConnectCallback);
        SmtpClient.onSignal_DisconnectCompleted.disconnect(onDisconnectCallback);
        SmtpClient.onSignal_SendCompleted.disconnect(onSendCallback);
    }

    function onConnectCallback(succeeded)
    {
        if (succeeded) {
            console.log("Connected successfully. Sending now...");
            var message = createJsonMessage();
            SmtpClient.sendAsJson(message);
        } else {
            buttonSend.enabled = true;
        }
    }

    function onDisconnectCallback()
    {
        console.log("Disconnected successfully.");
        buttonSend.enabled = true;
    }

    function onSendCallback(succeeded)
    {
        if (succeeded) {
            console.log("Message successfully sent. Disconnecting...");
            SmtpClient.Disconnect();
        } else {
            buttonSend.enabled = true;
        }
    }

    QtObject {
        id: privates

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
            font.pixelSize: privates.textFieldMaxPixelSize
            placeholderText: qsTr("TO") + UiEngine.EmptyLangString;
            focus: true;
        }

        TextField {
            id: subjectTextField;
            style: textFieldStyle;
            width: parent.width;
            height: privates.textFieldHeight;
            font.pixelSize: privates.textFieldMaxPixelSize
            placeholderText: qsTr("SUBJECT") + UiEngine.EmptyLangString;
        }

        TextField {
            id: ccTextField;
            style: textFieldStyle;
            width: parent.width;
            height: privates.textFieldHeight;
            font.pixelSize: privates.textFieldMaxPixelSize
            placeholderText: qsTr("CC") + UiEngine.EmptyLangString;
        }

        TextField {
            id: bccTextField;
            style: textFieldStyle;
            width: parent.width;
            height: privates.textFieldHeight;
            font.pixelSize: privates.textFieldMaxPixelSize
            placeholderText: qsTr("BCC") + UiEngine.EmptyLangString;
        }

        Text {
            text: qsTr("CONTENT") + UiEngine.EmptyLangString + ":";
            font.pixelSize: privates.textFieldMaxPixelSize;
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

        property int lineChars: (width / (font.pixelSize * 0.8));
        property bool firstLine: true;
        property int charCount: 0
        font.pixelSize: Math.max(Math.min(height / lineCount, privates.textFieldMaxPixelSize),
                                 privates.textFieldMinPixelSize);
        onTextChanged: {
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
                id: buttonSend
                height: parent.height;
                width: height;
                anchors.left: parent.left
                defaultImage: "qrc:///img/btn_bar_send.png"
                pressedImage: "qrc:///img/btn_bar_send_pressed.png"
                onSignal_clicked: {
                    buttonSend.enabled = false;
                    SmtpClient.SetHost(Mail.currentMailAccount.sendHost);
                    SmtpClient.setPort(Mail.currentMailAccount.sendPort);
                    SmtpClient.setSecurityType(Mail.strToSecurityType(Mail.currentMailAccount.sendSecurity));
                    SmtpClient.SetUsername(Mail.currentMailAccount.username);
                    SmtpClient.SetPassword(Mail.currentMailAccount.password);
                    SmtpClient.Connect();
                }
            }
        }
    }

    function createJsonMessage() {
        var subject = subjectTextField.text;
        var text = contentTextField.text;
        var from = []
        from.push({ email: Mail.currentMailAccount.username, name: "" });
        var recipients = [];
        var tos = toTextField.text.split(",");
        var ccs = ccTextField.text.split(",");
        var bccs = bccTextField.text.split(",");

        tos.forEach(function(target){
            recipients.push({ email: target, type: "TO" });
        });
        if (ccTextField.text.length > 0) {
            ccs.forEach(function(target){
                recipients.push({ email: target, type: "CC" });
            });
        }
        if (bccTextField.text.length > 0) {
            bccs.forEach(function(target){
                recipients.push({ email: target, type: "BCC" });
            });
        }

        var result = { from: from, subject: subject, text: text, to: recipients }

        var res = JSON.stringify(result);
        console.log(res);
        return res;
    }
}

