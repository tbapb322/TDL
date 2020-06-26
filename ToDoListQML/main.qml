import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import ToDo 1.0


ApplicationWindow {
    id: root
    visible: true
    width: 600
    height: 500

    Material.theme: Material.Dark
    Material.accent: Material.Blue

    Popup{
        id: detailsPopup
        width: parent.width*0.95
        height: parent.height*0.95
        anchors.centerIn: parent

        property int detailsNumber

        modal: true
        focus: true

        contentItem:  ColumnLayout{
            anchors.fill: detailsPopup
            Button{
                Layout.fillWidth : true
                onClicked: {
                    toDoList.setItemDetails(detailsPopup.detailsNumber, detailsTextArea.text)
                    toDoList.saveList()
                    detailsPopup.close()
                }
                id: closeButton
                text: "To List"
            }

            Label{
                text: "Details"
                font.pointSize: 18
                font.bold: true
                font.underline: true
                Layout.alignment: Qt.AlignCenter
            }

            ScrollView{
                ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                ScrollBar.vertical.policy: ScrollBar.AsNeeded
                Layout.fillHeight: true
                Layout.fillWidth: true
                TextArea {
                    id: detailsTextArea
                    wrapMode: TextArea.WrapAtWordBoundaryOrAnywhere
                    placeholderText: "Tap to enter"
                    font.pointSize: 16
                }
            }
        }
    }

    ColumnLayout
    {
        spacing: 1
        anchors.fill: parent

        RowLayout {
            spacing: 1
            Layout.alignment: Qt.AlignTop
            TextField {
                id: enterField
                Layout.fillWidth: true
                Layout.margins: 10
                font.pointSize: 16
                placeholderText: qsTr("Enter name")
            }

            Button {
                id: addButton
                text: "Add Item"
                onClicked: {
                    toDoList.addItem(enterField.text)
                    toDoList.saveList()
                    enterField.clear()
                }
                Layout.rightMargin: 10
             }
        }

        ListView
        {
            id: listView
            clip: true
            model: ToDoModel{
                list: toDoList
            }
            Layout.fillHeight: true
            Layout.fillWidth: true

            delegate: RowLayout{
                anchors.left: parent.left
                anchors.right: parent.right
                Layout.margins: 10

                CheckBox
                {
                    Layout.leftMargin: 15
                    checked: model.complete
                    onClicked: {
                        model.complete = checked
                        toDoList.saveList()
                    }
                }

                Text {
                    text: model.description
                    font.pointSize: 18
                    color: "white"
                    Layout.fillWidth: true
                }

                Button{
                    Layout.rightMargin: 10
                    text: "Details"
                    onClicked: {
                        detailsPopup.open()
                        detailsTextArea.text = model.details
                        detailsPopup.detailsNumber = model.index
                    }
                }
            }
        }

        Button {
            text: "Remove Completed"
            onClicked: {
                toDoList.removeCompletedItems()
                toDoList.saveList()
            }
            Layout.fillWidth: true
            Layout.margins: 10
        }
}
}
