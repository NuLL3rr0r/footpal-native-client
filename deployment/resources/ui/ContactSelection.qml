/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.1;
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

    QtObject {
        id: privates

        property bool isInitialized: false
        property int barHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.2 : 100
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5
        property int checkBoxSize: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.05 : 25
        property int buttonHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
    }

    Component.onCompleted: {
        privates.isInitialized = true;
    }

    Rectangle {
        id: topBar
        width: root.width
        height: privates.barHeight
        color: "#333"
        z: 1

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
            text: qsTr("SELECT_CONTACTS") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    JSONListModel {
        id: jsonModel
        json: "{ \"friends\" : { \"contact\" : [ "
              + "{ \"name\" : \"Majid\", \"picture\" : \"qrc:///img/ic_contact.png\" }, "
              + "{ \"name\" : \"Sadegh\", \"picture\" : \"qrc:///img/ic_contact.png\" }, "
              + "{ \"name\" : \"Morteza\", \"picture\" : \"qrc:///img/ic_contact.png\" }"
              + " ] } }"
        query: "$.friends.contact[*]"
    }

    ListView {
        anchors.top: topBar.bottom
        anchors.bottom: bottomBar.top
        anchors.bottomMargin: 5
        width: parent.width
        spacing: privates.itemSpacing
        model: jsonModel.model;
        delegate: Component {
            Rectangle {
                gradient: Gradient {
                    GradientStop { color: "#66ffffff"; position: 0.0 }
                    GradientStop { color: "#aaffffff"; position: 0.4 }
                    GradientStop { color: "#aaffffff"; position: 0.6 }
                    GradientStop { color: "#66ffffff"; position: 1.0 }
                }
                width: parent.width
                height: privates.itemHeight

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

    Rectangle {
        id: bottomBar
        width: root.width
        height: privates.buttonHeight + 16
        anchors.bottom: parent.bottom
        color: "#333"
        z: 1

        Button {
            id: btnContinue
            width: UiEngine.TargetScreenType === ScreenType.Phone ? root.width * 0.4 : root.width * 0.4
            height: privates.buttonHeight
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            style: buttonStyle
            text: qsTr("CONTINUE") + UiEngine.EmptyLangString;
        }
    }
}

