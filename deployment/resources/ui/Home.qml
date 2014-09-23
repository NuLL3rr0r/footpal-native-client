/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 * @author  Mohammad S. Babaei <info@babaei.net>
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

    QtObject {
        id: privates

        property bool isInitialized: false
        property double buttonWHRatio: 5.0
        property double buttonHeightCoef: 0.075
        property double columnSpacingCoef: 0.015
        property double buttonMaxWidthCoef: { UiEngine.TargetScreenType === ScreenType.Phone ? 0.7 : 0.5 }
    }

    Component.onCompleted: {
        privates.isInitialized = true;
    }

    Column {
        anchors.centerIn: parent
        spacing: root.height * privates.columnSpacingCoef;
        Button {
            height: root.height * privates.buttonHeightCoef;
            width: Math.min(height * privates.buttonWHRatio, root.width * privates.buttonMaxWidthCoef);
            style: buttonStyle;
            text: qsTr("CHAT") + UiEngine.EmptyLangString;
            onClicked: {
                pageLoader.setSource("qrc:///ui/ChatLog.qml")
            }
        }
        Button {
            height: root.height * privates.buttonHeightCoef;
            width: Math.min(height * privates.buttonWHRatio, root.width * privates.buttonMaxWidthCoef);
            style: buttonStyle;
            text: qsTr("FRIENDS") + UiEngine.EmptyLangString;
            onClicked: {
                pageLoader.setSource("qrc:///ui/ContactList.qml")
            }
        }
        Button {
            height: root.height * privates.buttonHeightCoef;
            width: Math.min(height * privates.buttonWHRatio, root.width * privates.buttonMaxWidthCoef);
            style: buttonStyle;
            text: qsTr("EMAIL") + UiEngine.EmptyLangString;
            onClicked: {
                pageLoader.setSource("qrc:///ui/MailServers.qml")
            }
        }
    }
}

