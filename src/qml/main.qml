import Material 0.3

ApplicationWindow {
    id: app

    theme {
        primaryColor: "brown"
        accentColor: "purple"
        tabHighlightColor: "white"
    }

    visible: true

    initialPage: TabbedPage {
        title: "Bible"

        actions: [
            Action {
                iconName: 'action/search'
            },

            Action {
                iconName: 'book-open'
            },

            Action {
                iconName: 'action/settings'
            }
        ]

        Tab {
            title: "Home"
        }

        Tab {
            title: "Read"
        }

        Tab {
            title: "Study"
        }
    }
}
