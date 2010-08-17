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
#include "fcsettings.h"

FCSettings::FCSettings(QObject *parent) :
    QSettings(parent)
{
    settingsRwl = new QReadWriteLock();
}

void FCSettings::setValue(const QString &key, const QVariant &value)
{
    settingsRwl->lockForWrite();
    QSettings::setValue(key, value);
    settingsRwl->unlock();
}

QVariant FCSettings::value(const QString &key,
                         const QVariant &defaultValue) const
{
    settingsRwl->lockForRead();
    QVariant ret = QSettings::value(key, defaultValue);
    settingsRwl->unlock();
    return ret;
}
