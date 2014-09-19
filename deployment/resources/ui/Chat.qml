/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 * @author  Mohammad S. Babaei <info@babaei.net>
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

        property bool isInitialized: false
        property int barHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property int bottomBarHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.1 : 50
        property int maxLeftBubbleWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.5 : Math.min(root.width * 0.75, 256)
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5
        property int imageSize: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.15 : 75
    }

    Component.onCompleted: {
        privates.isInitialized = true;
    }

    Rectangle {
        id: topBar
        width: root.width
        height: privates.barHeight
        color: "#333"
        z: 1

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
        json: "{ \"log\" : { \"conversation\" : [ "
              + "{ \"self\" : \"false\", \"contact\" : \"Majid\", \"date\" : \"9/17/2014\", \"time\": \"15:24\", \"content\": \"Hi!\" }, "
              + "{ \"self\" : \"true\", \"date\" : \"10/17/2014\", \"time\": \"13:12\", \"content\": \"How are you doing?\" }, "
              + "{ \"self\" : \"false\", \"date\" : \"11/17/2014\", \"time\": \"10:44\", \"content\": \"Is everything alright? I tried to call you the other day.\" }"
              + " ] } }"
        query: "$.log.conversation[*]"
    }

    ListModel {
        id: model1

        ListElement { contact: "Majid"; date: "9/17/2014"; time: "15:24" }
        ListElement { contact: "Majid"; date: "9/17/2014"; time: "15:24" }
        ListElement { contact: "Majid"; date: "9/17/2014"; time: "15:24" }
        ListElement { contact: "Majid"; date: "9/17/2014"; time: "15:24" }
        ListElement { contact: "Majid"; date: "9/17/2014"; time: "15:24" }
    }

    ListView {
        anchors.top: topBar.bottom
        anchors.bottom: bottomBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        spacing: privates.itemSpacing
        model: jsonModel.model;
        delegate: Component {
            Rectangle {
                color: "#00ffffff"
                width: parent.width
                height: childrenRect.height + 10

                Row {
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    layoutDirection: model.self === "true" ? Qt.RightToLeft : Qt.LeftToRight
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
                    LeftBubble {
                        id: speechBubble
                        isSenderSelf: model.self === "true" ? true : false
                        //                    anchors.left: isSenderSelf ? null : contactImage.right
                        anchors.top: parent.top
                        anchors.margins: 5
                        width: childrenRect.width + speechBubble.border.left + speechBubble.border.right
                        height: childrenRect.height + speechBubble.border.top + speechBubble.border.bottom

                        Text {
                            id: contentText
                            text: model.content
                            x: speechBubble.border.left
                            anchors.verticalCenter: parent.verticalCenter
                            wrapMode: Text.NoWrap
                            property bool updateFlag: false

                            onContentSizeChanged: {
                                if (width > privates.maxLeftBubbleWidth) {
                                    console.log(width)
                                    wrapMode = Text.WordWrap
                                    width = privates.maxLeftBubbleWidth
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: timeRect
                        //                    anchors.left: speechBubble.right
                        anchors.top: parent.top
                        anchors.margins: 5
                        width: childrenRect.width + 10
                        height: childrenRect.height + 5

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

    Rectangle {
        id: bottomBar
        width: root.width
        height: privates.barHeight
        anchors.bottom: parent.bottom
        color: "#333"
        z: 1

        Row {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            TextField {
                id: messageTextField
                height: parent.height
                width: parent.width * 0.6
                font.pixelSize: height * 0.5
                placeholderText: qsTr("TYPE_HERE") + UiEngine.EmptyLangString;
                focus: true;
            }

            Button {
                id: buttonSubmit
                style: buttonStyle
                width: parent.width * 0.25
                height: parent.height
                text: qsTr("SUBMIT") + UiEngine.EmptyLangString;
                onClicked: {
                    if (messageTextField.text == "")
                        return;
                    var data = { 'self': "true", 'contact': "Self", 'date': new Date().toDateString(), 'time': new Date().toTimeString().substring(0, 5), 'content': messageTextField.text }
                    jsonModel.model.append(data)
                    messageTextField.text = ""
                }
            }
        }
    }
}

