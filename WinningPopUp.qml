import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 2.15
import QtQuick.Controls.Material 2.15

Popup {
    id: winningPopup

    property string title : "You won the game!"
    property string description : "now you can start a new one"

    parent: Overlay.overlay
    anchors.centerIn: Overlay.overlay
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    dim: true

    enter: Transition {
        ParallelAnimation {
            NumberAnimation {
                property: "opacity";
                from: 0.0;
                to: 1.0;
                duration: 300
            }
            NumberAnimation {
                property: "scale";
                from: 0.4;
                to: 1.0;
                easing.type: Easing.OutBack
                duration: 300
            }
        }
    }

    exit: Transition {
        ParallelAnimation {
            NumberAnimation {
                property: "opacity";
                from: 1.0
                to: 0.0;
                duration: 300
            }
            NumberAnimation {
                property: "scale";
                from: 1.0
                to: 0.8;
                duration: 300
            }
        }
    }

    background: null

    contentItem : Pane {
        id: pane
        Material.elevation: 10

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            layoutDirection: Qt.LeftToRight

            Text {
                text: qsTr(title)
                font.bold: true
            }

            Text {
                text: qsTr(description)
                font.weight: Font.Light
            }

            RowLayout {
                Button {
                    text: "Close";
                    onClicked: {
                        winningPopup.close();
                    }
                }
            }
        }
    }


}
