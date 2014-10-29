/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import QtQuick.LocalStorage 2.0 as Sql;
import ScreenTypes 1.0;
import "custom"
import "scripts/settings.js" as Settings
import "scripts/mail.js" as Mail

Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: Settings.globalBgColor;
    state: "page1"

    QtObject {
        id: privates

        property int columnWidth: textFieldHeight * 8;// UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.8 : root.width * 0.5;
        property int textFieldHeight: root.height * 0.075;// columnWidth / 8;
        property int buttonWidth: columnWidth / 2;
    }

    Component.onCompleted: {
        if (Mail.newServerMode == "edit") {
            var account = Mail.currentMailAccount;
            if (account.protocol == "imap")
                radioIMAP.checked = true;
            else
                radioPOP3.checked = true;
            titleTextField.text = account.name;
            titleTextField.enabled = false;
            serverTextField.text = account.readHost;
            portTextField.text = account.readPort;
            comboBoxSecurity.currentIndex = Mail.strToSecurityComboIndex(account.readSecurity);
            smtpServerTextField.text = account.sendHost;
            smtpPortTextField.text = account.sendPort;
            comboBoxSMTPSecurity.currentIndex = Mail.strToSecurityComboIndex(account.sendSecurity);
            usernameTextField.text = account.username;
            passwordTextField.text = account.password;
        }
    }

    Component.onDestruction: {
    }

    states: [
        State {
            name: "page1"
            PropertyChanges { target: firstPageColumn; x: (root.width - privates.columnWidth) / 2 }
            PropertyChanges { target: secondPageColumn; x: root.width + (root.width - privates.columnWidth) / 2 }
            PropertyChanges { target: buttonNext; x: root.width - (privates.buttonWidth + (root.height * 0.025)) }
            PropertyChanges { target: buttonAccept; x: (2 * root.width) - (privates.buttonWidth + (root.height * 0.025)) }
            PropertyChanges { target: buttonBack; x: (privates.buttonWidth + (root.height * 0.025)) + root.width }
        },
        State {
            name: "page2"
            PropertyChanges { target: firstPageColumn; x: ((root.width - privates.columnWidth) / 2) - root.width }
            PropertyChanges { target: secondPageColumn; x: (root.width - privates.columnWidth) / 2 }
            PropertyChanges { target: buttonNext; x: -(privates.buttonWidth + (root.height * 0.025)) }
            PropertyChanges { target: buttonAccept; x: root.width - (privates.buttonWidth + (root.height * 0.025)) }
            PropertyChanges { target: buttonBack; x: root.height * 0.025 }
        }
    ]

    ListModel {
        id: securityTypesModel
        ListElement { text: "None" }
        ListElement { text: "SSL/TLS (if available)" }
        ListElement { text: "SSL/TLS (always)" }
        ListElement { text: "STARTTLS (if available)" }
        ListElement { text: "STARTTLS (always)" }
    }

    ListModel {
        id: authenticationTypesModel
        ListElement { text: "PLAIN"; }
        ListElement { text: "CRAM_MD5"; }
    }

    ListModel {
        id: smtpAuthenticationTypesModel
        ListElement { text: "PLAIN"; }
        ListElement { text: "CRAM_MD5"; }
        ListElement { text: "LOGIN"; }
        ListElement { text: "AUTOMATIC"; }
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
            text: qsTr("ADD_A_MAIL_SERVER") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    Column {
        id: firstPageColumn
        anchors.top: topBar.bottom
        anchors.margins: root.height * 0.05;
        spacing: root.height * 0.025;
        width: privates.columnWidth;

        Behavior on x {
            NumberAnimation { duration: 200 }
        }

        RowLayout {
            spacing: parent.width * 0.1
            ExclusiveGroup { id: mailProtocolGroup }
            RadioButton {
                id: radioIMAP;
                text: qsTr("IMAP") + UiEngine.EmptyLangString;
                checked: true;
                height: privates.textFieldHeight * 0.5;
                exclusiveGroup: mailProtocolGroup;
                style: whiteRadioButtonStyle;
            }
            RadioButton {
                id: radioPOP3;
                text: qsTr("POP3") + UiEngine.EmptyLangString;
                height: privates.textFieldHeight * 0.5;
                exclusiveGroup: mailProtocolGroup;
                style: whiteRadioButtonStyle;
            }
        }

        TextField {
            id: titleTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            placeholderText: qsTr("ACCOUNT_TITLE") + UiEngine.EmptyLangString;
            focus: true;
        }

        TextField {
            id: usernameTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            placeholderText: qsTr("USERNAME") + UiEngine.EmptyLangString;
            focus: true;
        }

        TextField {
            id: passwordTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            echoMode: TextInput.Password;
            placeholderText: qsTr("PASSWORD") + UiEngine.EmptyLangString;
        }

        TextField {
            id: serverTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            placeholderText: qsTr("SERVER_ADDRESS") + UiEngine.EmptyLangString;
        }

        TextField {
            id: portTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            placeholderText: qsTr("PORT") + UiEngine.EmptyLangString;
        }

        ComboBox {
            id: comboBoxSecurity
            model: securityTypesModel;
            width: parent.width;
            height: width / 8;
            style: comboBoxStyle;
            currentIndex: 0;
        }

        ComboBox {
            id: comboBoxAuthentication
            model: authenticationTypesModel;
            width: parent.width;
            height: width / 8;
            style: comboBoxStyle;
            currentIndex: 0;
        }
    }

    Column {
        id: secondPageColumn
        anchors.top: topBar.bottom;
        anchors.margins: root.height * 0.1;
        spacing: root.height * 0.025;
        width: privates.columnWidth;

        Behavior on x {
            NumberAnimation { duration: 200 }
        }

        Text {
            font.pixelSize: privates.textFieldHeight * 0.5;
            text: "SMTP"
        }

        TextField {
            id: smtpServerTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            placeholderText: qsTr("SERVER_ADDRESS") + UiEngine.EmptyLangString;
        }

        TextField {
            id: smtpPortTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            placeholderText: qsTr("PORT") + UiEngine.EmptyLangString;
        }

        ComboBox {
            id: comboBoxSMTPSecurity
            model: securityTypesModel;
            width: parent.width;
            height: width / 8;
            style: comboBoxStyle;
            currentIndex: 0;
        }

        ComboBox {
            id: comboBoxSMTPAuthentication
            model: smtpAuthenticationTypesModel;
            width: parent.width;
            height: width / 8;
            style: comboBoxStyle;
            currentIndex: 0;
        }
    }

    Button {
        id: buttonNext;
        style: buttonStyle;
        width: privates.columnWidth / 3;
        height: width / 4;
        y: root.height - (height + (root.height * 0.025));
        text: qsTr("NEXT") + UiEngine.EmptyLangString;

        Behavior on x {
            NumberAnimation { duration: 200 }
        }

        onClicked: {
            if (usernameTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                usernameTextField.focus = true;
                usernameTextField.selectAll();
                return;
            }
            if (passwordTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                passwordTextField.focus = true;
                passwordTextField.selectAll();
                return;
            }
            if (serverTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                serverTextField.focus = true;
                serverTextField.selectAll();
                return;
            }
            if (portTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                portTextField.focus = true;
                portTextField.selectAll();
                return;
            }

            root.state = "page2"
        }
    }

    Button {
        id: buttonAccept;
        style: buttonStyle;
        width: privates.columnWidth / 3;
        height: width / 4;
        y: root.height - (height + (root.height * 0.025));
        text: qsTr("ACCEPT") + UiEngine.EmptyLangString;

        Behavior on x {
            NumberAnimation { duration: 200 }
        }

        onClicked: {
            if (titleTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                titleTextField.focus = true;
                titleTextField.selectAll();
                return;
            }
            if (usernameTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                usernameTextField.focus = true;
                usernameTextField.selectAll();
                return;
            }
            if (passwordTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                passwordTextField.focus = true;
                passwordTextField.selectAll();
                return;
            }
            if (serverTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                serverTextField.focus = true;
                serverTextField.selectAll();
                return;
            }
            if (portTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                portTextField.focus = true;
                portTextField.selectAll();
                return;
            }
            if (smtpServerTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                serverTextField.focus = true;
                serverTextField.selectAll();
                return;
            }
            if (smtpPortTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                portTextField.focus = true;
                portTextField.selectAll();
                return;
            }

            var result;
            if (Mail.newServerMode == "edit") {
                applicationWindow.db_initialize();
                var protocol = radioIMAP.checked ? "imap" : "pop3";
                result = applicationWindow.db_editMailAccount(titleTextField.text, protocol, usernameTextField.text,
                                                                 passwordTextField.text, serverTextField.text, portTextField.text,
                                                                 getSecurityType(comboBoxSecurity.currentIndex),
                                                                 smtpServerTextField.text, smtpPortTextField.text,
                                                                 getSecurityType(comboBoxSMTPSecurity.currentIndex));
            } else {
                applicationWindow.db_initialize();
                var protocol = radioIMAP.checked ? "imap" : "pop3";
                result = applicationWindow.db_addMailAccount(titleTextField.text, protocol, usernameTextField.text,
                                                                 passwordTextField.text, serverTextField.text, portTextField.text,
                                                                 getSecurityType(comboBoxSecurity.currentIndex),
                                                                 smtpServerTextField.text, smtpPortTextField.text,
                                                                 getSecurityType(comboBoxSMTPSecurity.currentIndex));
            }

            if (result == "OK") {
                pageLoader.setSource("qrc:///ui/MailServers.qml");
            } else {
                UiEngine.showToast(qsTr("NOT_SUCCESSFUL") + UiEngine.EmptyLangString);
            }
        }
    }

    Button {
        id: buttonBack;
        style: buttonStyle;
        width: privates.columnWidth / 3;
        height: width / 4;
        y: root.height - (height + (root.height * 0.025));
        text: qsTr("BACK") + UiEngine.EmptyLangString;

        Behavior on x {
            NumberAnimation { duration: 200 }
        }

        onClicked: {
            root.state = "page1"
        }
    }

    function getSecurityType(type) {
        switch (type) {
        case 0:     //  PLAIN
            return "plain";
        case 1:     //  SSL/TLS
        case 2:     //  SSL/TLS
            return "ssl";
        case 3:     //  STARTTLS
        case 4:     //  STARTTLS
            return "starttls";
        default:
            return "";
        }
    }
}

