/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 * @author  Mohammad S. Babaei <info@babaei.net>
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

    RegExpValidator {
        id: folderNameRegExpValidator;
        regExp: /[A-Za-z0-9_. \-]+$/;
    }

    QtObject {
        id: privates

        property bool isInitialized: false
        property variant parentId: null;
        property variant currentId: null;
        property string movedFileId: "";
        property int barHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property int itemHeight: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.15 : 75
        property int itemSpacing: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.01 : 5
        property int checkBoxSize: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.05 : 25
        property int buttonSize: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : 40
        property var selectedItems: new Array();
        property string filesJson:
            "[                                                            " +
            "   {                                                         " +
            "      \"_id\":\"5460816f81ec8056ab7defed\",                  " +
            "      \"contentType\":\"Folder\",                            " +
            "      \"readCount\":0,                                       " +
            "      \"entityName\":\"mehdi\",                              " +
            "      \"owner\":\"54606739a0d1969d3039166e\",                " +
            "      \"access\":\"Public\",                                 " +
            "      \"__v\":0,                                             " +
            "      \"actionLog\":[                                        " +
            "         {                                                   " +
            "            \"userId\":\"54606739a0d1969d3039166e\",         " +
            "            \"action\":\"Create\",                           " +
            "            \"_id\":\"5460816f81ec8056ab7defee\",            " +
            "            \"date\":            \"2014-11-10T08:09:29.000           Z\"" +
            "         }                                                   " +
            "      ],                                                     " +
            "      \"status\":true,                                       " +
            "      \"acl\":[                                              " +
            "                                                             " +
            "      ]                                                      " +
            "   }                                                         " +
            "]                                                            "
        ;
        property variant filesObject: ({});
    }

    Component.onCompleted: {
        applicationWindow.openFileDialogAccepted.connect(root.onOpenFileDialogAccepted);
        applicationWindow.openFileDialogRejected.connect(root.onOpenFileDialogRejected);

        // TODO: load the list of files and folder

        privates.isInitialized = true;

        RestApi.onSignal_FS_GetListOfEntity.connect(
                    onSignalFS_GetListOfEntityCallback
                    );
        RestApi.onSignal_FS_CreateDirectory.connect(
                    onSignalFS_CreateDirectoryCallback
                    );
        RestApi.onSignal_FS_DeleteEntity.connect(
                    onSignalFS_DeleteEntityCallback
                    );
        RestApi.onSignal_FS_GetParentId.connect(
                    onSignalFS_GetParentIdCallback
                    );
        RestApi.onSignal_FS_MoveEntity.connect(
                    onSignalFS_MoveEntityCallback
                    );
        RestApi.onSignal_FS_Upload.connect(
                    onSignalFS_UploadCallback
                    );

        HttpFileTransfer.onSignal_Failed.connect(onDownloadUrl_Failed);
        HttpFileTransfer.onSignal_ReadyRead.connect(onDownloadUrl_ReadyRead);
        HttpFileTransfer.onSignal_DownloadProgress.connect(onDownloadUrl_DownloadProgress);
        HttpFileTransfer.onSignal_Finished.connect(onDownloadUrl_Finished);
        RestApi.onSignal_FS_DownloadUrl.connect(onSignalFS_DownloadUrlCallback);

        privates.filesJson = "[]";
        listCurrentDirectory();
    }

    Component.onDestruction: {
        applicationWindow.openFileDialogAccepted.disconnect(root.onOpenFileDialogAccepted);
        applicationWindow.openFileDialogRejected.disconnect(root.onOpenFileDialogRejected);

        RestApi.onSignal_FS_GetListOfEntity.disconnect(
                    onSignalFS_GetListOfEntityCallback
                    );
        RestApi.onSignal_FS_CreateDirectory.disconnect(
                    onSignalFS_CreateDirectoryCallback
                    );
        RestApi.onSignal_FS_DeleteEntity.disconnect(
                    onSignalFS_DeleteEntityCallback
                    );
        RestApi.onSignal_FS_GetParentId.disconnect(
                    onSignalFS_GetParentIdCallback
                    );
        RestApi.onSignal_FS_MoveEntity.disconnect(
                    onSignalFS_MoveEntityCallback
                    );
        RestApi.onSignal_FS_Upload.disconnect(
                    onSignalFS_UploadCallback
                    );
        RestApi.onSignal_FS_DownloadUrl.disconnect(onSignalFS_DownloadUrlCallback);
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
        json: privates.filesJson
        query: "*"
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
                        if(model.contentType === "Folder") {
                            privates.parentId = privates.currentId;
                            privates.currentId = model._id;
                            listCurrentDirectory();
                        }
                        else {
                            console.log("Calling DownloadUrl");
                            RestApi.fs_DownloadUrl(
                                        WS.Context.token,
                                        model._id
                                        );
                        }
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
                        if (model.contentType === "File")
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
                    text: model.entityName
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
                        RestApi.fs_DeleteEntity(
                                    WS.Context.token,
                                    model._id
                                    );
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
                        WS.Context.fileIdToShare = model._id;
                        pageLoader.setSource("qrc:///ui/FileSharing.qml");
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
                        privates.movedFileId = model._id;
                        UiEngine.showToast(qsTr("FILESHARING_MOVE") + UiEngine.EmptyLangString);
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
                id: buttonUp
                height: parent.height * 0.8;
                width: height;
                anchors.verticalCenter: parent.verticalCenter
                defaultImage: "qrc:///img/btn_bar_back.png"
                pressedImage: "qrc:///img/btn_bar_back_pressed.png"
                onSignal_clicked: {
                    privates.currentId = privates.parentId;
                    RestApi.fs_GetParentId(
                                WS.Context.token,
                                privates.currentId
                                );
                }
            }

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
                    UiEngine.openFileDialog();
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
                    if(privates.movedFileId !== "") {
                        RestApi.fs_MoveEntity(
                                    WS.Context.token,
                                    privates.movedFileId,
                                    privates.currentId
                                    );
                        privates.movedFileId = "";
                    }
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

                    RestApi.fs_CreateDirectory(
                                WS.Context.token,
                                folderNameTextField.text,
                                "Public",
                                (privates.currentId === null ? "" : privates.currentId)
                                );

                    newFolderBar.state = "normal"
                }
            }
        }
    }

    function onOpenFileDialogAccepted(path) {
        path = path.replace("file:///", "");
        console.log("OPEN FILE " + path);
        RestApi.fs_Upload(
                    WS.Context.token,
                    privates.currentId,
                    "Public",
                    path
                    );
    }

    function onOpenFileDialogRejected() {

    }

    function listCurrentDirectory() {
        if(privates.currentId == null) {
            RestApi.fs_GetListOfEntity(WS.Context.token);
        } else {
            RestApi.fs_GetListOfEntity(WS.Context.token, privates.currentId);
        }
    }

    function onDownloadUrl_Failed(localFile)
    {
        UiEngine.showToast(qsTr("Failed to download %1").arg(localFile));
    }

    function onDownloadUrl_ReadyRead(localFile)
    {

    }

    function onDownloadUrl_DownloadProgress(localFile, bytesReceived, bytesTotal)
    {
        UiEngine.showToast(qsTr("Downloading %1: %2\%").arg(localFile).arg(
                    Math.round(bytesReceived / bytesTotal * 100)));
    }

    function onDownloadUrl_Finished(localFile)
    {
        UiEngine.showToast(qsTr("Your file has been saved to %1.").arg(localFile));
    }

    function onSignalFS_DownloadUrlCallback(connectionStatus, downloadUrlStatus, response)
    {
        console.log("connection: " + connectionStatus +  ", status: " + downloadUrlStatus,
                    ", response: " + response);

        switch(downloadUrlStatus) {
        case 404:
            UiEngine.showToast(qsTr("ERROR_FS_DOWNLOADURL_404") + UiEngine.EmptyLangString);
            break;
        case 400:
            UiEngine.showToast(qsTr("ERROR_FS_DOWNLOADURL_400") + UiEngine.EmptyLangString);
            break;
        case 500:
            UiEngine.showToast(qsTr("ERROR_FS_DOWNLOADURL_500") + UiEngine.EmptyLangString);
            break;
        case 403:
            UiEngine.showToast(qsTr("ERROR_FS_DOWNLOADURL_403") + UiEngine.EmptyLangString);
            break;
        case 200:
            var parsedJson = JSON.parse(response);
            var relativeFileUrl = String(parsedJson.fileUrl);

            var url = HttpFileTransfer.getDownloadServerUrl();
            if (url.indexOf("/", url.length - 1) === -1) {
                url += "/";
            }
            url += relativeFileUrl;

            var lastPathSeparatorPosition = relativeFileUrl.lastIndexOf("/");
            var fileName = "";
            if (lastPathSeparatorPosition !== -1 && lastPathSeparatorPosition !== relativeFileUrl.length - 1) {
                fileName = relativeFileUrl.substring(lastPathSeparatorPosition + 1);
            }

            if (fileName !== "") {
                HttpFileTransfer.download(url, HttpFileTransfer.getLocalDownloadFolderPath(), fileName);
            }
            break;
        default:
            break;
        }
    }

    function onSignalFS_UploadCallback(
        connectionStatus, uploadStatus, response
        )
    {
        console.log("connection: " + connectionStatus +  ", status: " + uploadStatus,
                    ", response: " + response);
        switch(uploadStatus) {
        case 404:
            UiEngine.showToast(qsTr("ERROR_FS_UPLOAD_404") + UiEngine.EmptyLangString);
            break;
        case 400:
            UiEngine.showToast(qsTr("ERROR_FS_UPLAOD_400") + UiEngine.EmptyLangString);
            break;
        case 500:
            UiEngine.showToast(qsTr("ERROR_FS_UPLAOD_500") + UiEngine.EmptyLangString);
            break;
        case 403:
            UiEngine.showToast(qsTr("ERROR_FS_UPLOAD_403") + UiEngine.EmptyLangString);
            break;
        case 201:
            listCurrentDirectory();
            break;
        default:
            break;
        }
    }

    function onSignalFS_GetParentIdCallback(
        connectionStatus, getParentIdStatus, response
        )
    {
        console.log("connection: " + connectionStatus +  ", status: " + getParentIdStatus,
                    ", response: " + response);

        switch(getParentIdStatus) {
        case 404:
            UiEngine.showToast(qsTr("ERROR_FS_PARENTID_404") + UiEngine.EmptyLangString);
            break;
        case 400:
            UiEngine.showToast(qsTr("ERROR_FS_PARENTID_400") + UiEngine.EmptyLangString);
            break;
        case 500:
            UiEngine.showToast(qsTr("ERROR_FS_PARENTID_500") + UiEngine.EmptyLangString);
            break;
        case 200:
            privates.parentId = JSON.parse(response).parentId;
            listCurrentDirectory();
            break;
        default:
            break;
        }
    }

    function onSignalFS_MoveEntityCallback(
        connectionStatus, moveEntityStatus, response
        )
    {
        console.log("connection: " + connectionStatus +  ", status: " + moveEntityStatus,
                    ", response: " + response);

        switch(moveEntityStatus) {
        case 404:
            UiEngine.showToast(qsTr("ERROR_FS_MOVEENTITY_404") + UiEngine.EmptyLangString);
            break;
        case 400:
            UiEngine.showToast(qsTr("ERROR_FS_MOVEENTITY_400") + UiEngine.EmptyLangString);
            break;
        case 500:
            UiEngine.showToast(qsTr("ERROR_FS_MOVEENTITY_500") + UiEngine.EmptyLangString);
            break;
        case 403:
            UiEngine.showToast(qsTr("ERROR_FS_MOVEENTITY_403") + UiEngine.EmptyLangString);
            break;
        case 200:
            listCurrentDirectory();
            break;
        default:
            break;
        }
    }

    function onSignalFS_DeleteEntityCallback(
        connectionStatus, deleteEntityStatus, response
        )
    {
        console.log("connection: " + connectionStatus +  ", status: " + deleteEntityStatus,
                    ", response: " + response);
        switch(deleteEntityStatus) {
        case 404:
            UiEngine.showToast(qsTr("ERROR_FS_DELETEENTITY_404") + UiEngine.EmptyLangString);
            break;
        case 400:
            UiEngine.showToast(qsTr("ERROR_FS_DELETEENTITY_400") + UiEngine.EmptyLangString);
            break;
        case 500:
            UiEngine.showToast(qsTr("ERROR_FS_DELETEENTITY_500") + UiEngine.EmptyLangString);
            break;
        case 403:
            UiEngine.showToast(qsTr("ERROR_FS_DELETEENTITY_403") + UiEngine.EmptyLangString);
            break;
        case 200:
            listCurrentDirectory();
            break;
        default:
            break;
        }
    }

    function onSignalFS_GetListOfEntityCallback(
        connectionStatus, getListOfEntityStatus, response
        )
    {
        console.log("connection: " + connectionStatus +  ", status: " + getListOfEntityStatus,
                    ", response: " + response);
        switch(getListOfEntityStatus) {
        case 404:
            UiEngine.showToast(qsTr("ERROR_FS_GETLISTOFENTITY_404") + UiEngine.EmptyLangString);
            break;
        case 500:
            UiEngine.showToast(qsTr("ERROR_FS_GETLISTOFENTITY_500") + UiEngine.EmptyLangString);
            break;
        case 403:
            UiEngine.showToast(qsTr("ERROR_FS_GETLISTOFENTITY_403") + UiEngine.EmptyLangString);
            break;
        case 200:
            privates.filesJson = response;
            privates.filesObject = JSON.parse(response);
            break;
        default:
        }
    }

    function onSignalFS_CreateDirectoryCallback(
        connectionStatus, createDirectoryStatus, response
        )
    {
        console.log("connection: " + connectionStatus +  ", status: " + createDirectoryStatus,
                    ", response: " + response);
        switch(createDirectoryStatus) {
        case 400:
            UiEngine.showToast(qsTr("ERROR_FS_CREATEDIRECTORY_400") + UiEngine.EmptyLangString);
            break;
        case 404:
            UiEngine.showToast(qsTr("ERROR_FS_CREATEDIRECTORY_404") + UiEngine.EmptyLangString);
            break;
        case 201:
            listCurrentDirectory();
            break;
        default:
            break;
        }
    }
}

