#include "updatechecker.h"

UpdateChecker::UpdateChecker(FCSettings *settings) : settings(settings)
{
}

void UpdateChecker::run()
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(handleResponse(QNetworkReply*)), Qt::DirectConnection);
    parser = new QJson::Parser();
    timer = new QTimer();
    checkForUpdates();
    connect(timer, SIGNAL(timeout()), this,
            SLOT(checkForUpdates()), Qt::DirectConnection);
    timer->start(freqToMs(settings->value("notify/frequency").toInt()));

    exec();

    timer->stop();
}

int UpdateChecker::freqToMs(int freq)
{
    switch(freq) {
    case FCSettings::Every30:
        return 1000 * 60 * 30;
    case FCSettings::EveryHour:
        return 1000 * 60 * 60;
    case FCSettings::Every2hours:
        return 1000 * 60 * 120;
    case FCSettings::EveryDay:
        return 1000 * 60 * 60 * 24;
    case FCSettings::EveryWeek:
        return 1000 * 60 * 60 * 24 * 7;
    default:
        return 1000 * 60 * 60;
    }
}

void UpdateChecker::checkForUpdates()
{
    manager->get(QNetworkRequest(
            QUrl("http://octarineparrot.com/assets/fcn/latest.json")));
}

void UpdateChecker::handleResponse(QNetworkReply *reply)
{
    bool ok;
    QVariantMap json = parser->parse(reply->readAll(), &ok).toMap();
    reply->deleteLater();
    if(!ok) {
        // Some kind of notification in the GUI?
        // Same for network error ^ connect above
        return;
    }
    int mag = settings->value("magazine/version").toInt();
    int news = settings->value("news/version").toInt();
    int pod = settings->value("podcast/version").toInt();

    // TODO: Move this to triggerNotify() - if the second connection fails
    //       then the user will never be notified
    settings->setValue("magazine/version", json["mag"].toInt());
    settings->setValue("news/version", json["news"].toInt());
    settings->setValue("podcast/version", json["pod"].toInt());

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(triggerNotify(QNetworkReply*)), Qt::DirectConnection);
    if(json["mag"].toInt() > mag) {
        nam->get(QNetworkRequest(
                QUrl("http://notifier.fullcirclemagazine.org/mag/"
                     + json["mag"].toString() + ".json")));
    }
    if(json["news"].toInt() > news) {
        nam->get(QNetworkRequest(
                QUrl("http://notifier.fullcirclemagazine.org/news/"
                     + json["news"].toString() + ".json")));
    }
    if(json["pod"].toInt() > pod) {
        nam->get(QNetworkRequest(
                QUrl("http://notifier.fullcirclemagazine.org/pod/"
                     + json["pod"].toString() + ".json")));
    }

}

void UpdateChecker::triggerNotify(QNetworkReply *reply)
{
    bool ok;
    QVariantMap json = parser->parse(reply->readAll(), &ok).toMap();
    reply->deleteLater();
    if(!ok) {
        // Some kind of notification in the GUI?
        // Same for network error ^ connect above
        return;
    }
    emit notify(json["title"].toString(),
                json["desc"].toString(),
                json["url"].toString());
    emit addDownloadUrl(json["dl"].toString());
}

void UpdateChecker::updateInterval(int freq)
{
    timer->setInterval(freqToMs(freq));
}
