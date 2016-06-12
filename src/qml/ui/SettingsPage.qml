import QtQuick 2.0
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Bible 0.1
import "../components"

TabbedPage {
    title: "Settings"
    actionBar.backgroundColor: "gray"

    Tab {
        title: "Preferences"

        Flickable {
            Column {
                anchors {
                    fill: parent
                    topMargin: 8
                }

                ListItem.Standard {
                    text: "Bible Version"
                    valueText: "KJV"
                }
            }
        }
    }

    Tab {
        title: "Bible Versions"

        Item {
            ListView {
                anchors.fill: parent
                model: BibleManager.availableBibles
                delegate: ListItem.Standard {
                    id: bibleListItem
                    text: modelData.name
                    selected: modelData.installed

                    property var progress

                    onClicked: progress = modelData.install()

                    secondaryItem: [
                        Icon {
                            anchors.centerIn: parent
                            color: Theme.primaryColor
                            name: "file/cloud_done"
                            visible: !progressCircle.visible && bibleListItem.selected
                        },
                        ProgressCircle {
                            id: progressCircle
                            anchors.centerIn: parent
                            visible: bibleListItem.progress != null && bibleListItem.progress.busy
                        }
                    ]
                }
            }

            Placeholder {
                id: placeholder
                visible: busy
            }

            Component.onCompleted: placeholder.progress = BibleManager.refresh(false)
        }
    }
}
