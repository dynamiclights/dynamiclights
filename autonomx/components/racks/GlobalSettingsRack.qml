import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"
import "../fields"

ColumnLayout {
    id: globalSettingsRack

    Layout.fillWidth: true
    spacing: 0

    signal startedGenerating()
    signal finishedGenerating()

    SubRack {
        titleBarVisible: false

        ColumnLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 30
            Layout.rightMargin: 30
            Layout.topMargin: 20
            Layout.bottomMargin: 30
            spacing: 30

            ColumnLayout {
                spacing: 10

                Label {
                    id: generatorName
                    text: generatorModel.at(window.activeGeneratorIndex) ? generatorModel.at(window.activeGeneratorIndex)["name"] : ""
                    font: Stylesheet.fonts.title

                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 1
                        color: parent.color
                    }
                }

                Label {
                    id: generatorDescription
                    text: generatorModel.at(window.activeGeneratorIndex) ? generatorModel.at(window.activeGeneratorIndex)["description"] : ""
                    font: Stylesheet.fonts.text
                }
            }

            AreaField {
                fieldWidth: -1
                Layout.fillWidth: true
                labelText: "User notes"
                placeholder: "add anything relevant to your setup here!"
                propName: "userNotes"
            }
        }
    }

    // TODO: adapt to target multiple generators
    Repeater {
        model: metaModel ? Object.keys(metaModel.helpRacks) : []
        onModelChanged: {
            globalSettingsRack.startedGenerating();
            //delay(100, globalSettingsRack.finishedGenerating);
        }

        function delay(time, cb) {
            let timer = Qt.createQmlObject("import QtQuick 2.9; Timer {}", window);
            timer.interval = time;
            timer.repeat = false;
            timer.triggered.connect(cb);
            timer.start();
        }

        // documentation rack (collapsed by default)
        SubRack {
            subRackTitle: modelData.slice(3)
            collapsed: true

            fields: [
                Label {
                    width: parent.width

                    text: metaModel.helpRacks[modelData]
                    textFormat: Text.StyledText
                    wrapMode: Text.WordWrap
                    font: Stylesheet.fonts.text
                    lineHeight: 1.15

                    // cursor shape events on link hover
                    onLinkHovered: {
                        if (link.length > 0) overrideCursor(Qt.PointingHandCursor);
                        else restoreCursor();
                    }

                    // open URL on link click
                    onLinkActivated: Qt.openUrlExternally(link)
                }
            ]
        }
    }
}
