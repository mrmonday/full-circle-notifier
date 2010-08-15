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
