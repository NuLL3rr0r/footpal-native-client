import QtQuick 2.2;
import QtQuick.Controls 1.1;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;

Item {
    anchors.fill: parent;
    anchors.centerIn: parent;

    Component.onCompleted: {
    }

    Column {
        anchors.centerIn: parent;
        spacing: 10;

        width: {
            if (UiEngine.ScreenType == UiEngine.ScreenType_Phone) {
                return parent.width * 0.8;
            } else {
                return parent.width * 0.4;
            }
        }

        TextField {
            id: phoneNumberTextInput;
            style: textFieldStyle;
            width: parent.width;
            placeholderText: qsTr("PHONE_NUMBER") + UiEngine.EmptyLangString;
            focus: true;
        }

        TextField {
            id: passwordTextInput;
            style: textFieldStyle;
            width: parent.width;
            echoMode: TextInput.Password;
            placeholderText: qsTr("PASSWORD") + UiEngine.EmptyLangString;
        }

        TextField {
            id: confirmPasswordTextInput;
            style: textFieldStyle;
            width: parent.width;
            echoMode: TextInput.Password;
            placeholderText: qsTr("CONFIRM_PASSWORD") + UiEngine.EmptyLangString;
        }

        Button {
            id: signUpButton;
            style: buttonStyle;
            width: parent.width;
            text: qsTr("SIGN_UP") + UiEngine.EmptyLangString;
        }
    }

    Text {
        id: returnText;
        anchors {
            bottom: parent.bottom;
            bottomMargin: parent.height * 0.05;
            horizontalCenter: parent.horizontalCenter;
        }
        text: qsTr("RETURN") + UiEngine.EmptyLangString;

        MouseArea {
            anchors.fill: parent;
            cursorShape: Qt.PointingHandCursor;
            onClicked: {
                pageLoader.setSource("SignIn.qml");
            }
        }
    }
}

