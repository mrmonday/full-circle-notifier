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
#include <QtGui>
#include <QtSingleApplication>
#include "fcnotify.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);

    if(a.isRunning()) {
        QMessageBox::information(0, QObject::tr("Full Circle Notifier"),
                                 QObject::tr("Full Circle Notifier is "
                                             "already running!"));
        return 0;
    }

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        // TODO: This is a workaround for a bug Ronnie reported - we should
        //       look into replacing this
        for(int i = 0; i < 3; i++) {
            QTimer t;
            t.setSingleShot(true);
            t.start(2000);
            if(QSystemTrayIcon::isSystemTrayAvailable()) {
                break;
            } else {
                if(i < 3) continue;
                QMessageBox::critical(0, QObject::tr("Full Circle Notifier"),
                                      QObject::tr("Unable to find a system tray,"
                                                  " exitting."));
                return 1;
            }
        }
    }

    if (!QSystemTrayIcon::supportsMessages()) {
        QMessageBox::critical(0, QObject::tr("Full Circle Notifier"),
                              QObject::tr("System tray does not support "
                                          "notifications, exitting."));
        return 1;
    }

    FCNotify w;

    a.setActivationWindow(&w);
    a.setQuitOnLastWindowClosed(false);

    return a.exec();
}
