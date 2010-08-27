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
#ifndef FCNOTIFY_H
#define FCNOTIFY_H

#include <QtGui>
#include "downloader.h"
#include "fcsettings.h"
#include "updatechecker.h"

namespace Ui {
    class FCNotify;
}

class FCNotify : public QWidget {
    Q_OBJECT
public:
    FCNotify(QWidget *parent = 0);
    ~FCNotify();

signals:
    void checkUpdates();
    void updateInterval(int);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private slots:
    void checkForUpdates();
    void chooseFolder();
    void downloadProgress(qint64 received, qint64 total);
    void endApp();
    void handleButtonBox(QAbstractButton *button);
    void messageClicked();
    void notify(QString title, QString desc, QString url);
    void notifyChanged(int index);
    void notifyMagToggle(bool checked);
    void notifyPodToggle(bool checked);
    void notifyNewsToggle(bool checked);
    void noUpdates();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void updateChanged(int index);

private:
    bool doClose(bool notify=true);
    void setupConfig();
    void setupIcons();
    void setupSettings();
    void setupTimer();
    void toggleDownload();

    bool checkingUpdates;
    QString clickedUrl;
    Downloader *downloader;
    FCSettings *settings;
    QMenu *trayContextMenu;
    QSystemTrayIcon *trayIcon;
    Ui::FCNotify *ui;
    UpdateChecker *updateChecker;
};

#endif // FCNOTIFY_H
