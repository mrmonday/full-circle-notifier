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
        QMessageBox::critical(0, QObject::tr("Full Circle Notifier"),
                              QObject::tr("Unable to find a system tray,"
                                          " exitting."));
        return 1;
    }

    if (!QSystemTrayIcon::supportsMessages()) {
        QMessageBox::critical(0, QObject::tr("Full Circle Notifier"),
                              QObject::tr("System tray does not support "
                                          "notifications, exitting."));
        return 1;
    }

    FCNotify w;

    a.setActivationWindow(&w);

    return a.exec();
}
