#ifndef EMSPATHS_H
#define EMSPATHS_H

#include <QtCore>

//! EMS path helpers - centralized location for all runtime file paths.
/*!
 * All settings and log paths are relative to the application directory so
 * the EMS can run from any install location on the embedded system.
 * Replace the applicationDirPath() call site with these helpers to avoid
 * scattered hard-coded path strings.
 */
namespace EmsPaths {

inline QString settingsDir()
{
    return QCoreApplication::applicationDirPath() + "/ems/settings";
}

inline QString settingsIni()
{
    return settingsDir() + "/settings.ini";
}

inline QString gaugeSettingsIni()
{
    return settingsDir() + "/gaugeSettings.ini";
}

inline QString engineLogsDir()
{
    return QCoreApplication::applicationDirPath() + "/ems/engineLogs";
}

inline QString appLogsDir()
{
    return QCoreApplication::applicationDirPath() + "/ems/appLogs";
}

} // namespace EmsPaths

#endif // EMSPATHS_H
