import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../components/ui"

Item {
    Layout.preferredWidth: 320
    Layout.fillHeight: true

    Rectangle {
        anchors.fill: parent

        color: Stylesheet.colors.darkGrey
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ListView {
            id: generatorList

            property int prevCount: 0

            Layout.fillWidth: true
            Layout.fillHeight: true

            orientation: Qt.Vertical
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {
                interactive: false
            }

            model: generatorModel
            delegate: GeneratorWidget {
                onClicked: {
                    window.activeGeneratorIndex = selected ? -1 : model.index
                }
            }

            // deselect when clicking in empty zone of ListView
            MouseArea {
                anchors.fill: parent
                onClicked: window.activeGeneratorIndex = -1
                z: -1
            }
        }

        // add/delete generator buttons
        ColumnLayout {
            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true
            spacing: 0

            GeneratorButton {
                id: addGenerator
                text: "Add"

                Layout.fillWidth: true

                backgroundColor: Stylesheet.colors.generator
                iconSource: "qrc:/assets/images/plus.svg"

                onClicked: window.addGenerator("SNN")
            }

            Repeater {
                model: generatorMetaModel

                Label {
                    property string type: model.type
                    text: model.name

                    Layout.fillWidth: true
                    height: 50

                    MouseArea {
                        anchors.fill: parent
                        onClicked: console.log(parent.type)
                    }
                }
            }
        }
    }
}
