import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.0
import QtQuick.Dialogs

Window {
    id: window
    width: 1020
    height: 480
    //visibility: "FullScreen"
    Connections{
        target: schema
    }
    visible: true
    title: qsTr("Circuit Helper")
    Button {
        id: loadButton
        text: "Open File"
        width: Math.max(mainFloor.width/6,100)
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 5
        onClicked: fileDialog.open()
    }
    Button {
        id: newButton
        text: "New file"
        width: Math.max(mainFloor.width/6,100)
        anchors.right: parent.right
        anchors.top: loadButton.bottom
        anchors.topMargin: 5
        onClicked: {
            grid.enabled = true
            grid.visible = true
            grid.update();
            filler.visible = false;
            schema.writeFile("New Blueprint.xml")
            schema.updateGrid()
        }
    }
    Button {
        id: saveButton
        text: "Save file"
        width: Math.max(mainFloor.width/6,100)
        anchors.right: parent.right
        anchors.top: newButton.bottom
        anchors.topMargin: 5
        onClicked: {
            grid.enabled = true
            grid.visible = true
            grid.update();
            filler.visible = false;
            fileSave.open()
        }
    }
    Button {
        id: startButton
        text: "Start generation"
        width: Math.max(mainFloor.width/6,100)
        anchors.right: parent.right
        anchors.top: saveButton.bottom
        anchors.topMargin: 5
        onClicked: {
            schema.startUpdate()
        }
    }

    Item {
        id: mainFloor
        anchors.fill: parent
        anchors.rightMargin: 20
        Text{
            id: filler
            anchors.centerIn: parent
            anchors.rightMargin: Math.max(100, parent.width/5)
            text: "Load file or create a new one"
        }
        CircuitGrid{
            id: grid
            /////////
            enabled: false//ИЗНАЧАЛЬНО СЕТКА ОТКЛЮЧЕНА
            visible: false
            /////////
            anchors.fill:parent
            anchors.rightMargin: Math.max(100, parent.width/5); //Изменение ширины правого отступа

        }
    }
    Item{
        id: tileInfoHandler
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: parent.height*2/3
        width: Math.max(mainFloor.width/6,100)
        TileInfo{
            id: tileInfo
            anchors.fill: parent
        }
    }

    //FileDialog
    FileDialog {
        id: fileDialog
        title:"Please Choose a file"
        signal startDecypher(string filename)
        nameFilters: ["Circuit Blueprint(*xml)" ]
        onAccepted: {
            grid.enabled = true
            grid.visible = true
            filler.visible = false
            grid.update();
            console.log(currentFile)
            schema.readFile(currentFile)
        }
        onRejected: {
            grid.update()
            filler.visible = true
            grid.enabled = false
            grid.visible = false
        }
    }
    FileDialog {
        id: fileSave
        title:"Please Choose a file or create a new one"
        nameFilters: ["Circuit Blueprint(*xml)" ]
        onAccepted: {
            grid.enabled = true
            grid.visible = true
            filler.visible = false
            grid.update();
            console.log(currentFile)
            schema.writeFile(currentFile)
        }
        onRejected: {
            grid.update()
            filler.visible = true
            grid.enabled = false
            grid.visible = false
        }
    }
}
