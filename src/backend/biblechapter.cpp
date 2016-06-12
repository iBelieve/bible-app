/*
 * Whatsoever ye do in word or deed, do all in the name of the
 * Lord Jesus, giving thanks to God and the Father by him.
 * - Colossians 3:17
 *
 * uBible - Bible application for Ubuntu Touch
 * Copyright (C) 2013 The uBible Developers.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "biblechapter.h"

#include <QDebug>

BibleChapter::BibleChapter(Bible *bible, const QString &book, int chapter, QObject *parent) :
    QAbstractListModel(parent),
    m_bible(bible), m_book(book), m_chapter(chapter)
{
    // Nothing needed here
}

QString BibleChapter::nextChapter() const
{
    if (m_bible == nullptr || m_bible->name().isEmpty())
        return QString();

    uint chapter = m_chapter;
    QString book = m_book;

    if (chapter < bible()->chapterCount(book)) {
        chapter++;
    } else {
        // Next chapter is the first chapter of the next book
        QStringList books = bible()->books();
        int currentIndex = books.indexOf(book);

        if (currentIndex >= books.length() - 1)
            return QString();

        book = books[currentIndex + 1];
        chapter = 1;
    }

    return QString("%1 %2").arg(book).arg(chapter);
}

QString BibleChapter::previousChapter() const
{
    if (m_bible == nullptr || m_bible->name().isEmpty())
        return QString();

    int chapter = m_chapter;
    QString book = m_book;

    if (chapter > 1) {
        chapter--;
    } else {
        // Previous chapter is last chapter in the previous book
        QStringList books = bible()->books();
        int currentIndex = books.indexOf(book);

        if (currentIndex == 0)
            return QString();

        book = books[currentIndex - 1];
        chapter = bible()->chapterCount(book);
    }

    return QString("%1 %2").arg(book).arg(chapter);
}
