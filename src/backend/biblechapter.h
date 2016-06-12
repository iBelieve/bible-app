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

#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QDebug>

#include <sword/swmodule.h>
#include <sword/swmgr.h>

#include "bible.h"

class BibleChapter : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int chapter READ chapter WRITE setChapter NOTIFY chapterChanged)
    Q_PROPERTY(QString book READ book WRITE setBook NOTIFY bookChanged)
    Q_PROPERTY(Bible *bible READ bible WRITE setBible NOTIFY bibleChanged)

    Q_PROPERTY(QString previousChapter READ previousChapter NOTIFY previousChapterChanged)
    Q_PROPERTY(QString nextChapter READ nextChapter NOTIFY nextChapterChanged)

    enum BibleVerseRoles {
        VerseRole = Qt::UserRole + 1
    };

public:
    explicit BibleChapter(Bible *bible = nullptr, const QString &book = "Genesis", int chapter = 1, QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const {
        if (bible() == nullptr)
            return 0;

        return bible()->verseCount(book(), chapter());
    }

    virtual QHash<int,QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles[VerseRole] = "verse";
        return roles;
    }

    virtual QVariant data(const QModelIndex &index, int role) const {
        return QVariant(bible()->verse(book(), chapter(), index.row() + 1));
    }

    int chapter() const { return m_chapter; }
    QString book() const { return m_book; }
    Bible *bible() const { return m_bible; }

    QString previousChapter() const;
    QString nextChapter() const;

signals:
    void chapterChanged(int chapter);
    void bookChanged(const QString& book);
    void bibleChanged(Bible *bible);

    void previousChapterChanged(const QString& previousChapter);
    void nextChapterChanged(const QString& nextChapter);

public slots:
    void setChapter(int chapter) {
        if (chapter != m_chapter) {
            beginResetModel();
            m_chapter = chapter;
            endResetModel();

            emit chapterChanged(chapter);
            emit previousChapterChanged(this->previousChapter());
            emit nextChapterChanged(this->nextChapter());
        }
    }

    void setBook(const QString& book) {
        if (book != m_book) {
            beginResetModel();
            m_book = book;
            endResetModel();

            emit bookChanged(book);
            emit previousChapterChanged(this->previousChapter());
            emit nextChapterChanged(this->nextChapter());
        }
    }

    void setBible(Bible *bible) {
        if (bible != m_bible) {
            beginResetModel();
            m_bible = bible;
            endResetModel();

            emit bibleChanged(bible);
        }
    }

private:
    Bible *m_bible;
    QString m_book;
    int m_chapter;
};
