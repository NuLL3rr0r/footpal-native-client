/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import QtGraphicalEffects 1.0

Rectangle {
    id: numberCircle;
    border.color: "white";
    border.width: 3;
    width: height;
    radius: width / 2;
    color: "red";

    property int value: 0;

    Text {
        anchors.centerIn: parent;
        color: "white";
        text: value;
        font.pixelSize: parent.width / 2;
    }
}
