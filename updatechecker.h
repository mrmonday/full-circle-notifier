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
#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include "fcsettings.h"

#include <qjson/parser.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QThread>
#include <QTimer>
#include <QUrl>

class UpdateChecker : public QThread
{
    Q_OBJECT

public:
    UpdateChecker(FCSettings*);
    void run();

signals:
    void notify(QString title, QString desc, QString url);
    void addDownloadUrl(QUrl);

private slots:
    void checkForUpdates();
    void handleResponse(QNetworkReply*);
    void triggerNotify(QNetworkReply*);
    void updateInterval(int);

private:
    int freqToMs(int freq);

    QJson::Parser *parser;
    QNetworkAccessManager *manager;
    FCSettings *settings;
    QTimer *timer;
};

#endif // UPDATECHECKER_H
