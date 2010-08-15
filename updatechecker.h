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
