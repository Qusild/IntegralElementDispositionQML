import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import QtQuick.Window 2.2
Rectangle {
    id: infoBase
    border.color: "black"
    Connections{
        target: schema
    }

    signal sendChanges(string nameInfo, int typeInfo, int idInfo)
    property string nameInfo: "null"
    property int typeInfo: -1
    property int idInfo: -1
    property int elementId: 1
    property int numInfo:-1
    property string connect1Info: "none"
    property string connect2Info: "none"
    property string connect3Info: "none"
    property string connect4Info: "none"
    property string holder1: "Connection name"
    property string holder2: "Element ID"
    Connections{
        target: grid
        function onSendInfo(sendName,sendId,sendNum,sendType, connect_1, connect_2, connect_3, connect_4){
            tileInfo.nameInfo = sendName
            tileInfo.typeInfo = sendType
            tileInfo.idInfo = sendId
            tileInfo.numInfo = sendNum
            tileInfo.connect1Info = connect_1
            tileInfo.connect2Info = connect_2
            tileInfo.connect3Info = connect_3
            tileInfo.connect4Info = connect_4
        }
    }
    Item{
    id: textHolder
    anchors.fill: parent
        Text{
            id: nameDisp
            visible: false
            anchors.top: parent.top
            anchors.topMargin: parent.height/100
            anchors.horizontalCenter: parent.horizontalCenter
            text: nameInfo
        }
        Text{
            id: idDisp
            visible: false
            anchors.left: parent.left
            anchors.top: nameDisp.bottom
            anchors.topMargin: 10
            text: "ID:" + idInfo
        }
        Text{
            id: con1
            visible: (typeInfo==1)? true:false
            anchors.left: parent.left
            anchors.top: idDisp.bottom
            anchors.topMargin: 10
            text: "Сonnection to:" + connect1Info
        }
        Text{
            id: con2
            visible: (connect1Info!="none")?true:false
            anchors.left: parent.left
            anchors.top: con1.bottom
            anchors.topMargin: 10
            text: "Connection to:" + connect2Info
        }
        Text{
            id: con3
            visible: (connect2Info!="none")?true:false
            anchors.left: parent.left
            anchors.top: con2.bottom
            anchors.topMargin: 10
            text: "Connection to:" + connect3Info
        }
        Text{
            id: con4
            visible: (connect3Info!="none")?true:false
            anchors.left: parent.left
            anchors.top: con3.bottom
            anchors.topMargin: 10
            text: "Connection to:" + connect4Info
        }
    }
    Button{
        id: delElement
        visible: false
        anchors.bottom: parent.bottom
        anchors.left: addConnection.left
        anchors.right: addConnection.right
        anchors.bottomMargin: 10
        text: "Delete this element"
        onClicked: {
            schema.delElement(infoBase.idInfo)
        }
    }
    Button{
        id: addConnection
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: delElement.top
        anchors.bottomMargin: 10
        text: "Add new connection"
        onClicked:{
            newCon.open()
        }
    }
    Button{
        id: addElement
        visible: false
        anchors.bottom: delElement.top
        anchors.left: addConnection.left
        anchors.right: addConnection.right
        anchors.bottomMargin: 10
        text: "Add new element"
        onClicked: {
            newElement.open()
            idInfo = elementId
            typeInfo = 1
        }
    }
    Button{
        id: delConnection
        visible: addConnection.visible
        anchors.bottom: addConnection.top
        anchors.left: addConnection.left
        anchors.right: addConnection.right
        anchors.bottomMargin: 10
        text: "Delete connection"
        onClicked: {
            delCon.open()
        }
    }
    onNameInfoChanged: {
        nameDisp.visible = true
        idDisp.visible = true
    }
    onTypeInfoChanged: {
        addElement.visible = (typeInfo == 0)?true:false
        addConnection.visible = (typeInfo == 1)?(connect4Info==="none"):false
        delElement.visible = (typeInfo == 1)
    }
    Dialog{
       id: newElement
       anchors.centerIn: Overlay.overlay
       signal newElem(string name)
       modal:true
       title: "Name for Element"
       TextField{
           id: field
        placeholderText: numInfo
       }
       standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            (field.length<=0)?sendChanges(numInfo, 1, elementId)
                             :sendChanges(field.text, 1, elementId)
            if(field.length>0)infoBase.nameInfo = field.text
            newElement.close()
            schema.addElement(infoBase.numInfo, infoBase.numInfo)
            infoBase.update()
            field.clear()
        }
        onRejected: {
            typeInfo = 0;
            idInfo = 0;
            newElement.close()
        }
    }
    Dialog{
        modal: true
        id: newCon
            anchors.centerIn: Overlay.overlay
            title: "Connect to"
            TextField{
                id: conField
             placeholderText: holder2
            }
        standardButtons: Dialog.Ok | Dialog.Cancel

           onAccepted: {
               if(conField.length>0)
                   schema.addConnection(idInfo,conField.text)
               conField.clear()
           }

           onRejected: {

           }
     }
    Dialog{
        modal: true
        id: delCon
            anchors.centerIn: Overlay.overlay
            title: "Delete connection to"
            TextField{
                id: delconField
             placeholderText: holder2
            }
        standardButtons: Dialog.Ok | Dialog.Cancel

           onAccepted: {
               if(delconField.length>0)
                   schema.delConnection(idInfo,delconField.text)
               delconField.clear()
           }

           onRejected: {

           }
     }


}
