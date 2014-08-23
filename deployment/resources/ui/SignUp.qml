import QtQuick 2.3;
import QtQuick.Controls 1.1;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;

Rectangle {
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";

    RegExpValidator {
        id: emailRegExpValidator;
        regExp: /[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+(?:[a-z]{2}|com|org|net|edu|gov|mil|biz|info|mobi|name|aero|asia|jobs|museum)\b/;
    }

    RegExpValidator {
        id: mobilePhoneRegExpValidator;
        regExp: /09\d{9}$/;
    }

    Component.onCompleted: {
        RestApi.onSignal_SignUp.connect(onSingUpSuccess);
    }

    function onSingUpSuccess(response)
    {
        UiEngine.notify(qsTr("APP_TITLE"), response);
        UiEngine.showToast(response);
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
            validator: mobilePhoneRegExpValidator;
        }

        TextField {
            id: emailTextInput;
            style: textFieldStyle;
            width: parent.width;
            placeholderText: qsTr("EMAIL") + UiEngine.EmptyLangString;
            validator: emailRegExpValidator;
        }

        TextField {
            id: passwordTextInput;
            style: textFieldStyle;
            width: parent.width;
            echoMode: TextInput.Password;
            placeholderText: qsTr("PASSWORD") + UiEngine.EmptyLangString;
        }

        Button {
            id: signUpButton;
            style: buttonStyle;
            width: parent.width;
            text: qsTr("SIGN_UP") + UiEngine.EmptyLangString;
            onClicked: {
                if (!phoneNumberTextInput.acceptableInput) {
                    phoneNumberTextInput.focus = true;
                    phoneNumberTextInput.selectAll();
                    return;
                }
                if (!emailTextInput.acceptableInput) {
                    emailTextInput.focus = true;
                    emailTextInput.selectAll();
                    return;
                }
                if (passwordTextInput.text == "") {
                    passwordTextInput.focus = true;
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

