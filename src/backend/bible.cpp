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

#include "bible.h"

#include <QDebug>
#include <QSharedPointer>

#include <sword/gbfplain.h>
#include <swmgr.h>
#include <swmodule.h>
#include <swfilter.h>
#include <versekey.h>
#include <sword/gbfredletterwords.h>

using namespace sword;

Bible::Bible(SWModule *module, bool installed, BibleManager *parent)
    : Module(module, installed, parent)
{
    sword::GBFRedLetterWords *option = new sword::GBFRedLetterWords();
    option->setOptionValue("On");
    module->addRenderFilter(option);
}

QStringList Bible::books() {
    // Code taken from BibleTime
    if (m_bookList.size() == 0) {
        qDebug() << "Creating book list...";

        // Initialize m_hasOT and m_hasNT
        if (!m_boundsInitialized)
            initBounds();

        if (module() == nullptr) {
            qWarning("Bible not found: %s", qPrintable(name()));
            return m_bookList;
        }

        int min = 1; // 1 = OT
        int max = 2; // 2 = NT

        if (!m_hasOT)
            min++; // min == 2

        if (!m_hasNT)
            max--; // max == 1

        if (min > max) {
            qWarning("Bible (%s) no OT and no NT! Check your config!", qPrintable(name()));
        } else {
            SharedVerseKeyPointer key = createKey();
            key->setPosition(sword::TOP);

            for (key->setTestament(min); !key->popError() && key->getTestament() <= max; key->setBook(key->getBook() + 1)) {
                m_bookList.append( QString::fromUtf8(key->getBookName()) );
            }
        }
    }

    return m_bookList;
}

void Bible::initBounds() {
    // Code taken from BibleTime
    Q_ASSERT(!m_boundsInitialized);

    qDebug() << "Initializing bounds...";

    sword::SWModule *m = module();

    if (m == NULL) {
        return;
    }

    const bool oldStatus = m->isSkipConsecutiveLinks();
    m->setSkipConsecutiveLinks(true);

    m->setPosition(sword::TOP); // position to first entry
    sword::VerseKey key(module()->getKeyText());
    m_hasOT = (key.getTestament() == 1);

    m->setPosition(sword::BOTTOM);
    key = module()->getKeyText();
    m_hasNT = (key.getTestament() == 2);

    m->setSkipConsecutiveLinks(oldStatus);

    m_lowerBound = (m_hasOT ? "Genesis 1:1" : "Matthew 1:1");
    m_upperBound = (!m_hasNT ? "Malachi 4:6" : "Revelation of John 22:21");

    m_boundsInitialized = true;
}

unsigned int Bible::bookNumber(const QString &book) const {
    // Code taken from BibleTime
    unsigned int bookNumber = 0;

    qDebug() << this;
    qDebug() << "Book" << book << m_module;

    if (book == 0 || book.isEmpty() || module() == nullptr) {
        return 0;
    }

    SharedVerseKeyPointer key = createKey();
    key->setPosition(sword::TOP);

    key->setBookName(book.toUtf8().constData());

    bookNumber = ((key->getTestament() > 1) ? key->BMAX[0] : 0) + key->getBook();

    //qDebug() << book << "is number" << bookNumber;
    return bookNumber;
}

unsigned int Bible::chapterCount(int bookNumber) const {
    if (module() == nullptr)
        return 0;

    SharedVerseKeyPointer key = createKey();

    key->setBook(bookNumber);
    key->setChapter(sword::MAXCHAPTER);

    return key->getChapter();
}

unsigned int Bible::verseCount(int book, int chapter) const {
    if (module() == nullptr)
        return 0;

    SharedVerseKeyPointer key = createKey();

    key->setBook(book);
    key->setChapter(chapter);
    key->setPosition(sword::MAXVERSE);

    return key->getVerse();
}

QString Bible::verse(int book, int chapter, int verse) {
    sword::VerseKey key;
    key.setPosition(sword::TOP);
    key.setBook(book);
    key.setChapter(chapter);
    key.setVerse(verse);

    return this->verse(key);
}

QString Bible::verse(const QString &verse) {
    qDebug() << "Getting verse" << verse;

    SWKey key(qPrintable(verse));

    return this->verse(key);
}

QString Bible::verse(SWKey key)
{
    if (module() == nullptr) {
        return "No Bibles installed.";
    }

    module()->setKey(key);

    GBFRedLetterWords *option = new GBFRedLetterWords();
    option->setOptionValue("On");
    module()->addRenderFilter(option);

    QString contents(module()->renderText());

    if (contents.isEmpty())
        return "Module not supported: " + name();
    else
        return contents;
}

SharedVerseKeyPointer Bible::createKey() const {
    return SharedVerseKeyPointer(static_cast<sword::VerseKey *>(module()->createKey()));
}

QStringList Bible::search(const QString &phrase) {
    qDebug() << "Doing search...";
    sword::SWMgr *library = new sword::SWMgr();

    sword::SWModule *module = library->getModule(this->module()->getName());

    sword::ListKey searchResults = module->search(qPrintable(phrase), 2);

    qDebug() << "Setting key...";
    module->setKey(searchResults);
    module->addRenderFilter(new GBFPlain()); //search results

    QStringList results;
    for (searchResults = TOP; !searchResults.popError(); searchResults++) {
        module->setKey(searchResults);

        results.append(QString((const char *) searchResults));
    }

    qDebug() << "List: " << results;

    delete module;

    return results;
}
