/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 * @author  Majid Sadeghi Alavijeh <majid.sadeghi.alavijeh@gmail.com>
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;
import "scripts/ws.js" as WS;
import "custom";
import "utils";


Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";

    QtObject {
        id: privates

        property bool isInitialized: false
        property int barHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property int bottomBarHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.1 : 50
        property int maxLeftBubbleWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.5 : Math.min(root.width * 0.75, 256)
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5
        property int imageSize: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.15 : 75
    }

    Component.onCompleted: {
        privates.isInitialized = true;
        initChatRoom();
    }

    function messageHandler(roomId, message){
        console.log("chat room message handler");
        var self = false;
        if(message.from === WS.Context.currentProfile.id || message.from === 'self')
            self = true;
        console.log("self : " + self + " : " + WS.Context.currentProfile.id )
        var data = { 'self': self, 'contact': message.from , 'date': message.date , 'time': message.time , 'content': message.body };
        console.log(data)
        jsonModel.model.append(data)
    }

    function initChatRoom(){
        console.log("Room id is : " + WS.Context.currentRoomId);
        var room = WS.Context.getRoomById(WS.Context.currentRoomId);
        if(room){
            var messages = room.getMessages();
            console.log("Exist Messages : " + messages.length)
            for(var i = 0 ; i < messages.length ; i++){
                messageHandler(WS.Context.currentRoomId, messages[i])
            }
            WS.Context.regitserRoomMessageReaderCallback(WS.Context.currentRoomId, messageHandler);
        }
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
                pageLoader.setSource("qrc:///ui/ChatLog.qml")
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("CHAT") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    JSONListModel {
        id: jsonModel
        json: ""
        query: "$.log.conversation[*]"
    }

    ListView {
        anchors.top: topBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        spacing: privates.itemSpacing
        model: jsonModel.model;
        delegate: Component {
            Rectangle {
                color: "#00ffffff"
                width: parent.width
                height: deleateRow.height + 10

                Row {
                    id: deleateRow
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    layoutDirection: model.self == true ? Qt.RightToLeft : Qt.LeftToRight
                    spacing: 5
                    Image {
                        id: contactImage
                        //                    anchors.left: model.self === "true" ? parent.right : parent.left
                        anchors.top: parent.top
                        anchors.margins: 5
                        width: privates.imageSize
                        height: privates.imageSize
                        source: "qrc:///img/Splash.png"
                    }
                    Bubble {
                        id: speechBubble
                        isSenderSelf: model.self == true ? true : false
                        //                    anchors.left: isSenderSelf ? null : contactImage.right
                        anchors.top: parent.top
                        anchors.margins: 5
                        width: childrenRect.width + speechBubble.border.left + speechBubble.border.right
                        height: contentText.paintedHeight + speechBubble.border.top + speechBubble.border.bottom

                        Text {
                            id: contentText;
                            text: model.content;
                            wrapMode: Text.Wrap;
                            x: speechBubble.border.left;
                            anchors.verticalCenter: parent.verticalCenter;

                            onContentWidthChanged: {
                                if (width > privates.maxLeftBubbleWidth) {
                                    console.log(width)
                                    width = privates.maxLeftBubbleWidth
                                    wrapMode = Text.Wrap
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: timeRect
                        //                    anchors.left: speechBubble.right
                        anchors.top: parent.top
                        anchors.margins: 5
                        width: timeText.width + 10
                        height: timeText.height + 5

                        color: "white";
                        antialiasing: true;
                        border.color: "gray";
                        radius: height / 4;

                        Text {
                            id: timeText
                            anchors.centerIn: parent
                            text: model.time
                        }
                    }
                }
            }
        }
    }

    Bar {
        id: bottomBarExtension

        Behavior on y {
            NumberAnimation { duration: 200 }
        }

        state: "normal"

        states: [
            State {
                name: "normal"
                PropertyChanges { target: bottomBarExtension; y: root.height - height }
            },
            State {
                name: "extended"
                PropertyChanges { target: bottomBarExtension; y: root.height - 2 * height }
            }
        ]

        Item {
            anchors.fill: parent
            anchors.margins: 5

            ExtButton {
                id: buttonNewContact
                height: parent.height;
                width: height;
                anchors.left: parent.left
                defaultImage: "qrc:///img/btn_bar_new_contact.png"
                pressedImage: "qrc:///img/btn_bar_new_contact_pressed.png"
                onSignal_clicked: {
                    pageLoader.setSource("qml:///ui/ContactSelection.qml")
                }
            }

            ExtButton {
                id: buttonSettings
                height: parent.height;
                width: height;
                anchors.right: parent.right
                defaultImage: "qrc:///img/btn_bar_settings.png"
                pressedImage: "qrc:///img/btn_bar_settings_pressed.png"
                onSignal_clicked: {
                }
            }
        }
    }

    Bar {
        id: bottomBar
        anchors.bottom: parent.bottom
        z: 2

        Item {
            anchors.fill: parent
            anchors.margins: 5

            TextField {
                id: messageTextField
                height: parent.height
//                width: parent.width * 0.6
                anchors.left: parent.left
                anchors.right: buttonSubmit.left
                anchors.margins: 5
                font.pixelSize: height * 0.5
                placeholderText: qsTr("TYPE_HERE") + UiEngine.EmptyLangString;
                focus: true;
            }

            Button {
                id: buttonSubmit
                style: buttonStyle
                width: parent.width * 0.25
                anchors.right: buttonMore.left
                anchors.margins: 5
                height: parent.height
                text: qsTr("SUBMIT") + UiEngine.EmptyLangString;
                onClicked: {
                    if (messageTextField.text == "")
                        return;
//                    var data = { 'self': "true", 'contact': "Self", 'date': new Date().toDateString(), 'time': new Date().toTimeString().substring(0, 5), 'content': messageTextField.text }
//                    jsonModel.model.append(data)
//                    messageTextField.text = ""
                    WS.sendTextMessageToRoom(messageTextField.text, WS.Context.currentRoomId)
                }
            }

            ExtButton {
                id: buttonMore
                height: parent.height;
                width: height;
                anchors.right: parent.right
                defaultImage: "qrc:///img/btn_bar_more.png"
                pressedImage: "qrc:///img/btn_bar_more_pressed.png"
                onSignal_clicked: {
                    if (bottomBarExtension.state == "normal") {
                        bottomBarExtension.state = "extended"
                    } else {
                        bottomBarExtension.state = "normal"
                    }
                }
            }
        }
    }
}

