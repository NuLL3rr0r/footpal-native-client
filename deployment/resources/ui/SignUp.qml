/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import RestStatusCodes 1.0;
import ScreenTypes 1.0;
import "scripts/settings.js" as Settings

Rectangle {
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: Settings.globalBgColor;

    RegExpValidator {
        id: emailRegExpValidator;
        regExp: /[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+(?:[a-z]{2}|com|org|net|edu|gov|mil|biz|info|mobi|name|aero|asia|jobs|museum)\b/;
    }

    RegExpValidator {
        id: mobilePhoneRegExpValidator;
        regExp: /09\d{9}$/;
    }

    Component.onCompleted: {
        RestApi.onSignal_SignUp.connect(onSignUpCallback);
    }

    Component.onDestruction: {
        RestApi.onSignal_SignUp.disconnect(onSignUpCallback);
    }

    function onSignUpCallback(connectionStatus, statusCode, data)
    {
        console.log("connection: " + connectionStatus, ", status: " + statusCode + ", data: "
                    + data);

        switch (statusCode) {
        case 201:
            UiEngine.showToast(qsTr("ERROR_SIGNUP_201") + UiEngine.EmptyLangString);
            pageLoader.setSource("qrc:///ui/SignIn.qml");
            break;
        case 409:
            UiEngine.showToast(qsTr("ERROR_SIGNUP_409") + UiEngine.EmptyLangString);
            break;
        case 400:
        default:
            break;
        }
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
            id: phoneNumberTextInput;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            placeholderText: qsTr("PHONE_NUMBER") + UiEngine.EmptyLangString;
            focus: true;
            validator: mobilePhoneRegExpValidator;
        }

        TextField {
            id: emailTextInput;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            placeholderText: qsTr("EMAIL") + UiEngine.EmptyLangString;
            validator: emailRegExpValidator;
        }

        TextField {
            id: passwordTextInput;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            echoMode: TextInput.Password;
            placeholderText: qsTr("PASSWORD") + UiEngine.EmptyLangString;
        }

        TextField {
            id: confirmPasswordTextInput;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8;
            font.pixelSize: height * 0.5;
            echoMode: TextInput.Password;
            placeholderText: qsTr("CONFIRM_PASSWORD") + UiEngine.EmptyLangString;
        }

        Button {
            id: signUpButton;
            style: buttonStyle;
            width: parent.width;
            height: width / 8;
            text: qsTr("SIGN_UP") + UiEngine.EmptyLangString;
            onClicked: {
                if (!phoneNumberTextInput.acceptableInput) {
                    UiEngine.showToast(qsTr("INVALID_PHONE_NUMBER"));
                    phoneNumberTextInput.focus = true;
                    phoneNumberTextInput.selectAll();
                    return;
                }
                if (!emailTextInput.acceptableInput) {
                    UiEngine.showToast(qsTr("INVALID_EMAIL_ADDRESS"));
                    emailTextInput.focus = true;
                    emailTextInput.selectAll();
                    return;
                }
                if (passwordTextInput.text == "") {
                    UiEngine.showToast(qsTr("INVALID_PASSWORD_LENGTH"));
                    passwordTextInput.focus = true;
                    passwordTextInput.selectAll();
                    return;
                }
                if (confirmPasswordTextInput.text != passwordTextInput.text) {
                    UiEngine.showToast(qsTr("PASSWORDS_WONT_MATCH"));
                    confirmPasswordTextInput.focus = true;
                    confirmPasswordTextInput.selectAll();
                    return;
                }

                RestApi.signUp(phoneNumberTextInput.text, emailTextInput.text, passwordTextInput.text);
            }
        }
    }

    Text {
        id: returnText;
        anchors {
            bottom: parent.bottom;
            bottomMargin: parent.height * 0.05;
            horizontalCenter: parent.horizontalCenter;
        }
        color: "white";
        text: qsTr("RETURN") + UiEngine.EmptyLangString;
        font.pixelSize: phoneNumberTextInput.font.pixelSize;

        MouseArea {
            anchors.fill: parent;
            cursorShape: Qt.PointingHandCursor;
            onClicked: {
                pageLoader.setSource("SignIn.qml");
            }
        }
    }
}

