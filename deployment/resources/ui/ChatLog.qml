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
import "scripts/ws.js" as WS
import "custom"
import "utils"

Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";

    QtObject {
        id: privates

        property bool isInitialized: false
        property int barHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.2 : 100
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5
    }

    Component.onCompleted: {
        privates.isInitialized = true;
        privates.itemHeight = UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.2 : 100
        loadConversations();
    }

    function loadConversations(){
        console.log("Load conversations");
        console.log(WS.Context.rooms.length);
        for(var i = 0 ; i < WS.Context.rooms.length ; i++){
            var tmpConversation = { "contact" : WS.Context.rooms[i].id , "date" : "9/17/2014", "time": "15:24" };
             jsonModel.model.append(tmpConversation);
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
                pageLoader.setSource("qrc:///ui/Home.qml")
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("CONVERSATIONS") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    JSONListModel {
        id: jsonModel
        json: ""
    }

    ListView {
        anchors.top: topBar.bottom
        anchors.bottom: bottomBar.top
        width: parent.width
        spacing: privates.itemSpacing
        model: jsonModel.model;
        delegate: Component {
            Rectangle {
                color: "#aaffffff"
                width: parent.width
                height: privates.itemHeight

                Image {
                    id: contactImage
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 5
                    width: height
                    source: "qrc:///img/Splash.png"
                }
                Text {
                    id: contactText
                    anchors.left: contactImage.right
                    anchors.top: parent.top
                    anchors.margins: 5
                    text: model.contact
                }
                Text {
                    id: dateText
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 5
                    text: model.date
                }
                Text {
                    id: timeText
                    anchors.right: parent.right
                    anchors.top: dateText.bottom
                    anchors.margins: 5
                    text: model.time
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log(index);
                        console.log(model.contact + ":" + WS.Context.currentRoomId);
                        WS.Context.currentRoomId = model.contact;
                        pageLoader.setSource("qrc:///ui/Chat.qml");
                    }
                }
            }
        }
    }

    Bar {
        id: bottomBar
        anchors.bottom: parent.bottom

        Row {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            ExtButton {
                height: parent.height * 0.8;
                width: height;
                anchors.verticalCenter: parent.verticalCenter
                defaultImage: "qrc:///img/btn_bar_new_chat.png"
                pressedImage: "qrc:///img/btn_bar_new_chat_pressed.png"
                onSignal_clicked: {
                    pageLoader.setSource("qrc:///ui/ContactSelection.qml")
                }
            }
        }
    }
}

