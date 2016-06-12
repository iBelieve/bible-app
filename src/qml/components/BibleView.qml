/*
 * Whatsoever ye do in word or deed, do all in the name of the
 * Lord Jesus, giving thanks to God and the Father by him.
 * - Colossians 3:17
 *
 * Bible app - Cross-platform Bible application using QML and Material Design
 *
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import Material 0.1
import Bible 0.1

Rectangle {
    color: BibleManager.installedBibles.length == 0 ? "transparent" : "white"

    BibleChapter {
        id: bibleChapter

        bible: app.bible
    }

    Placeholder {
        iconName: "book-open"
        text: "No Bibles installed yet"
        subText: "Open the Bible manager from Settings to install a Bible."
        visible: BibleManager.installedBibles.length == 0
    }

    ListView {
        id: list

        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        width: Math.min(600, parent.width)

        topMargin: 16
        bottomMargin: 16

        model: bibleChapter
        snapMode: ListView.SnapToItem

        header: Label {
            height: implicitHeight + 16

            anchors {
                left: parent.left
                right: parent.right
                margins: 40
            }

            verticalAlignment: Text.AlignVCenter
            style: "title"
            text: bibleChapter.book + " " + bibleChapter.chapter
        }

        delegate: VerseDelegate {

        }
    }
}
