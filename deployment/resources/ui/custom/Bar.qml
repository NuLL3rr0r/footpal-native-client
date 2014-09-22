/**
 * @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */


import QtQuick 2.3;
import QtQuick.Controls 1.2;
import ScreenTypes 1.0;

Rectangle {
    id: topBar
    width: root.width
    height: UiEngine.TargetScreenType === ScreenType.Phone ? root.height * 0.08 : root.height * 0.08
    color: "#333"
    z: 1
}
