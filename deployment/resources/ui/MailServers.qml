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

Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";

    QtObject {
        id: privates

        property bool isInitialized: false;
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.2 : 100;
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5;
        property string testJSON: "{ \"mailservers\":" +
                                  "{ \"server\": [" +
                                  "{ \"icon\": \"qrc:///img/ic_gmail.png\", \"username\": \"morteza.sabetraftar@gmail.com\", \"count\": 4 }," +
                                  "{ \"icon\": \"qrc:///img/ic_yahoomail.png\", \"username\": \"m_4th_riche@yahoo.com\", \"count\": 2 } ] } }"
    }

    Component.onCompleted: {
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
        query: "$.mailservers.server[*]";
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
                    source: model.icon
                }
                Text {
                    id: usernameText
                    anchors.left: serverIcon.right
                    anchors.verticalCenter: parent.verticalCenter
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
                    value: model.count
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // TODO: prepare to go the inbox for selected mail server

                        pageLoader.setSource("qrc:///ui/Mailbox.qml");
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

