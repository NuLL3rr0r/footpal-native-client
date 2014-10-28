/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;
import "custom"
import "scripts/settings.js" as Settings

Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: Settings.globalBgColor;

    Component.onCompleted: {
    }

    Component.onDestruction: {
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
                pageLoader.setSource("qrc:///ui/UserProfile.qml")
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("CHANGE_PASSWORD") + UiEngine.EmptyLangString;
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

        TextField {
            id: currentPasswordTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            echoMode: TextInput.Password;
            placeholderText: qsTr("CURRENT_PASSWORD") + UiEngine.EmptyLangString;
            focus: true;
        }

        TextField {
            id: passwordTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            echoMode: TextInput.Password;
            placeholderText: qsTr("NEW_PASSWORD") + UiEngine.EmptyLangString;
        }

        TextField {
            id: confirmPasswordTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            echoMode: TextInput.Password;
            placeholderText: qsTr("CONFIRM_NEW_PASSWORD") + UiEngine.EmptyLangString;
        }
    }

    Button {
        id: confirmButton;
        style: buttonStyle;
        width: mainColumn.width;
        height: width / 8;
        anchors.horizontalCenter: parent.horizontalCenter
        y: mainColumn.y + 3 * (currentPasswordTextField.height + (root.height * 0.025));
        text: qsTr("CONFIRM") + UiEngine.EmptyLangString;
        onClicked: {
            if (currentPasswordTextField == "") {
                UiEngine.showToast(qsTr("INVALID_PASSWORD_LENGTH"));
                currentPasswordTextField.focus = true;
                currentPasswordTextField.selectAll();
                return;
            }
            if (passwordTextField.text == "") {
                UiEngine.showToast(qsTr("INVALID_PASSWORD_LENGTH"));
                passwordTextField.focus = true;
                passwordTextField.selectAll();
                return;
            }
            if (confirmPasswordTextField.text != passwordTextField.text) {
                UiEngine.showToast(qsTr("PASSWORDS_WONT_MATCH"));
                confirmPasswordTextField.focus = true;
                confirmPasswordTextField.selectAll();
                return;
            }

            //  TODO: change the password
        }
    }
}

