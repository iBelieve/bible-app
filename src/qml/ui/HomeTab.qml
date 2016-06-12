import QtQuick 2.0
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Bible 0.1
import "../components"

Tab {
    iconName: 'action/home'
    title: "Home"

    Item {
        View {
            anchors.centerIn: parent

            radius: 2
            elevation: 1

            width: Math.min(parent.width, 300)
            height: column.implicitHeight + 2 * column.anchors.margins

            Column {
                id: column

                anchors {
                    fill: parent
                    margins: 16
                }

                spacing: 8

                Label {
                    id: titleLabel
                    text: 'John 3:16'
                    style: "title"
                }

                Label {
                    text: bible.verse(titleLabel.text)
                    textFormat: Text.StyledText
                    wrapMode: Text.Wrap
                    style: "subheading"
                    width: parent.width
                }
            }
        }
    }
}
