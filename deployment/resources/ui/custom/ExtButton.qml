/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;

Image {
    id: extButton

    property string defaultImage: "qrc:///img/Splash.png"
    property string pressedImage: "qrc:///img/Splash.png"

    signal signal_clicked();
    signal signal_longClicked();

    source: defaultImage;
    MouseArea {
        anchors.fill: parent
        onPressed: extButton.source = pressedImage;
        onReleased: extButton.source = defaultImage;
        onClicked: signal_clicked();
        onPressAndHold: signal_longClicked();
    }
}
