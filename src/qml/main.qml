import QtQuick 2.0
import Material 0.3
import Material.ListItems 0.1 as ListItem
import "components"
import Bible 0.1

ApplicationWindow {
    id: app

    theme {
        primaryColor: "brown"
        accentColor: "purple"
        tabHighlightColor: "white"
    }

    visible: true

    Component.onCompleted: BibleManager.confirmedPermission = true

    initialPage: TabbedPage {
        title: "Bible"

        actions: [
            Action {
                iconName: 'action/search'
                text: 'Search'
            },

            Action {
                iconName: 'book-open-page'
                text: 'Go to Verse'
            },

            Action {
                iconName: 'action/list'
                text: 'Quick Reference'
            },

            Action {
                iconName: 'action/settings'
                text: 'Settings'

                onTriggered: pageStack.push(Qt.resolvedUrl("ui/SettingsPage.qml"))
            }
        ]

        Tab {
            iconName: 'action/home'
            title: "Home"
        }

        Tab {
            title: "Read"
            iconName: 'book-open-page'

            BibleView {

            }
        }

        Tab {
            title: "Study"
            iconName: 'social/school'

            Item {
                Sidebar {
                    Column {
                        anchors.fill: parent

                        Item {
                            width: parent.width
                            height: 8
                        }

                        ListItem.Standard {
                            text: "Notes"
                        }

                        ListItem.Standard {
                            text: "Highlighted Verses"
                        }
                    }
                }
            }
        }
    }
}
