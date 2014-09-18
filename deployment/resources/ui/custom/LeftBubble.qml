import QtQuick 2.3;
import QtQuick.Controls 1.1;

BorderImage {
    id: leftBubble
    property bool isSenderSelf: false
//    border { left: 55; top: 56; right: 15; bottom: 15 }
    border.left: isSenderSelf ? 13 : 50
    border.right: isSenderSelf ? 52 : 12
    border.top: 49
    border.bottom: isSenderSelf ? 7 : 8
    horizontalTileMode: BorderImage.Stretch
    verticalTileMode: BorderImage.Stretch
    source: isSenderSelf ? "qrc:///img/bubble_right_blue.png" : "qrc:///img/bubble_left_green.png"
}
