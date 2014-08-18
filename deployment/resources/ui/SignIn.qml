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
            id: usernameTextInput;
            style: textFieldStyle;
            width: parent.width;
            placeholderText: qsTr("USERNAME") + UiEngine.EmptyLangString;
            focus: true;
        }

        TextField {
            id: passwordTextInput;
            style: textFieldStyle;
            width: parent.width;
            echoMode: TextInput.Password;
            placeholderText: qsTr("PASSWORD") + UiEngine.EmptyLangString;
        }

        Button {
            id: signInButton;
            style: buttonStyle;
            width: parent.width;
            text: qsTr("SIGN_IN") + UiEngine.EmptyLangString;
        }

        Text {
            id: forgotPassword;
            anchors.horizontalCenter: parent.horizontalCenter;
            text: qsTr("FORGOT_PASSWORD") + UiEngine.EmptyLangString;

            MouseArea {
                anchors.fill: parent;
                cursorShape: Qt.PointingHandCursor;
                onClicked: {
                    pageLoader.setSource("ForgotPassword.qml");
                }
            }
        }
    }

    Text {
        id: signUpText;
        anchors {
            bottom: parent.bottom;
            bottomMargin: parent.height * 0.05;
            horizontalCenter: parent.horizontalCenter;
        }
        text: qsTr("SIGN_UP") + UiEngine.EmptyLangString;

        MouseArea {
            anchors.fill: parent;
            cursorShape: Qt.PointingHandCursor;
            onClicked: {
                pageLoader.setSource("SingUp.qml");
            }
        }
    }
}

