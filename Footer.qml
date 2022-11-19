import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Rectangle{
    id: footer
    height: 70
    color: "transparent"

    ColumnLayout
    {
        anchors.fill: parent
        RowLayout {
            Layout.alignment: Qt.AlignHCenter

            Button {
                text: "New Game"
                onClicked: {
                    GameModel.boardsize = difficultySpinBox.value
                    GameModel.newGame();
                }
            }

            Button {
                text: "Reset"
                onClicked: { GameModel.reset() }
            }

            Item { // spacer item
                Layout.fillWidth: true
            }

            Text {
                text:qsTr("difficulty:")
            }

            SpinBox {
                id: difficultySpinBox
                from: 2
                to: 10
                value: 4
            }
        }

        Text {
            text:qsTr("Start a new game to change difficulty")
            font.pixelSize : 10
            visible: GameModel.boardsize !== difficultySpinBox.value
        }
    }
}
