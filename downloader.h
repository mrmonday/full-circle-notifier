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
    void download(QNetworkReply*);
    void dlProgress(qint64, qint64);
    void addUrl(QUrl);

private:
    bool downloading;
    QNetworkAccessManager *manager;
    FCSettings *settings;
    QQueue<QUrl> *urls;
};

#endif // DOWNLOADER_H
