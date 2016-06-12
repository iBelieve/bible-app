/*
 * All scripture is given by inspiration of God, and is profitable
 * for doctrine, for reproof, for correction, for instruction in
 * righteousness: That the man of God may be perfect, thoroughly
 * furnished unto all good works.
 * - 2 Timothy 3:16-17
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

#include "module.h"

#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>

#include <sword/swmgr.h>

#include "biblemanager.h"
#include "promise.h"

Module::Module(sword::SWModule *module, bool installed, BibleManager *parent) :
    QObject()
{
    m_module = module;
    m_manager = parent;

    moveToThread(QCoreApplication::instance()->thread());

    setInstalled(installed);
    setName(module->getName());
    setDescription(module->getDescription());
    setLanguage(module->getLanguage());
}

Progress *Module::install()
{
    return m_manager->installModule(this);
}

Progress *Module::uninstall()
{
    // return m_manager->uninstallModule(this);
    return nullptr;
}
