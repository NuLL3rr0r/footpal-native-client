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
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.15 : 100;
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5;
        property string testJSON: "{ \"server\": [" +
                                  "{ \"name\": \"GMail\", \"username\": \"morteza.sabetraftar@gmail.com\", \"count\": 4 }," +
                                  "{ \"name\": \"Yahoomail!\", \"username\": \"m_4th_riche@yahoo.com\", \"count\": 2 } ] } }"
    }

    Component.onCompleted: {
        applicationWindow.db_initialize();
        var servers = applicationWindow.db_getMailAccounts();

        if (servers !== "")
            jsonModel.json = servers;

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
                pageLoader.setSource("qrc:///ui/Home.qml")
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("E-MAIL") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    JSONListModel {
        id: jsonModel;
        json: privates.testJSON;
        query: "$.server[*]";
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
                    id: serverIcon
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 5
                    width: height
                    source: "qrc:///img/ic_mailbox.png"
                }
                Text {
                    id: titleText
                    anchors.left: serverIcon.right
                    anchors.top: parent.top
                    anchors.margins: 10
                    text: model.name
                    font.pixelSize: parent.height * 0.25
                }
                Text {
                    id: usernameText
                    anchors.left: serverIcon.right
                    anchors.top: titleText.bottom
                    anchors.margins: 10
                    text: model.username
                    font.pixelSize: parent.height * 0.2
                }
                NumberCircle {
                    id: inboxCount
                    height: parent.height * 0.4
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 10
                    value: 0
                    visible: value > 0;
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        applicationWindow.db_initialize();
                        Mail.currentMailAccount = applicationWindow.db_getMailAccount(model.name);
                        pageLoader.setSource("qrc:///ui/Mailbox.qml");
                    }
                    onPressAndHold: {
                        Mail.newServerMode = "edit";
                        Mail.currentMailAccount = applicationWindow.db_getMailAccount(model.name);
                        pageLoader.setSource("qrc:///ui/AddMailServer.qml");
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
                defaultImage: "qrc:///img/btn_bar_new_mail_server.png"
                pressedImage: "qrc:///img/btn_bar_new_mail_server_pressed.png"
                onSignal_clicked: {
                    pageLoader.setSource("qrc:///ui/AddMailServer.qml");
                }
            }
        }
    }
}

