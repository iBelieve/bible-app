import QtQuick 2.0
import Material 0.3
import Material.ListItems 0.1 as ListItem

ListItem.BaseListItem {
    id: verseDelegate

    readonly property int verseNumber: index + 1
    readonly property string verse: model.verse

    height: verseLabel.height + 2 * verseLabel.anchors.topMargin

    Label {
        id: numberLabel

        anchors {
            left: parent.left
            top: verseLabel.top
            leftMargin: 8
        }

        text: verseNumber
        font.bold: true
        style: "subheading"
        color: Theme.light.subTextColor
    }

    // Icon {
    //     anchors {
    //         left: parent.left
    //         leftMargin: units.gu(0.5)
    //         verticalCenter: number.verticalCenter
    //     }
    //
    //     visible: bookmarked
    //     color: textColor
    //     width: units.gu(2)
    //     height: width
    //     name: "action/star"
    // }

    Label {
        id: verseLabel

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            leftMargin: 40
            rightMargin: 16
            topMargin: 4
        }

        wrapMode: Text.Wrap

        text: verseDelegate.verse
        textFormat: Text.StyledText
        enabled: false
        style: "subheading"
    }
}
