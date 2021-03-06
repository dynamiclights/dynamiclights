import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/stylesheet"

Field {
    id: numberField

    property string placeholder: ""
    property real defaultNum: propName && target ? target[propName] : 0

    readonly property string fieldText: fieldFocused ? defaultNum : (unit ? defaultNum + unit : defaultNum)

    property bool unsigned: false   // when true: negative values allowed
    property int type: 0            // 0 = int; 1 = real
    property real min               // minimum accepted range value
    property real max               // maximum accepted range value
    property real incStep: 1        // inc/dec widget step value
    property string unit            // non-editable suffix

    // inc/dec functions
    function increment() {
        var newNum = defaultNum + incStep;
        if (max && newNum > max) newNum = max;
        valueChanged(newNum);
    }
    function decrement() {
        var newNum = defaultNum - incStep;
        if (min && newNum < min) newNum = min;
        valueChanged(newNum);
    }

    // validators
    IntValidator {
        id: intValidator
        top: max ? max : 2147483647
        bottom: min ? min : (unsigned ? 0 : -2147483647)
    }
    DoubleValidator {
        id: doubleValidator;
        top: max ? max : Number.POSITIVE_INFINITY
        bottom: min ? min : Number.NEGATIVE_INFINITY
    }

    // this is essentially just a TextField with Int/Double validation
    // and fancy increment/decrement controls :)
    // TODO: change to SpinBox
    fieldContent: TextField {
        id: fieldInput

        // alignment
        leftPadding: 0

        // text
        text: type === 0 ? parseInt(fieldText, 10) : parseFloat(fieldText, 10)
        placeholderText: placeholder

        // background
        background: Item {}

        validator: numberField.type === 0 ? intValidator : doubleValidator

        // interactivity
        selectByMouse: true

        // signal hooks
        onEditingFinished: {
            numberField.valueChanged(type === 0 ? parseInt(text, 10) : parseFloat(text, 10));
            focus = false;
        }

        // field frame
        onHoveredChanged: fieldHovered = hovered
        onActiveFocusChanged: {
            window.editingTextField = activeFocus
            fieldFocused = activeFocus
        }

        // inc/dec widget
        RowLayout {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            spacing: 0

            Repeater {
                model: ["minus", "plus-small"]

                Item {
                    Layout.alignment: Qt.AlignVCenter
                    width: 14; height: 14
                    x: 0.5; y: -0.5

                    Image {
                        source: "qrc:/assets/images/" + modelData + ".svg"
                        opacity: mouseArea.containsMouse && !mouseArea.pressed ? 1 : 0.35
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: index % 2 === 0 ? decrement() : increment()
                    }
                }
            }
        }

        // keypress events: inc/dec with up/down arrows
        Keys.onPressed: {
            if (event.key === Qt.Key_Down) decrement();
            if (event.key === Qt.Key_Up) increment();
        }
    }
}
