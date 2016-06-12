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

#include <QObject>

#include <sword/swmgr.h>
#include <sword/swmodule.h>
#include <sword/installmgr.h>

#include "bible.h"
#include "progress.h"

class BibleManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList installedBibles READ installedBibles NOTIFY installedBiblesChanged)

    Q_PROPERTY(bool confirmedPermission READ confirmedPermission WRITE setConfirmedPermission NOTIFY confirmedPermissionChanged)

    Q_PROPERTY(QVariantList availableBibles READ availableBibles NOTIFY availableBiblesChanged)

public:
    explicit BibleManager(QObject *parent = 0);

    virtual ~BibleManager();

    QVariantList installedBibles() const { return m_installedBibles; }

    bool confirmedPermission() const { return m_confirmedPermission; }

    QVariantList availableBibles() const { return m_availableBibles; }

    Q_INVOKABLE Bible *getBible(const QString &name);

signals:
    void installedBiblesChanged(QVariantList arg);

    void confirmedPermissionChanged(bool arg);

    void availableBiblesChanged(QVariantList arg);

public slots:
    Progress *installModule(Module *module);

    Progress *refresh(bool force = false);

    void loadInstalledBibles();

    void setConfirmedPermission(bool arg)
    {
        if (m_confirmedPermission != arg) {
            m_confirmedPermission = arg;
            emit confirmedPermissionChanged(arg);
        }
    }

    void setAvailableBibles(QVariantList arg)
    {
        if (m_availableBibles != arg) {
            m_availableBibles = arg;
            emit availableBiblesChanged(arg);
        }
    }

protected slots:
    void setInstalledBibles(QVariantList installedBibles) {
        m_installedBibles = installedBibles;
        emit installedBiblesChanged(installedBibles);
    }

private:
    void loadFromRemoteSources();
    void loadRemoteSources(Progress *progress);

    QVariantList m_installedBibles;

    sword::InstallMgr *m_installManager;
    sword::SWMgr *m_manager;
    bool m_confirmedPermission;
    bool m_busy;
    QString m_status;
    QVariantList m_availableBibles;
};
