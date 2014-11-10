/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtQuick.Controls.Styles 1.2;
import QtQuick.Layouts 1.1;
import ScreenTypes 1.0;
import "custom"
import "utils"
import "scripts/ws.js" as WS
import "scripts/settings.js" as Settings


Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: Settings.globalBgColor;
    state: "public"

    Component.onCompleted: {
        RestApi.onSignal_GetCurrentProfile.connect(onGetCurrentProfileCallback);

        loadProfile();
        console.log(jsonModel.model.get(0).things.length());
    }

    Component.onDestruction: {
        RestApi.onSignal_GetCurrentProfile.disconnect(onGetCurrentProfileCallback);
    }

    QtObject {
        id: privates
        property int firstNameWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.3 : root.width * 0.3
        property int lastNameWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.4 : root.width * 0.4
        property int nameColumnWidth: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.75 : root.width * 0.5
        property int textFieldHeight: nameColumnWidth / 8;// root.height * 0.075
        property int textHeight: root.height * 0.05
        property string jsonTest: "{                                                                                                     "+
                                  "    \"data\":                                                                                         "+
                                  "    [                                                                                                 "+
                                  "        {                                                                                             "+
                                  "            \"message_id\": \"1584243002.894505385.1415110596157.JavaMail.root@sjmas02.marketo.org\", "+
                                  "            \"from\":                                                                                 "+
                                  "            [                                                                                         "+
                                  "                {                                                                                         "+
                                  "                     \"email\": \"sales@lisasoft.com\",                                                    "+
                                  "                     \"name\": \"JulietLin\"                                                               "+
                                  "                }                                                                                        "+
                                  "            ]," +
                                  "            \"subject\": \"How to reduce your IT Spend with Postgres\",                               "+
                                  "            \"date\": \"2:4:11:2014:8:16:36:+0330\",                                                       "+
                                  "            \"to\":                                                                                   "+
                                  "            [                                                                                         "+
                                  "                {                                                                                     "+
                                  "                    \"name\": \"\",                                                                   "+
                                  "                    \"email\": \"arcananemous@gmail.com\",                                            "+
                                  "                    \"type\": \"TO\"                                                                  "+
                                  "                }                                                                                     "+
                                  "            ],                                                                                        "+
                                  "            \"headers\":                                                                              "+
                                  "            {                                                                                         "+
                                  "                \"Reply-To\": \"edbmarketing@enterprisedb.com\"                                       "+
                                  "            },                                                                                        "+
                                  "            \"text\": \"<HTML><BODY test=\\\"2323232 fff\\\">FFFJFJFJJF</BODY></HTML>\""+
                                  "        } "+
                                  "    ]     "+
                                  "}         ";
    }

    ListModel {
        id: languageModel
        ListElement { text: "English"; }
        ListElement { text: "فارسی"; }
    }

    JSONListModel {
        id: jsonModel;
        json: privates.jsonTest;
        query: "$.data.items[*]";
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
                pageLoader.setSource("qrc:///ui/Home.qml")
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("MY_PROFILE") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    Image {
        id: profilePicture
        height: root.height * 0.2
        width: height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topBar.bottom
        anchors.margins: root.height * 0.05
        source: "qrc:///img/ic_contact.png"
    }

    Column {
        id: nameColumn
        anchors.top: profilePicture.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: root.height * 0.05
        spacing: root.width * 0.025

        TextField {
            id: firstNameText
            width: privates.nameColumnWidth
            height: privates.textFieldHeight
            font.pixelSize: height / 2
            text: "Morteza"
            style: textFieldStyle
        }

        TextField {
            id: lastNameText
            width: privates.nameColumnWidth
            height: privates.textFieldHeight
            font.pixelSize: height / 2
            text: "Sabetraftar"
            style: textFieldStyle
        }
    }

    ComboBox {
        id: comboBoxLanguage
        model: languageModel;
        width: nameColumn.width;
        height: privates.textFieldHeight;
        anchors.top: nameColumn.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: root.height * 0.05
        style: comboBoxStyle;
        currentIndex: WS.Context.currentLanguage === "EN" ? 0 : 1;
        onCurrentIndexChanged: {
            if (currentIndex == 0) {
                applicationWindow.signal_englishLanguageSelected();
                WS.Context.currentLanguage = "EN";
            } else if (currentIndex == 1) {
                applicationWindow.signal_farsiLanguageSelected();
                WS.Context.currentLanguage = "FA";
            }
        }
    }

    Text {
        id: emailText
        height: privates.textFieldHeight
        font.pixelSize: height / 2
        text: "morteza.sabetraftar@gmail.com"
        anchors.top: comboBoxLanguage.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: root.height * 0.05
        color: "white"
    }

    Text {
        id: phoneNumberText
        height: privates.textFieldHeight
        font.pixelSize: height / 2
        text: "+989125300764"
        anchors.top: emailText.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: root.height * 0.025
        color: "white"
    }

    Text {
        id: signUpText;
        height: privates.textHeight
        anchors {
            bottom: parent.bottom;
            bottomMargin: root.height * 0.05;
            horizontalCenter: parent.horizontalCenter;
        }
        color: "white";
        text: qsTr("CHANGE_PASSWORD") + UiEngine.EmptyLangString;
        font.pixelSize: height / 2

        MouseArea {
            anchors.fill: parent;
            cursorShape: Qt.PointingHandCursor;
            onClicked: {
                pageLoader.setSource("ChangePassword.qml");
            }
        }
    }

    function loadProfile() {
        RestApi.getCurrentProfile(WS.Context.token);
    }

    function onGetCurrentProfileCallback(connectionStatus, signInStatus, response) {
        //        console.log("c")
    }
}

