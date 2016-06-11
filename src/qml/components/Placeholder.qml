import QtQuick 2.0
import QtQuick.Layouts 1.0
import Material 0.3

ColumnLayout {
    anchors.centerIn: parent

    width: parent.width - 32

    property alias iconName: icon.name
    property alias iconSource: icon.source
    property alias text: textLabel.text
    property alias subText: subTextLabel.text

    Icon {
        id: icon
        Layout.alignment: Qt.AlignHCenter
        size: 64
        color: Theme.light.disabledColor
    }

    Item {
        Layout.preferredHeight: 8
    }

    Label {
        id: textLabel

        Layout.preferredWidth: parent.width

        visible: text != ""
        style: "title"
        color: Theme.light.disabledColor
        horizontalAlignment: Text.AlignHCenter
    }

    Label {
        id: subTextLabel

        Layout.preferredWidth: parent.width

        visible: text != ""
        style: "subheading"
        color: Theme.light.disabledColor
        horizontalAlignment: Text.AlignHCenter
    }
}
