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
#include "updatechecker.h"

UpdateChecker::UpdateChecker(FCSettings *settings) : settings(settings)
{
    if(settings->value("notify/beta", false).toBool()) {
        baseUrl = "http://notifier.fullcirclemagazine.org/beta/" + settings->value("notify/lang", "en").toString() + "/";
    } else {
        baseUrl = "http://notifier.fullcirclemagazine.org/" + settings->value("notify/lang", "en").toString() + "/";
    }
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
            QUrl(baseUrl + "latest.json")));
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
    bool updates = false;
    if(json["mag"].toInt() > mag) {
        updates = true;
        nam->get(QNetworkRequest(
                QUrl(baseUrl + "mag/" + json["mag"].toString() + ".json")));
    }
    if(json["news"].toInt() > news) {
        updates = true;
        nam->get(QNetworkRequest(
                QUrl(baseUrl + "news/" + json["news"].toString()+".json")));
    }
    if(json["pod"].toInt() > pod) {
        updates = true;
        nam->get(QNetworkRequest(
                QUrl(baseUrl + "pod/" + json["pod"].toString() + ".json")));
    }
    if(!updates) {
        emit noUpdates();
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
