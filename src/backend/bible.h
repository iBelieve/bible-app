/*
 * Whatsoever ye do in word or deed, do all in the name of the
 * Lord Jesus, giving thanks to God and the Father by him.
 * - Colossians 3:17
 *
 * Bible app - Cross-platform Bible application using QML and Material Design
 *
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
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

#pragma once

#include "module.h"

#include <QObject>
#include <QStringList>
#include <QDebug>

#include <sword/swmgr.h>
#include <sword/swmodule.h>
#include <sword/versekey.h>


typedef QSharedPointer<sword::VerseKey> SharedVerseKeyPointer;

class Bible : public Module
{
    Q_OBJECT

    Q_PROPERTY(QStringList books READ books)

public:
    explicit Bible(QObject *parent = nullptr) : Module(parent) {}

    explicit Bible(sword::SWModule *module, bool installed, BibleManager *parent);

    ~Bible() {
        qDebug() << "Deleted!";
    }

    Q_INVOKABLE unsigned int chapterCount(const QString &book) const {
        return chapterCount(bookNumber(book));
    }

    Q_INVOKABLE unsigned int chapterCount(int book) const;

    Q_INVOKABLE unsigned int verseCount(const QString &book, int chapter) const {
        return verseCount(bookNumber(book), chapter);
    }

    Q_INVOKABLE unsigned int verseCount(int book, int chapter) const;

    Q_INVOKABLE unsigned int bookNumber(const QString &book) const;

    Q_INVOKABLE QStringList books();

    Q_INVOKABLE QString verse(const QString &book, int chapter, int verse) {
        return this->verse(bookNumber(book), chapter, verse);
    }

    Q_INVOKABLE QString verse(int book, int chapter, int verse);

    Q_INVOKABLE QStringList search(const QString &phrase);

    Q_INVOKABLE QString verse(const QString &verse);

signals:

public slots:

private:
    void initBounds();

    SharedVerseKeyPointer createKey() const;
    QString verse(sword::SWKey key);

    QStringList m_bookList;
    bool m_hasOT = false;
    bool m_hasNT = false;
    sword::VerseKey m_lowerBound;
    sword::VerseKey m_upperBound;

    bool m_boundsInitialized = false;
};
