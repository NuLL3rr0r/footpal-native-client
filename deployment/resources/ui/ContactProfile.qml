/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 * @author  Majid Sadeghi Alavijeh <majid.sadeghi.alavijeh@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;
import "custom"
import "scripts/ws.js" as WS
import "scripts/settings.js" as Settings


Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: Settings.globalBgColor;
    state: "public"

    Component.onCompleted: {
        loadProfile();
        RestApi.onSignal_CreateIndividualRoom.connect(onCreateIndividualCallback);
    }

    Component.onDestruction: {
        RestApi.onSignal_CreateIndividualRoom.disconnect(onCreateIndividualCallback);
    }

    function onCreateIndividualCallback(connectionStatus, signInStatus, response){
        console.log(connectionStatus + " : " + signInStatus + " : " + response)
        var result = JSON.parse(response);
        console.log("room id is : " + result.roomId);
        WS.Context.currentRoomId = result.roomId;
        pageLoader.setSource("qrc:///ui/Chat.qml")
    }

    function loadProfile(){
        //TODO : Load profile from the server or local repository
    }

    QtObject {
        id: privates

        property int nameWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.7 : root.width * 0.4
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
                pageLoader.setSource("qrc:///ui/ContactList.qml")
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("CONTACT_PROFILE") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    Image {
        id: contactPicture
        height: root.height * 0.2
        width: height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topBar.bottom
        anchors.margins: root.height * 0.05
        source: "qrc:///img/ic_contact.png"
    }

    Text {
        id: contactName
        font.pixelSize: privates.nameWidth / 8
        text: "Majid Sadeghi Alavijeh"
        anchors.top: contactPicture.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: root.height * 0.05
    }

    Text {
        id: contactPhoneNumber
        font.pixelSize: privates.nameWidth / 12
        text: "+09197343303"
        anchors.top: contactName.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: root.height * 0.05
    }

    Bar {
        id: bottomBar
        anchors.bottom: parent.bottom

        Item {
            anchors.fill: parent
            anchors.margins: 5

            ExtButton {
                id: buttonNewChat
                height: parent.height;
                width: height;
                anchors.left: parent.left
                defaultImage: "qrc:///img/btn_bar_new_chat.png"
                pressedImage: "qrc:///img/btn_bar_new_chat_pressed.png"
                onSignal_clicked: {
                    console.log("try to chat with : " + WS.Context.currentContactId);
                    RestApi.createIndividualRoom(WS.Context.token, WS.Context.currentContactId);
                    //TODO : Show Progress bar that hide via timeout or by server responce in CreateiIndividualRoom call back
                }
            }
        }
    }
}

