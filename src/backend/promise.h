/*
 * Trust in the Lord with all thine heart; and lean not unto thine own
 * understanding. In all thy ways acknowledge him, and he shall direct
 * thy paths.
 * - Proverbs 3:5-6
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

#pragma once

#include <QObject>

#include <QtConcurrent/QtConcurrentRun>
#include <QFutureWatcher>

class Promise {

public:
    template<typename Functor>
    void done(Functor function)
    {
        QFutureWatcher<void> *watcher = new QFutureWatcher<void>();

        QObject::connect(watcher, &QFutureWatcher<void>::finished, function);
        watcher->setFuture(future);
    }

    template<typename Functor>
    static Promise when(Functor function)
    {
        return Promise(function);
    }

private:
    template<typename Functor>
    explicit Promise(Functor function) {
        future = QtConcurrent::run(function);
    }

    QFuture<void> future;
};
