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

    RegExpValidator {
        id: mobilePhoneRegExpValidator;
        regExp: /09\d{9}$/;
    }

    QtObject {
        id: privates

        property bool isInitialized: false
        property int barHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.2 : 100
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5
        property int checkBoxSize: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.05 : 25
        property int buttonHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
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
            text: qsTr("MY_FRIENDS") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    JSONListModel {
        id: jsonModel
        json: "{ \"friends\" : { \"contact\" : [ "
              + "{ \"name\" : \"Majid\", \"picture\" : \"qrc:///img/ic_contact.png\" }, "
              + "{ \"name\" : \"Sadegh\", \"picture\" : \"qrc:///img/ic_contact.png\" }, "
              + "{ \"name\" : \"Morteza\", \"picture\" : \"qrc:///img/ic_contact.png\" }"
              + " ] } }"
        query: "$.friends.contact[*]"
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
                    id: contactImage
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 5
                    width: height
                    source: model.picture
                }
                Text {
                    id: contactText
                    anchors.left: contactImage.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 5
                    text: model.name
                    font.pixelSize: parent.height * 0.2
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
                id: buttonNewContact
                height: parent.height * 0.8;
                width: height;
                anchors.verticalCenter: parent.verticalCenter
                defaultImage: "qrc:///img/btn_bar_new_contact.png"
                pressedImage: "qrc:///img/btn_bar_new_contact_pressed.png"
                onSignal_clicked: {
                    if (newContactBar.state == "normal") {
                        newContactBar.state = "extended"
                    } else {
                        newContactBar.state = "normal"
                    }
                }
            }
        }
    }

    Bar {
        id: newContactBar
        z: 1

        Behavior on y {
            NumberAnimation { duration: 200 }
        }

        state: "normal"

        states: [
            State {
                name: "normal"
                PropertyChanges { target: newContactBar; y: root.height - height }
            },
            State {
                name: "extended"
                PropertyChanges { target: newContactBar; y: root.height - 2 * height }
            }
        ]

        Item {
            anchors.fill: parent
            anchors.margins: 5

            TextField {
                id: phoneNumberTextField;
                style: textFieldStyle;
                anchors.left: parent.left;
                anchors.right: buttonAdd.left
                anchors.margins: 5
                height: parent.height
                font.pixelSize: height * 0.5;
                placeholderText: qsTr("PHONE_NUMBER") + UiEngine.EmptyLangString;
                focus: true;
                validator: mobilePhoneRegExpValidator;
            }

            Button {
                id: buttonAdd
                width: parent.width * 0.25
                anchors.right: parent.right
                height: parent.height
                anchors.margins: 5
                style: buttonStyle
                text: qsTr("ADD") + UiEngine.EmptyLangString;
                onClicked: {
                    if (!phoneNumberTextField.acceptableInput) {
                        UiEngine.showToast(qsTr("INVALID_PHONE_NUMBER"));
                        phoneNumberTextInput.focus = true;
                        phoneNumberTextInput.selectAll();
                        return;
                    }

                    //  TODO: Search and Add the requested member
                }
            }
        }
    }
}

