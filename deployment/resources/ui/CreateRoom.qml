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

    Component.onCompleted: {
    }

    Component.onDestruction: {
    }

    states: [
        State {
            name: "public"
            PropertyChanges { target: passwordTextField; visible: false }
            PropertyChanges { target: confirmPasswordTextField; visible: false }
        },
        State {
            name: "private"
            PropertyChanges { target: passwordTextField; visible: true }
            PropertyChanges { target: confirmPasswordTextField; visible: true }
        }
    ]

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
            text: qsTr("CREATE_A_ROOM") + UiEngine.EmptyLangString;
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
            id: roomNameTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            placeholderText: qsTr("ROOM_NAME") + UiEngine.EmptyLangString;
            focus: true;
        }

        RowLayout {
            spacing: parent.width * 0.1
            ExclusiveGroup { id: roomAccessiblityGroup }
            RadioButton {
                text: qsTr("PUBLIC") + UiEngine.EmptyLangString;
                checked: true
                exclusiveGroup: roomAccessiblityGroup
                onCheckedChanged: {
                    if (checked) {
                        root.state = "public"
                    } else {
                        root.state = "private"
                    }
                }
                style: radioButtonStyle
            }
            RadioButton {
                text: qsTr("PRIVATE") + UiEngine.EmptyLangString;
                exclusiveGroup: roomAccessiblityGroup
                style: radioButtonStyle
            }
        }

        TextField {
            id: passwordTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            echoMode: TextInput.Password;
            placeholderText: qsTr("PASSWORD") + UiEngine.EmptyLangString;
        }

        TextField {
            id: confirmPasswordTextField;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            echoMode: TextInput.Password;
            placeholderText: qsTr("CONFIRM_PASSWORD") + UiEngine.EmptyLangString;
        }
    }

    Button {
        id: confirmButton;
        style: buttonStyle;
        width: mainColumn.width;
        height: width / 8;
        anchors.horizontalCenter: parent.horizontalCenter
        y: roomNameTextField.y + 4 * (roomNameTextField.height + (root.height * 0.025));
        text: qsTr("CONFIRM") + UiEngine.EmptyLangString;
        onClicked: {
            if (roomNameTextField == "") {
                UiEngine.showToast(qsTr("INVALID_FIELD_LENGTH"));
                roomNameTextField.focus = true;
                roomNameTextField.selectAll();
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

            //  TODO: create the room
        }
    }
}

