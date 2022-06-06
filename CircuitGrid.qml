import QtQuick 2.0
import QtQuick.Controls 2.0
GridView {
    signal sendInfo(string sendName, int sendId, int sendNum,int sendType, string connect_1, string connect_2, string connect_3, string connect_4)
    id:root
    property int circHeight;
    property int circWidth;
    model: GridModel.width*GridModel.height
    Connections{
        target: schema
        function onSendCell(id, pos, type, name, con_1, con_2, con_3, con_4){
            root.itemAtIndex(pos).id = id
            root.itemAtIndex(pos).name = name
            root.itemAtIndex(pos).connect_1 = con_1
            root.itemAtIndex(pos).connect_2 = con_2
            root.itemAtIndex(pos).connect_3 = con_3
            root.itemAtIndex(pos).connect_4 = con_4
            root.itemAtIndex(pos).type = type
        }
        function onSendCon(pos, con){
            if (root.itemAtIndex(pos).connect_1==="none")
                root.itemAtIndex(pos).connect_1 = con
            else if (root.itemAtIndex(pos).connect_2==="none"&&root.itemAtIndex(pos).connect_1!==con)
                root.itemAtIndex(pos).connect_2 = con
            else if (root.itemAtIndex(pos).connect_3==="none"&&root.itemAtIndex(pos).connect_1!==con
                     &&root.itemAtIndex(pos).connect_2!==con)
                root.itemAtIndex(pos).connect_3 = con
            else if (root.itemAtIndex(pos).connect_4==="none"&&root.itemAtIndex(pos).connect_1!==con
                     &&root.itemAtIndex(pos).connect_2!==con&&root.itemAtIndex(pos).connect_3!==con)
                root.itemAtIndex(pos).connect_4 = con
        }

        function onUpdModel(dimensions_x,dimensions_y)
        {
           GridModel.height = dimensions_y
           GridModel.width = dimensions_x
        }
    }
    cellHeight: height/GridModel.height;
    cellWidth: width/GridModel.width;
    property int high: -1 
    Connections{
        target: tileInfo
        function onSendChanges(nameInfo, typeInfo, idInfo) {
            root.itemAtIndex(root.currentIndex).type = typeInfo
            root.itemAtIndex(root.currentIndex).name = nameInfo
            root.itemAtIndex(root.currentIndex).id = idInfo

        }
    }

    delegate:Item {
            id: elem
            property bool isRed: false
            //type 0 - empty cell
            //type 1 - element
            //type 2 - horizontal wire
            //type 3 - vertical wire
            //type 4 - top-left wire
            //type 5 - top-right wire
            //type 6 - bottom-left wire
            //type 7 - bottom-right wire
            property int type: 0
            property string name: ""
            property int id: 0
            property string connect_1: "none"
            property string connect_2: "none"
            property string connect_3: "none"
            property string connect_4: "none"
            Button{
                id : tiler
                height: cellHeight
                width: cellWidth
                visible: true
                onClicked:{
                    if (high!=-1){
                        root.itemAtIndex(high).isRed = !root.itemAtIndex(high).isRed
                    }
                    root.currentIndex = index
                    root.itemAtIndex(root.currentIndex)
                    root.high = root.currentIndex
                    console.log(root.high)
                    elem.isRed = !elem.isRed
                    root.sendInfo(elem.name, elem.id, root.currentIndex,elem.type, elem.connect_1, elem.connect_2, elem.connect_3, elem.connect_4)
                }
                background: Rectangle {
                    id: back
                    color: isRed ? "red":(parent.down ? "#bbbbbb" :
                             (parent.hovered ? "#d6d6d6" : "#f6f6f6"))
                }

                Tile {
                        anchors.fill:parent
                        anchors.margins: parent.width/100
                        Text {
                            visible: isRed? true:false
                            text:elem.name
                            anchors.fill:parent
                        }
                        Image {
                            id: tilePic
                            anchors.fill:parent
                            source: (elem.type>3)?((elem.type>5)?((elem.type==6)?"Images/type6.png":"Images/type7.png"):
                                                                  (elem.type==5)?"Images/type5.png":"Images/type4.png"):
                                                   ((elem.type==3)?"Images/type3.png":(elem.type==2)?"Images/type2.png":(elem.type==1)?"Images/type1.png":"")
                        }
                        height: root.cellHeight
                        width: root.cellWidth
                }
            }
    }
}
