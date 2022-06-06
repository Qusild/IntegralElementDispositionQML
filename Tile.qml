import QtQuick 2.0

Rectangle {
    id: tile
    color: "lightgreen"
    property string  displayType: ""
    border.color:"black"


    Text {
        id: tyleType
        text: tile.displayType
    }
}
