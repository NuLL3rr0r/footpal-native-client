/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 * @author  Majid Sadeghi Alavijeh <majid.sadeghi.alavijeh@gmail.com>
 */

import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;
import "scripts/ws.js" as WS

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
        var result = JSON.parse(response);
        WS.Context.token = result.token;
        console.log(WS.Context.token);
        WS.authorizeToWs(WS.Context.token);
        pageLoader.setSource("Home.qml");
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
            height: width / 8;
            placeholderText: qsTr("USERNAME") + UiEngine.EmptyLangString;
            validator: mobilePhoneRegExpValidator;
            focus: true;
            font.pixelSize: height * 0.5
        }

        TextField {
            id: passwordTextInput;
            style: textFieldStyle;
            width: parent.width;
            height: width / 8
            echoMode: TextInput.Password;
            placeholderText: qsTr("PASSWORD") + UiEngine.EmptyLangString;
            font.pixelSize: height * 0.5
        }

        Button {
            id: signInButton;
            style: buttonStyle;
            width: parent.width;
            height: width / 8
            text: qsTr("SIGN_IN") + UiEngine.EmptyLangString;
            onClicked: {
//                if (!usernameTextInput.acceptableInput) {
//                    UiEngine.showToast(qsTr("INVALID_PHONE_NUMBER"));
//                    usernameTextInput.focus = true;
//                    usernameTextInput.selectAll();
//                    return;
//                }
//                if (passwordTextInput.text == "") {
//                    UiEngine.showToast(qsTr("INVALID_PASSWORD_LENGTH"));
//                    passwordTextInput.focus = true;
//                    passwordTextInput.selectAll();
//                    return;
//                }

//                RestApi.signIn(usernameTextInput.text, passwordTextInput.text);
                pageLoader.setSource("Home.qml");
            }
        }

        Text {
            id: forgotPassword;
            anchors.horizontalCenter: parent.horizontalCenter;
            color: "white";
            text: qsTr("FORGOT_PASSWORD") + UiEngine.EmptyLangString;
            font.pixelSize: passwordTextInput.font.pixelSize

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
        font.pixelSize: passwordTextInput.font.pixelSize

        MouseArea {
            anchors.fill: parent;
            cursorShape: Qt.PointingHandCursor;
            onClicked: {
                pageLoader.setSource("SignUp.qml");
            }
        }
    }
}

