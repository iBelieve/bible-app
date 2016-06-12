/*
 * There is not a square inch in the whole domain of our human existence
 * over which Christ, who is Sovereign over all, does not cry: "Mine!"
 * - Abraham Kuyper
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

#include "biblemanager.h"

#include <QVariantList>
#include <QVariantMap>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QStandardPaths>
#include <swlog.h>
#include <QFile>

#include "module.h"
#include "bible.h"
#include "promise.h"

using namespace sword;

class CustomInstallManager: public InstallMgr
{
public:
    explicit CustomInstallManager(BibleManager *manager, QString configPath)
        : InstallMgr(qPrintable(configPath)) {
        m_manager = manager;
    }

    virtual bool isUserDisclaimerConfirmed() const {
        return m_manager->confirmedPermission();
    }

private:
    BibleManager *m_manager;
};

BibleManager::BibleManager(QObject *parent) :
    QObject(parent)
{
    SWLog::getSystemLog()->setLogLevel(SWLog::LOG_DEBUG);

    QString dataPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QString configPath = dataPath + "/Databases/library/";

    qDebug() << configPath << QDir(configPath).mkpath("modules");
    qDebug() << QDir(configPath).mkpath("mods.d");

    m_manager = new SWMgr(qPrintable(configPath));

    if (m_manager->config == nullptr) {
        qFatal("SWORD configuration file not found!");
    }

    m_installManager = new CustomInstallManager(this, dataPath + "/remote_sources");

    this->loadInstalledBibles();
}

BibleManager::~BibleManager() {
    delete m_installManager;
    delete m_manager;
}

Progress *BibleManager::refresh(bool force) {
    setAvailableBibles(QVariantList());

    if (force || m_installManager->sources.size() == 0) {
        Progress *progress = Progress::begin("Refreshing sources...");

        Promise::when([this, progress] {
            loadRemoteSources(progress);
        }).done([this, progress] {
            loadFromRemoteSources();

            progress->finish("Done!");
        });

        return progress;
    } else {
        loadFromRemoteSources();

        return Progress::done();
    }
}

Bible *BibleManager::getBible(const QString &name)
{
    SWModule *module = m_manager->getModule(qPrintable(name));

    if (module == nullptr)
        return nullptr;

    return new Bible(module, true, this);
}

Progress *BibleManager::installModule(Module *module) {
    Progress *progress = Progress::begin("Installing " + module->name());

    Promise::when([this, module, progress] {
        InstallSource *source = module->source();
        int error = m_installManager->installModule(m_manager, 0, qPrintable(module->name()),
                                                    source);

        if (error) {
            qDebug() << "Error installing module: [" << module->name() << "] (write permissions?)\n";

            progress->finish("Failure :(");
        } else {
            module->setInstalled(true);
            progress->finish("Installed!");
        }
    }).done([this] {
        loadInstalledBibles();
    });

    return progress;
}

void BibleManager::loadRemoteSources(Progress *progress) {
    int result = m_installManager->refreshRemoteSourceConfiguration();

    if (result == -1)
        return;

    InstallSourceMap sources = m_installManager->sources;

    for (std::pair<SWBuf, InstallSource *> pair : sources) {
        progress->update("Refreshing " + QString(pair.second->caption));

        m_installManager->refreshRemoteSource(pair.second);
    }
}

void BibleManager::loadFromRemoteSources() {
    QVariantList list;
    InstallSourceMap sources = m_installManager->sources;

    for (std::pair<SWBuf, InstallSource *> pair : sources) {
        qDebug() << pair.second->caption;
        SWMgr *source = pair.second->getMgr();

        std::map<SWModule *, int> mods = InstallMgr::getModuleStatus(*m_manager, *source);
        for (std::pair<SWModule *, int> modPair : mods) {
            SWModule *module = modPair.first;
            QString version = module->getConfigEntry("Version");

            // It's not a Bible, let's skip it for now
            if (strcmp(module->getType(), "Biblical Texts") != 0)
                continue;

            bool installed = !(modPair.second & InstallMgr::MODSTAT_NEW);
            Module *bible = new Module(module, installed, this);
            bible->setSource(pair.second);

            list.append(QVariant::fromValue(bible));
        }
    }

    setAvailableBibles(list);
}

void BibleManager::loadInstalledBibles() {
    QVariantList list;

    m_manager->Load();

    ModMap modules = m_manager->Modules;
    ModMap::iterator it;
    SWModule *module = 0;

    for (it = modules.begin(); it != modules.end(); it++) {
        module = (*it).second;

        if (!strcmp(module->getType(), "Biblical Texts")) {
            Bible *bible = new Bible(module, true, this);

            list.append(QVariant::fromValue(bible));
        } else if (!strcmp(module->getType(), "Commentaries")) {
            // do something with module
        } else if (!strcmp(module->getType(), "Lexicons / Dictionaries")) {
            // do something with module
        }
    }

    qDebug() << list;

    setInstalledBibles(list);
}
