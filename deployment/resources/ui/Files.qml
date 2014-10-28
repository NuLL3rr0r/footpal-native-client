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
import "scripts/settings.js" as Settings

Rectangle {
    id: root
    anchors.fill: parent;
    anchors.centerIn: parent;
    color: Settings.globalBgColor;

    RegExpValidator {
        id: folderNameRegExpValidator;
        regExp: /[A-Za-z0-9_. \-]+$/;
    }

    QtObject {
        id: privates

        property bool isInitialized: false
        property int barHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.15 : 75
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5
        property int checkBoxSize: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.05 : 25
        property int buttonSize: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property var selectedItems: new Array();
        property string testJson: "{ \"files\":" +
                                  "{ \"file\": [" +
                                  "{ \"Id\": \"1234\", \"contentType\":\"folder\", \"filename\":\"Work Pics\" }, " +
                                  "{ \"Id\": \"2345\", \"contentType\":\"folder\", \"filename\":\"Personal Pics\" }, " +
                                  "{ \"Id\": \"3456\", \"contentType\":\"file\", \"filename\":\"saves.dat\" }, " +
                                  "{ \"Id\": \"4567\", \"contentType\":\"file\", \"filename\":\"links.txt\" } " +
                                  "] } }";
    }

    Component.onCompleted: {
        // TODO: load the list of files and folder

        privates.isInitialized = true;
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
                pageLoader.setSource("qrc:///ui/Home.qml");
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("MY_FILES") + UiEngine.EmptyLangString;
            color: "white"
        }
    }

    JSONListModel {
        id: jsonModel
        json: privates.testJson
        query: "$.files.file[*]"
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

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                    }
                }

                Image {
                    id: typeIconImage
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 5
                    width: height
                    source: {
                        if (model.contentType == "file")
                            return "qrc:///img/ic_file.png";
                        else
                            return "qrc:///img/ic_folder.png";
                    }
                }
                Text {
                    id: filenameText
                    anchors.left: typeIconImage.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 5
                    text: model.filename
                    font.pixelSize: parent.height * 0.2
                }
                ExtButton {
                    id: btnFileDelete;
                    height: privates.buttonSize;
                    width: height;
                    anchors.right: parent.right;
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 5
                    defaultImage: "qrc:///img/btn_file_delete.png";
                    pressedImage: "qrc:///img/btn_file_delete_pressed.png";
                    onSignal_clicked: {
                        UiEngine.showToast("Clicked on Delete");

                        // TODO: prompt and delete the file
                    }
                }
                ExtButton {
                    id: btnFileShare;
                    height: privates.buttonSize;
                    width: height;
                    anchors.right: btnFileDelete.left;
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 5
                    defaultImage: "qrc:///img/btn_file_share.png";
                    pressedImage: "qrc:///img/btn_file_share_pressed.png";
                    onSignal_clicked: {
                        UiEngine.showToast("Clicked on Share");

                        pageLoader.setSource("qrc:///ui/FileSharing.qml");
                        // TODO: go to file sharing page
                    }
                }
                ExtButton {
                    id: btnFileMove;
                    height: privates.buttonSize;
                    width: height;
                    anchors.right: btnFileShare.left;
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 5
                    defaultImage: "qrc:///img/btn_file_move.png";
                    pressedImage: "qrc:///img/btn_file_move_pressed.png";
                    onSignal_clicked: {
                        UiEngine.showToast("Clicked on Move");

                        // TODO: mark for movement
                    }
                }
            }
        }
    }

    Bar {
        id: bottomBar
        anchors.bottom: parent.bottom
        z: 2

        Row {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            ExtButton {
                id: buttonNewFolder
                height: parent.height * 0.8;
                width: height;
                anchors.verticalCenter: parent.verticalCenter
                defaultImage: "qrc:///img/btn_bar_new_folder.png"
                pressedImage: "qrc:///img/btn_bar_new_folder_pressed.png"
                onSignal_clicked: {
                    if (newFolderBar.state == "normal") {
                        newFolderBar.state = "extended"
                    } else {
                        newFolderBar.state = "normal"
                    }
                }
            }

            ExtButton {
                id: buttonNewFile
                height: parent.height * 0.8;
                width: height;
                anchors.verticalCenter: parent.verticalCenter
                defaultImage: "qrc:///img/btn_bar_new_file.png"
                pressedImage: "qrc:///img/btn_bar_new_file_pressed.png"
                onSignal_clicked: {
                }
            }

            ExtButton {
                id: buttonPaste
                height: parent.height * 0.8;
                width: height;
                anchors.verticalCenter: parent.verticalCenter
                defaultImage: "qrc:///img/btn_bar_paste.png"
                pressedImage: "qrc:///img/btn_bar_paste_pressed.png"
                onSignal_clicked: {
                }
            }
        }
    }

    Bar {
        id: newFolderBar
        z: 1

        Behavior on y {
            NumberAnimation { duration: 200 }
        }

        state: "normal"

        states: [
            State {
                name: "normal"
                PropertyChanges { target: newFolderBar; y: root.height - height }
            },
            State {
                name: "extended"
                PropertyChanges { target: newFolderBar; y: root.height - 2 * height }
            }
        ]

        Item {
            anchors.fill: parent
            anchors.margins: 5

            TextField {
                id: folderNameTextField;
                style: textFieldStyle;
                anchors.left: parent.left;
                anchors.right: buttonAdd.left
                anchors.margins: 5
                height: parent.height
                font.pixelSize: height * 0.5;
                placeholderText: qsTr("FOLDER_NAME") + UiEngine.EmptyLangString;
                focus: true;
                validator: folderNameRegExpValidator;
            }

            Button {
                id: buttonAdd
                width: parent.width * 0.25
                anchors.right: parent.right
                height: parent.height
                anchors.margins: 5
                style: buttonStyle
                text: qsTr("ADD") + UiEngine.EmptyLangString;
                onClicked: {
                    if (!folderNameTextField.acceptableInput) {
                        UiEngine.showToast(qsTr("INVALID_FOLDER_NAME"));
                        folderNameTextField.focus = true;
                        folderNameTextField.selectAll();
                        return;
                    }

                    // TODO: create the folder

                    newFolderBar.state = "normal"
                }
            }
        }
    }
}

