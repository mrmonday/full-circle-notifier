/**
 * fcnotifier: Notify the user of a new full circle issue/podcast/news item
 * Copyright (C) 2010  Robert Clipsham <robert@octarineparrot.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef FCSETTINGS_H
#define FCSETTINGS_H

#include <QReadWriteLock>
#include <QSettings>

class FCSettings : public QSettings
{
Q_OBJECT
public:
    explicit FCSettings(QObject *parent = 0);
    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key,
                   const QVariant &defaultValue = QVariant()) const;

    enum NotifyStyle {
        Notify = 0,
        Download = 1
    };

    enum UpdateFrequency {
        Every30 = 0,
        EveryHour = 1,
        Every2hours = 2,
        EveryDay = 3,
        EveryWeek = 4
    };

private:
    QReadWriteLock *settingsRwl;

};

#endif // FCSETTINGS_H
