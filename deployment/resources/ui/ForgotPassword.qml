import QtQuick 2.3;
import QtQuick.Controls 1.1;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;

Rectangle {
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";

    Component.onCompleted: {
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
            placeholderText: qsTr("PHONE_NUMBER") + UiEngine.EmptyLangString;
            focus: true;
        }

        Button {
            id: recoverPasswordButton;
            style: buttonStyle;
            width: parent.width;
            text: qsTr("RECOVER_PASSWORD") + UiEngine.EmptyLangString;
            onClicked: {
                if (!phoneNumberTextInput.acceptableInput) {
                    UiEngine.showToast(qsTr("INVALID_PHONE_NUMBER"));
                    phoneNumberTextInput.focus = true;
                    phoneNumberTextInput.selectAll();
                    return;
                }
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
        color: "white";

        MouseArea {
            anchors.fill: parent;
            cursorShape: Qt.PointingHandCursor;
            onClicked: {
                pageLoader.setSource("SignIn.qml");
            }
        }
    }
}

