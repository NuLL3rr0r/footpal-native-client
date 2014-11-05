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
import "scripts/settings.js" as Settings

Rectangle {
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: Settings.globalBgColor;

    RegExpValidator {
        id: mobilePhoneRegExpValidator;
        regExp: /09\d{9}$/;
    }

    Component.onCompleted: {
        RestApi.onSignal_SignIn.connect(onSignInCallback);
    }

    Component.onDestruction: {
        RestApi.onSignal_SignIn.disconnect(onSignInCallback);
    }

    function onSignInCallback(connectionStatus, signInStatus, response) {
        console.log("connection: " + connectionStatus +  ", status: " + signInStatus,
                    ", response: " + response);
        switch (signInStatus) {
        case 202:
            var result = JSON.parse(response);
            WS.Context.token = result.token;
            WS.authorizeToWs(WS.Context.token);
            pageLoader.setSource("Home.qml");
            break;
        case 403:
            UiEngine.showToast(qsTr("ERROR_SIGNIN_403") + UiEngine.EmptyLangString);
            break;
        case 404:
            UiEngine.showToast(qsTr("ERROR_SIGNIN_404") + UiEngine.EmptyLangString);
            break;
        case 406:
            UiEngine.showToast(qsTr("ERROR_SIGNIN_406") + UiEngine.EmptyLangString);
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

//                pageLoader.setSource("qrc:///ui/Home.qml");
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

