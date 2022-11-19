import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

ApplicationWindow {
    id: window
    title: qsTr("15(ish) puzzle Game")
    visible: true
    width: 500
    height: 570

    background: Rectangle {
        color: "white"
    }

    PlayBoard {
        id: board
        width: window.width
        height: window.height - 70
    }

    Footer {
        anchors{
            top: board.bottom
            left: window.left
            right: window.right
            bottom: window.bottom
        }
    }
}
