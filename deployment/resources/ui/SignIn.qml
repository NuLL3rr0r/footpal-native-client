/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.1;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;

Rectangle {
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";

    RegExpValidator {
        id: mobilePhoneRegExpValidator;
        regExp: /09\d{9}$/;
    }

    Component.onCompleted: {
        RestApi.onSignal_SignIn.connect(onSingInSuccess);
    }

    Component.onDestruction: {
        RestApi.onSignal_SignIn.disconnect(onSingInSuccess);
    }

    function onSingInSuccess(response)
    {
        UiEngine.notify(qsTr("APP_TITLE"), response);
        UiEngine.showToast(response);
    }

    function onSingInError(response)
    {

    }

    Column {
        anchors.centerIn: parent;
        spacing: 10;

        width: {
            if (UiEngine.TargetScreenType === ScreenType.Phone) {
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
            validator: mobilePhoneRegExpValidator;
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
            onClicked: {
                if (!usernameTextInput.acceptableInput) {
                    UiEngine.showToast(qsTr("INVALID_PHONE_NUMBER"));
                    usernameTextInput.focus = true;
                    usernameTextInput.selectAll();
                    return;
                }
                if (passwordTextInput.text == "") {
                    UiEngine.showToast(qsTr("INVALID_PASSWORD_LENGTH"));
                    passwordTextInput.focus = true;
                    passwordTextInput.selectAll();
                    return;
                }

                RestApi.signIn(usernameTextInput.text, passwordTextInput.text);
            }
        }

        Text {
            id: forgotPassword;
            anchors.horizontalCenter: parent.horizontalCenter;
            color: "white";
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
        color: "white";
        text: qsTr("SIGN_UP") + UiEngine.EmptyLangString;

        MouseArea {
            anchors.fill: parent;
            cursorShape: Qt.PointingHandCursor;
            onClicked: {
                pageLoader.setSource("SignUp.qml");
            }
        }
    }
}

