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
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "fcsettings.h"

#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QQueue>
#include <QThread>
#include <QUrl>

class Downloader : public QThread
{
    Q_OBJECT

public:
    Downloader(FCSettings *settings);
    void run();

signals:
    void downloadProgress(qint64, qint64);

private slots:
    void cancelDownload();
    void download(QNetworkReply*);
    void dlProgress(qint64, qint64);
    void addUrl(QUrl);

private:
    QNetworkReply *currentReply;
    bool downloading;
    QNetworkAccessManager *manager;
    FCSettings *settings;
    QQueue<QUrl> *urls;
};

#endif // DOWNLOADER_H
