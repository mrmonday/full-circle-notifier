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
#include "downloader.h"

Downloader::Downloader(FCSettings *settings) : settings(settings)
{
    downloading = false;
    manager = new QNetworkAccessManager(this);
    urls = new QQueue<QUrl>();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(download(QNetworkReply*)), Qt::DirectConnection);
}

void Downloader::run()
{
    exec();
}

void Downloader::addUrl(QUrl url)
{
    if(settings->value("notify/style").toInt() == FCSettings::Download) {
        urls->enqueue(url);
        if(!downloading) {
            downloading = true;
            QNetworkReply *reply = manager->get(QNetworkRequest(urls->dequeue()));
            connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                    this, SLOT(dlProgress(qint64,qint64)), Qt::QueuedConnection);
        }
    }
}

void Downloader::dlProgress(qint64 received, qint64 total)
{
    emit downloadProgress(received, total);
}

void Downloader::download(QNetworkReply *reply)
{
    QUrl url = reply->url();
    QDir dir(settings->value("notify/directory").toString());
    QFile file( QDir::cleanPath(dir.absolutePath()) + "/" + url.toString().section("/", -1));
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.close();
        }
    }
    reply->deleteLater();
    if(urls->length() > 0) {
        QNetworkReply *rep = manager->get(QNetworkRequest(urls->dequeue()));
        connect(rep, SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(dlProgress(qint64,qint64)), Qt::QueuedConnection);
    }
    downloading = false;
}
