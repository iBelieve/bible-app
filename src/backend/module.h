/*
 * There is not a square inch in the whole domain of our human existence
 * over which Christ, who is Sovereign over all, does not cry: "Mine!"
 * - Abraham Kuyper
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

#include <QObject>
#include <QDebug>

#include <sword/swmodule.h>
#include <sword/swmgr.h>
#include <sword/installmgr.h>

#include "progress.h"

class BibleManager;

class Module : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString language READ language NOTIFY languageChanged)
    Q_PROPERTY(QString sourceName READ sourceName NOTIFY sourceNameChanged)
    Q_PROPERTY(bool installed READ installed NOTIFY installedChanged)

public:
    explicit Module(QObject *parent = nullptr) : QObject(parent) {}

    explicit Module(sword::SWModule *module, bool installed, BibleManager *parent);

    QString name() const { return m_name; }
    QString description() const { return m_description; }
    QString language() const { return m_language; }
    QString sourceName() const { return QString(m_source->caption); }

    bool installed() const { return m_installed; }

    sword::SWModule *module() const { return m_module; }
    sword::InstallSource *source() const { return m_source; }

signals:
    void nameChanged(QString arg);
    void descriptionChanged(QString arg);
    void languageChanged(QString arg);
    void sourceNameChanged(QString arg);

    void installedChanged(bool arg);

public slots:
    Progress *install();
    Progress *uninstall();

    void setName(QString arg)
    {
        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }
    void setDescription(QString arg)
    {
        if (m_description != arg) {
            m_description = arg;
            emit descriptionChanged(arg);
        }
    }
    void setInstalled(bool arg)
    {
        if (m_installed != arg) {
            m_installed = arg;
            emit installedChanged(arg);
        }
    }

    void setLanguage(QString arg)
    {
        if (m_language != arg) {
            m_language = arg;
            emit languageChanged(arg);
        }
    }

    void setSource(sword::InstallSource *source)
    {
        if (m_source != source) {
            m_source = source;
            emit sourceNameChanged(QString(source->caption));
        }
    }

protected:
    sword::SWModule *m_module = nullptr;

private:
    BibleManager *m_manager = nullptr;

    QString m_name = "";
    QString m_description = "";
    QString m_language = "";
    sword::InstallSource *m_source = nullptr;

    bool m_installed = false;
};
