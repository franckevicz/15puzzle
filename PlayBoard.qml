import QtQuick 2.15

Rectangle {

    GridView {
        id: root
        interactive: false
        implicitHeight: parent.height
        implicitWidth: parent.width
        cellWidth: root.width / GameModel.boardsize
        cellHeight: root.height / GameModel.boardsize

        model: GameModel

        delegate: Item {
            width: root.cellWidth
            height: root.cellHeight

            Rectangle {
                id: tile
                anchors.fill: parent
                anchors.margins: 3
                color : "#ececec"
                radius: 3
                visible: model.value !== 0

                Text {
                    color: "#343434"
                    text: model.value
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (!GameModel.canPress(model.value))
                            anim.start()
                        GameModel.swapItemWithZero(index)
                    }
                }

                SequentialAnimation {
                    id: anim
                    PropertyAnimation {
                        target: tile
                        property: "color"
                        to: "#ffdcdc"
                        duration: 200

                    }
                    PropertyAnimation {
                        target: tile
                        property: "color"
                        to: "#ececec"
                        duration: 200

                    }
                }
            }
        }
    }

    WinningPopUp {
        id: winningPopup
        visible: GameModel.gameFinished
        width: parent.width * 0.8
        height: parent.height * 0.2
    }
}
