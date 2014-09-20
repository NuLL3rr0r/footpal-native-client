/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 *
 * @author  Majid Sadeghi Alavijeh <majid.sadeghi.alavijeh@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import Qt.WebSockets 1.0
import RestStatusCodes 1.0;
import ScreenTypes 1.0;
import "custom";
import "scripts/ws.js" as WS

ApplicationWindow {
    visible: true;
    width: 480;
    height: 600;
    title: qsTr("APP_TITLE");
    color: "#203070";

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
        console.log(socket.url);
        WS.openSocket();
        pageLoader.setSource("Splash.qml");    
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

    Toast {
        id: toast;
        anchors.fill: parent;
        anchors.centerIn: parent;
        fontPath: FontPath;
    }
}
