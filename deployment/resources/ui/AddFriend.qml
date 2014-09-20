/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */

import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;
import "custom"

Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";
    state: "public"

    RegExpValidator {
        id: mobilePhoneRegExpValidator;
        regExp: /09\d{9}$/;
    }

    Component.onCompleted: {
    }

    Component.onDestruction: {
    }

    Bar {
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
            text: qsTr("ADD_A_FRIEND") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    Column {
        id: mainColumn
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height * 0.3
        spacing: root.height * 0.025;

        width: {
            if (UiEngine.TargetScreenType === ScreenType.Phone) {
                return parent.width * 0.8;
            } else {
                return parent.width * 0.4;
            }
        }

        Item {
            width: parent.width;

            TextField {
                id: phoneNumberTextField;
                style: textFieldStyle;
                width: parent.width * 0.7;
                height: width / 8;
                font.pixelSize: height * 0.5;
                placeholderText: qsTr("PHONE_NUMBER") + UiEngine.EmptyLangString;
                focus: true;
                validator: mobilePhoneRegExpValidator;
            }

            Button {
                id: buttonSearch
                anchors.left: phoneNumberTextField.right
                anchors.right: parent.right
                height: phoneNumberTextField.height
                anchors.margins: 5
                style: buttonStyle
                text: qsTr("SEARCH") + UiEngine.EmptyLangString;
                onClicked: {
                    //  TODO: Search for the requested member
                }
            }
        }
    }
}

