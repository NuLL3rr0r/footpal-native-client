/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


import QtQuick 2.3;
import QtGraphicalEffects 1.0;

Rectangle {
    anchors.centerIn: parent;
    anchors.fill: parent;
    color: "#203070";

    Component.onCompleted: {
        splashTimer.start();
    }

    Image {
        anchors.centerIn: parent;
        cache: false;
        fillMode: Image.PreserveAspectFit;
        mipmap: true;
        smooth: true;
        source: "qrc:///img/Splash.png";
        width: {
            if (parent.width <= parent.height) {
                return (parent.width / 1920.0) * 520.0;
            } else {
                return (parent.height / 1920.0) * 520.0;
            }
        }
    }

    Timer {
        id: splashTimer;
        interval: 1500;
        repeat: false;
        running: false;
        triggeredOnStart: false;
        onTriggered: {
            pageLoader.setSource("SignIn.qml");
        }
    }
}

