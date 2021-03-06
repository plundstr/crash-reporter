/*
 * This file is part of crash-reporter
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Ville Ilvonen <ville.p.ilvonen@nokia.com>
 * Author: Riku Halonen <riku.halonen@nokia.com>
 *
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Jakub Adam <jakub.adam@jollamobile.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

// System includes.

#include <QFile>
#include <QDir>
#include <QDebug>

// User includes.

#include "creportersettingsbase.h"
#include "creporterprivacysettingsmodel.h"
#include "creporternamespace.h"
#include "../coredir/creportercoreregistry.h"

// Pointer to this class.
CReporterPrivacySettingsModel* CReporterPrivacySettingsModel::sm_Instance = 0;

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::instance
// ----------------------------------------------------------------------------
CReporterPrivacySettingsModel* CReporterPrivacySettingsModel::instance()
{
    if (sm_Instance == 0) {
        // Get default settings, if not overriden by the user.
        QString home = QDir::homePath();
        if (!QFile::exists(home + CReporter::PrivacySettingsFileUser)) {
            QDir dir;
            dir.mkpath(home + CReporter::UserSettingsLocation);
            QFile::copy(CReporter::PrivacySettingsFileSystem, home + CReporter::PrivacySettingsFileUser);
        }

        sm_Instance = new CReporterPrivacySettingsModel();
    }
    return sm_Instance;
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::freeSingleton
// ----------------------------------------------------------------------------
void CReporterPrivacySettingsModel::freeSingleton()
{
    if (sm_Instance != 0) {
        delete sm_Instance;
        sm_Instance = 0;
    }
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::CReporterPrivacySettingsModel
// ----------------------------------------------------------------------------
CReporterPrivacySettingsModel::CReporterPrivacySettingsModel() :
        CReporterSettingsBase("crash-reporter-settings", "crash-reporter-privacy")
{

}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::CReporterPrivacySettingsModel
// ----------------------------------------------------------------------------
CReporterPrivacySettingsModel::~CReporterPrivacySettingsModel()
{

}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::coreDumpingEnabled
// ----------------------------------------------------------------------------
bool CReporterPrivacySettingsModel::coreDumpingEnabled() const
{
    return value(Settings::ValueCoreDumping, QVariant(true)).toBool();
}

QString CReporterPrivacySettingsModel::enduranceCollectMark()
{
    return CReporterCoreRegistry::instance()->getCoreLocationPaths().first() +
           "/endurance-enabled-mark";
}

bool CReporterPrivacySettingsModel::enduranceEnabled() const
{
    return QFile::exists(enduranceCollectMark());
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::notificationsEnabled
// ----------------------------------------------------------------------------
bool CReporterPrivacySettingsModel::notificationsEnabled() const
{
    return value(Settings::ValueNotifications, QVariant(true)).toBool();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::autoDeleteDuplicates
// ----------------------------------------------------------------------------
bool CReporterPrivacySettingsModel::autoDeleteDuplicates() const
{
    return value(Settings::ValueAutoDeleteDuplicates, QVariant(true)).toBool();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::autoDeleteMaxSimilarCores
// ----------------------------------------------------------------------------
int CReporterPrivacySettingsModel::autoDeleteMaxSimilarCores() const
{
    return value(Settings::ValueAutoDeleteMaxSimilarCores, QVariant(5)).toInt();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::automaticSendingEnabled
// ----------------------------------------------------------------------------
bool CReporterPrivacySettingsModel::automaticSendingEnabled() const
{
    return value(Settings::ValueAutomaticSending, QVariant(true)).toBool();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::instantDialogsEnabled
// ----------------------------------------------------------------------------
bool CReporterPrivacySettingsModel::instantDialogsEnabled() const
{
    return value(Settings::ValueInstantDialogs, QVariant(false)).toBool();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::includeCore
// ----------------------------------------------------------------------------
bool CReporterPrivacySettingsModel::includeCore() const
{
    return value(Privacy::ValueIncludeCore, QVariant(true)).toBool();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::includeSystemLog
// ----------------------------------------------------------------------------
bool CReporterPrivacySettingsModel::includeSystemLog() const
{
    return value(Privacy::ValueIncludeSysLog, QVariant(true)).toBool();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::includePackageList
// ----------------------------------------------------------------------------
bool CReporterPrivacySettingsModel::includePackageList() const
{
    return value(Privacy::ValueIncludePkgList, QVariant(true)).toBool();
}

bool CReporterPrivacySettingsModel::includeStackTrace() const
{
    return value(Privacy::ValueIncludeStackTrace, QVariant(false)).toBool();
}

bool CReporterPrivacySettingsModel::downloadDebuginfo() const
{
    return value(Privacy::ValueDownloadDebuginfo, QVariant(false)).toBool();
}

bool CReporterPrivacySettingsModel::privacyNoticeAccepted() const
{
    return value(Settings::ValueNoticeAccepted, QVariant(false)).toBool();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::reduceCore
// ----------------------------------------------------------------------------
bool CReporterPrivacySettingsModel::reduceCore() const
{
    return value(Privacy::ValueReduceCore, QVariant(true)).toBool();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::setCoreDumpingEnabled
// ----------------------------------------------------------------------------
void CReporterPrivacySettingsModel::setCoreDumpingEnabled(bool value)
{
    if (setValue(Settings::ValueCoreDumping, QVariant(value)))
        emit coreDumpingEnabledChanged();
}

void CReporterPrivacySettingsModel::setEnduranceEnabled(bool value)
{
    if (value == enduranceEnabled()) {
        return;
    }

    if (value) {
        QFile(enduranceCollectMark()).open(QFile::WriteOnly);
    } else {
        QFile::remove(enduranceCollectMark());
    }

    emit enduranceEnabledChanged();
}

void CReporterPrivacySettingsModel::setNotificationsEnabled(bool value)
{
    if (setValue(Settings::ValueNotifications, QVariant(value)))
        emit notificationsEnabledChanged();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::setAutoDeleteDuplicates
// ----------------------------------------------------------------------------
void CReporterPrivacySettingsModel::setAutoDeleteDuplicates(bool value)
{
    if (setValue(Settings::ValueAutoDeleteDuplicates, QVariant(value)))
        emit autoDeleteDuplicatesChanged();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::setAutoDeleteMaxSimilarCores
// ----------------------------------------------------------------------------
void CReporterPrivacySettingsModel::setAutoDeleteMaxSimilarCores(int value)
{
    setValue(Settings::ValueAutoDeleteMaxSimilarCores, QVariant(value));
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::setAutomaticSendingEnabled
// ----------------------------------------------------------------------------
void CReporterPrivacySettingsModel::setAutomaticSendingEnabled(bool value)
{
    setValue(Settings::ValueAutomaticSending, QVariant(value));
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::setInstantDialogsEnabled
// ----------------------------------------------------------------------------
void CReporterPrivacySettingsModel::setInstantDialogsEnabled(bool value)
{
    setValue(Settings::ValueInstantDialogs, QVariant(value));
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::setIncludeCore
// ----------------------------------------------------------------------------
void CReporterPrivacySettingsModel::setIncludeCore(bool value)
{
   setValue(Privacy::ValueIncludeCore, QVariant(value));
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::setIncludeSystemLog
// ----------------------------------------------------------------------------
void CReporterPrivacySettingsModel::setIncludeSystemLog(bool value)
{
   setValue(Privacy::ValueIncludeSysLog, QVariant(value));
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::setIncludePackageList
// ----------------------------------------------------------------------------
void CReporterPrivacySettingsModel::setIncludePackageList(bool value)
{
   setValue(Privacy::ValueIncludePkgList, QVariant(value));
}

void CReporterPrivacySettingsModel::setIncludeStackTrace(bool value)
{
    if (setValue(Privacy::ValueIncludeStackTrace, QVariant(value)))
        emit includeStackTraceChanged();
}

void CReporterPrivacySettingsModel::setDownloadDebuginfo(bool value)
{
    if (setValue(Privacy::ValueDownloadDebuginfo, QVariant(value)))
        emit downloadDebuginfoChanged();
}

void CReporterPrivacySettingsModel::setPrivacyNoticeAccepted(bool value)
{
    if (setValue(Settings::ValueNoticeAccepted, QVariant(value)))
        emit privacyNoticeAcceptedChanged();
}

// ----------------------------------------------------------------------------
// CReporterPrivacySettingsModel::setReduceCore
// ----------------------------------------------------------------------------
void CReporterPrivacySettingsModel::setReduceCore(bool value)
{
    setValue(Privacy::ValueReduceCore, QVariant(value));
}

// End of file.

