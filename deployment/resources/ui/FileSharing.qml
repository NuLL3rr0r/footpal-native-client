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

Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: "#203070";
    state: "not_private"

    QtObject {
        id: privates

        property bool isInitialized: false
        property int barHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property int radioHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.06 : 30
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.15 : 75
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5
        property int buttonHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property int checkBoxSize: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.05 : 25
        property var selectedContacts: new Array();
        property string testJson: "{ \"friends\":" +
                                  "{ \"contact\": [" +
                                  "{ \"name\": \"Majid\", \"picture\":\"qrc:///img/ic_contact.png\", \"Id\":\"123\" }, " +
                                  "{ \"name\": \"Sadegh\", \"picture\":\"qrc:///img/ic_contact.png\", \"Id\":\"456\" }, " +
                                  "{ \"name\": \"Mehdi\", \"picture\":\"qrc:///img/ic_contact.png\", \"Id\":\"789\" }, " +
                                  "{ \"name\": \"Morteza\", \"picture\":\"qrc:///img/ic_contact.png\", \"Id\":\"183\" } " +
                                  "] } }";
    }

    Component.onCompleted: {
    }

    Component.onDestruction: {
    }

    JSONListModel {
        id: jsonModel
        json: privates.testJson;
        query: "$.friends.contact[*]"
    }

    states: [
        State {
            name: "not_private"
            PropertyChanges { target: listViewContacts; visible: false }
        },
        State {
            name: "private"
            PropertyChanges { target: listViewContacts; visible: true }
        }
    ]

    Bar {
        id: topBar

        ExtButton {
            height: privates.barHeight;
            width: height;
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: (parent.height - height) / 2;
            defaultImage: "qrc:///img/btn_bar_return.png"
            pressedImage: "qrc:///img/btn_bar_return_pressed.png"
            onSignal_clicked: {
                pageLoader.setSource("qrc:///ui/Files.qml")
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("SHARING_AN_ITEM") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    Column {
        id: mainColumn;
        anchors.top: topBar.bottom;
        anchors.bottom: confirmButton.top;
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.margins: 10;
        spacing: root.height * 0.025;

        width: {
            if (UiEngine.TargetScreenType === ScreenType.Phone) {
                return parent.width * 0.8;
            } else {
                return parent.width * 0.4;
            }
        }

        ColumnLayout {
            id: radioColumn;
            width: childrenRect.width;
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: privates.itemSpacing;
            anchors.margins: 5;
            ExclusiveGroup { id: itemAccessiblityGroup;
                onCurrentChanged: {
                    if (radioPrivate.checked)
                        root.state = "private";
                    else
                        root.state = "not_private";
                }
            }
            RadioButton {
                text: qsTr("DO_NOT_SHARE") + UiEngine.EmptyLangString;
                checked: true;
                height: privates.radioHeight;
                exclusiveGroup: itemAccessiblityGroup;
                onCheckedChanged: {
                    if (checked) {
                        root.state = "not_private"
                    } else {
                        root.state = "private"
                    }
                }
                style: radioButtonStyle;
            }
            RadioButton {
                text: qsTr("PUBLIC") + UiEngine.EmptyLangString;
                checked: false;
                height: privates.radioHeight;
                exclusiveGroup: itemAccessiblityGroup;
                onCheckedChanged: {
                    if (checked) {
                        root.state = "not_private"
                    } else {
                        root.state = "private"
                    }
                }
                style: radioButtonStyle;
            }
            RadioButton {
                text: qsTr("FRIENDS") + UiEngine.EmptyLangString;
                checked: false;
                height: privates.radioHeight;
                exclusiveGroup: itemAccessiblityGroup;
                onCheckedChanged: {
                    if (checked) {
                        root.state = "not_private"
                    } else {
                        root.state = "private"
                    }
                }
                style: radioButtonStyle;
            }
            RadioButton {
                id: radioPrivate;
                text: qsTr("PRIVATE") + UiEngine.EmptyLangString;
                checked: false;
                height: privates.radioHeight;
                exclusiveGroup: itemAccessiblityGroup;
                onCheckedChanged: {
                    if (checked) {
                        root.state = "private"
                    } else {
                        root.state = "not_private"
                    }
                }
                style: radioButtonStyle;
            }
        }

        ListView {
            id: listViewContacts;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 5;
            width: UiEngine.TargetScreenType === ScreenType.Phone ? root.width : root.width * 0.5;
            anchors.top: radioColumn.bottom;
            anchors.bottom: parent.bottom;
            spacing: privates.itemSpacing;
            model: jsonModel.model;
            delegate: Component {
                Rectangle {
                    gradient: Gradient {
                        GradientStop { color: "#66ffffff"; position: 0.0 }
                        GradientStop { color: "#aaffffff"; position: 0.4 }
                        GradientStop { color: "#aaffffff"; position: 0.6 }
                        GradientStop { color: "#66ffffff"; position: 1.0 }
                    }
                    width: parent.width;
                    height: privates.itemHeight;

                    CheckBox {
                        id: contactCheckBox
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: 5
                        checked: false
                        width: privates.checkBoxSize
                        height: privates.checkBoxSize
                        style: CheckBoxStyle {
                            indicator: Rectangle {
                                implicitWidth: control.width
                                implicitHeight: control.height
                                radius: 3
                                border.color: control.activeFocus ? "darkblue" : "gray"
                                border.width: 1
                                Rectangle {
                                    visible: control.checked
                                    color: "#555"
                                    border.color: "#333"
                                    radius: 1
                                    anchors.margins: 4
                                    anchors.fill: parent
                                }
                            }
                        }
                        onCheckedChanged: {
                            if (checked) {
                                privates.selectedContacts.push(model.Id)
                            } else {
                                privates.selectedContacts.pop(model.Id)
                            }
                        }
                    }

                    Image {
                        id: contactImage
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.margins: 5
                        width: height
                        source: model.picture
                    }
                    Text {
                        id: contactText
                        anchors.left: contactImage.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: 5
                        text: model.name
                        font.pixelSize: parent.height * 0.2
                    }
                }
            }
        }
    }

    Button {
        id: confirmButton;
        style: buttonStyle;
        height: privates.buttonHeight;
        width: height * 4;
        anchors.margins: 10;
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        text: qsTr("CONFIRM") + UiEngine.EmptyLangString;
        onClicked: {
        }
    }
}

