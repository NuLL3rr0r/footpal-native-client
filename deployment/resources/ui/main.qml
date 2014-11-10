/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 * @author  Majid Sadeghi Alavijeh <majid.sadeghi.alavijeh@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.1;
import QtQuick.LocalStorage 2.0 as Sql;
import Qt.WebSockets 1.0
import RestStatusCodes 1.0;
import ScreenTypes 1.0;
import "custom";
import "scripts/ws.js" as WS

ApplicationWindow {
    id: applicationWindow;
    visible: true;
    width: 800;
    height: 600;
    title: qsTr("APP_TITLE");
    color: "#203070";

    signal signal_englishLanguageSelected();
    signal signal_farsiLanguageSelected();

    signal openFileDialogAccepted(string path);
    signal openFileDialogRejected();

    property Component buttonStyle: ButtonStyle {
        id: buttonStyle;
        label: Text {
            font.pixelSize: control.height * 0.5;
            text: control.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle {
            implicitHeight: 20;
            implicitWidth: 100;
            color: control.pressed ? "darkGray" : "lightGray";
            antialiasing: true;
            border.color: "gray";
            radius: height / 4;
        }
    }

    property Component comboBoxStyle: ComboBoxStyle {
        id: comboBoxStyle
        label: Text {
            text: control.currentText;
            font.pixelSize: control.height * 0.5
            verticalAlignment: Text.AlignVCenter
        }
    }

    property Component radioButtonStyle: RadioButtonStyle {
        id: radioButtonStyle
        indicator: Rectangle {
            implicitWidth: control.height
            implicitHeight: control.height
            radius: control.height / 2
            border.color: control.activeFocus ? "darkblue" : "gray"
            border.width: 1
            Rectangle {
                anchors.fill: parent
                visible: control.checked
                color: "#555"
                radius: control.height / 2
                anchors.margins: 4
            }
        }
        label: Text {
            text: control.text
            font.pixelSize: control.height * 0.75
        }
    }

    property Component whiteRadioButtonStyle: RadioButtonStyle {
        id: whiteRadioButtonStyle
        indicator: Rectangle {
            implicitWidth: control.height
            implicitHeight: control.height
            radius: control.height / 2
            border.color: control.activeFocus ? "darkblue" : "gray"
            border.width: 1
            Rectangle {
                anchors.fill: parent
                visible: control.checked
                color: "#555"
                radius: control.height / 2
                anchors.margins: 4
            }
        }
        label: Text {
            text: control.text
            font.pixelSize: control.height * 0.75
            color: "white"
        }
    }

    property Component textFieldStyle: TextFieldStyle {
        background: Rectangle {
            implicitWidth: 100;
            implicitHeight: 40;
            color: "#f0f0f0";
            antialiasing: true;
            border.color: "gray";
            radius: height / 4;
        }
    }

    Component.onCompleted: {
        WS.registerSocket(socket);
        WS.registerTimer(elapsedTimer);
        console.log(socket.url);
        elapsedTimer.start();
        pageLoader.setSource("Splash.qml");
    }

    Timer  {
        id: elapsedTimer
        interval: 1000;
        running: true;
        repeat: false;
        onTriggered : WS.TryToConnectToWS();
    }

    WebSocket {
        id: socket
        url: WS.url;
        onTextMessageReceived: {
            WS.parseTextMessage(message);
        }
        onStatusChanged: if (socket.status == WebSocket.Error) {
                             WS.webSocketError(socket.errorString);
                         } else if (socket.status == WebSocket.Open) {
                             WS.websocketOpened();
                         } else if (socket.status == WebSocket.Closed) {
                             WS.webSocketClosed();
                         }
        active: false
    }

    Loader {
        id: pageLoader;
        anchors.fill: parent;
        anchors.centerIn: parent;
    }

    function showToast(text, duration) {
        toast.timoutInterval = duration;
        toast.notificationText = "";
        toast.notificationText = text;
    }

    function openFileDialog() {
        fileDialog.open();
    }

    function onOpenFileDialogAccepted(path)
    {
        openFileDialogAccepted(path);
    }

    function onOpenFileDialogRejected()
    {
        openFileDialogRejected();
    }

    Toast {
        id: toast;
        anchors.fill: parent;
        anchors.centerIn: parent;
        fontPath: FontPath;
    }

    FileDialog {
        id: fileDialog;
        modality: Qt.WindowModal;
        nameFilters: [ "All files (*)" ];
        selectExisting: true;
        selectMultiple: false;
        selectFolder: false;
        title: qsTr("CHOOSE_FILE");
        visible: false;

        onAccepted: {
            applicationWindow.openFileDialogAccepted(fileDialog.fileUrl + "");
        }
        onRejected: {
            applicationWindow.openFileDialogRejected();
        }
    }

    /**********************************************************************************************
                                           DATABASE FUNCTIONS
     **********************************************************************************************/

    function db_getDatabase() {
        return Sql.LocalStorage.openDatabaseSync("SettingsDB", "1.0", "StorageDatabase", 100000);
    }

    function db_initialize() {
        var db = db_getDatabase();
        db.transaction(
                    function(tx) {
                        // Mail Accounts Table
                        tx.executeSql('CREATE TABLE IF NOT EXISTS MailServers(name TEXT PRIMARY KEY,' +
                                      'protocol TEXT, username TEXT, password TEXT, readHost TEXT, readPort INTEGER,' +
                                      'readSecurity TEXT, sendHost TEXT, sendPort INTEGER, sendSecurity TEXT)');
                    });
    }

    function db_addMailAccount(name, protocol, username, password, readHost, readPort, readSecurity,
                            sendHost, sendPort, sendSecurity) {
        var db = db_getDatabase();
        var res = "";
        db.transaction(function(tx) {
            var rs = tx.executeSql('INSERT OR REPLACE INTO MailServers VALUES (?,?,?,?,?,?,?,?,?,?);',
                                   [name, protocol, username, password, readHost, readPort, readSecurity,
                                    sendHost, sendPort, sendSecurity]);
            if (rs.rowsAffected > 0) {
                res = "OK";
            } else {
                res = "Error";
            }
        });
        return res;
    }

    function db_editMailAccount(name, protocol, username, password, readHost, readPort, readSecurity,
                                sendHost, sendPort, sendSecurity) {
        var db = db_getDatabase();
        var res = "";
        db.transaction(function(tx) {
            var rs = tx.executeSql('UPDATE MailServers SET protocol=?, username=?, password=?, ' +
                                   'readHost=?, readPort=?, readSecurity=?, sendHost=?, sendPort=?, ' +
                                   'sendSecurity=? WHERE name=?',
                                   [protocol, username, password, readHost, readPort, readSecurity,
                                    sendHost, sendPort, sendSecurity, name]);
            if (rs.rowsAffected > 0) {
                res = "OK";
            } else {
                res = "Error";
            }
        });
        return res;
    }

    function db_getMailAccount(name) {
        var db = db_getDatabase();
        var res = "";
        db.transaction(function(tx) {
            var rs = tx.executeSql('SELECT * FROM MailServers WHERE name=?;', [name]);
            if (rs.rows.length > 0) {
                var result = { name: rs.rows.item(0).name,
                    protocol: rs.rows.item(0).protocol,
                    username: rs.rows.item(0).username,
                    password: rs.rows.item(0).password,
                    readHost: rs.rows.item(0).readHost,
                    readPort: rs.rows.item(0).readPort,
                    readSecurity: rs.rows.item(0).readSecurity,
                    sendHost: rs.rows.item(0).sendHost,
                    sendPort: rs.rows.item(0).sendPort,
                    sendSecurity: rs.rows.item(0).sendSecurity
                }
                res = result;// JSON.stringify(result);
            } else {
                res = "Unknown";
            }
        });
        return res
    }

    function db_getMailAccounts() {
        var db = db_getDatabase();
        var res = "";
        db.transaction(function(tx) {
            var rs = tx.executeSql('SELECT name, username FROM MailServers;');

            if (rs.rows.length > 0) {
                var servers = [];
                for (var i = 0; i < rs.rows.length; i++) {
                    servers.push({ name: rs.rows.item(i).name, username: rs.rows.item(i).username });
                }
                var jsonRoot = { server: servers };
                res = JSON.stringify(jsonRoot);
            }
        });
        return res;
    }
}
