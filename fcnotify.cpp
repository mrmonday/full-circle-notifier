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
#include "fcnotify.h"
#include "ui_fcnotify.h"

FCNotify::FCNotify(QWidget *parent) :
    QWidget(parent),
    checkingUpdates(false),
    ui(new Ui::FCNotify)
{
    ui->setupUi(this);

    setWindowTitle(tr("Full Circle Notifier"));

    // Hide progess bar
    ui->progressBar->setVisible(false);
    QCommonStyle style;
    ui->closeDownloadButton->setIcon(style.standardIcon(QStyle::SP_DialogCloseButton));
    ui->closeDownloadButton->setVisible(false);

    // Set up icons
    setupIcons();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    trayIcon->show();

    // Load and configure settings
    setupSettings();

    // Set up config window
    setupConfig();

    // Set up the timer
    setupTimer();

}

FCNotify::~FCNotify()
{
    delete ui;
}

void FCNotify::cancelDownload()
{
    emit abortDownload();
    ui->progressBar->setVisible(false);
    ui->closeDownloadButton->setVisible(false);
    int perc = (ui->progressBar->value() - ui->progressBar->minimum())/
               (ui->progressBar->maximum() - ui->progressBar->minimum());
    if(perc != 1) {
        QMessageBox::information(this, tr("Full Circle Notifier"),
                                 tr("Download cancelled."));
    }
}

void FCNotify::checkForUpdates()
{
    checkingUpdates = true;
    emit checkUpdates();
}

void FCNotify::noUpdates()
{
    if(checkingUpdates) {
        QMessageBox::information(this, tr("Full Circle Notifier"),
                                 tr("No updates are currently available."));
        checkingUpdates = false;
    }
}

void FCNotify::endApp()
{
    if(doClose(false)) {
        emit qApp->quit();
    }
}

bool FCNotify::doClose(bool notify)
{
    QString message;
    if(settings->value("notify/style").toUInt() == FCSettings::Download) {
        if((settings->value("notify/mag").toBool() ||
            settings->value("notify/podcast").toBool()) &&
           ui->downloadFolderLabel->text() == tr("No folder chosen")) {
            message = tr("Please choose a download folder.");
        } else if(!settings->value("notify/mag").toBool() ||
                  !settings->value("notify/podcast").toBool()) {
            message = tr("Notification style can only be <i>Notify</i> if "
                         "neither <i>Full Circle Magazine</i> nor <i>Full "
                         "Circle Podcast</i> are selected.");
        }
    }
    if(message.length() > 0) {
        QMessageBox::information(this, tr("Full Circle Notifier"), message);
        return false;
    }
    if(notify) {
        QMessageBox::information(this, tr("Full Circle Notifier"),
                             tr("Full Circle Notifier will continue to run in "
                                "the background. To quit, right click the "
                                "tray icon, and click <b>Quit</b>."));
    }
    hide();
    return true;
}

void FCNotify::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible()) {
        doClose();
        event->ignore();
    }
}

void FCNotify::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        setVisible(!isVisible());
        break;
    default:
        ;
    }
}

void FCNotify::toggleDownload()
{
    if(!settings->value("notify/mag").toBool() &&
       !settings->value("notify/podcast").toBool()) {
        ui->downloadLabel->setEnabled(false);
        ui->downloadButton->setEnabled(false);
        ui->downloadFolderLabel->setEnabled(false);
    } else if(settings->value("notify/style").toUInt() == FCSettings::Download) {
        ui->downloadLabel->setEnabled(true);
        ui->downloadButton->setEnabled(true);
        ui->downloadFolderLabel->setEnabled(true);
    }
}

void FCNotify::notifyMagToggle(bool checked)
{
    settings->setValue("notify/mag", checked);
    toggleDownload();
}

void FCNotify::notifyPodToggle(bool checked)
{
    settings->setValue("notify/podcast", checked);
    toggleDownload();
}

void FCNotify::notifyNewsToggle(bool checked)
{
    settings->setValue("notify/news", checked);
    toggleDownload();
}

void FCNotify::notifyChanged(int index)
{
    if((settings->value("notify/mag").toBool() ||
        settings->value("notify/podcast").toBool()) && index == FCSettings::Download
       ) {
        ui->downloadLabel->setEnabled(true);
        ui->downloadButton->setEnabled(true);
        ui->downloadFolderLabel->setEnabled(true);
        settings->setValue("notify/style", FCSettings::Download);
    } else {
        ui->downloadLabel->setEnabled(false);
        ui->downloadButton->setEnabled(false);
        ui->downloadFolderLabel->setEnabled(false);
        settings->setValue("notify/style", FCSettings::Notify);
    }
}

void FCNotify::updateChanged(int index)
{
    settings->setValue("notify/frequency", index);
    emit updateInterval(index);
}

void FCNotify::handleButtonBox(QAbstractButton *)
{
    doClose();
}

void FCNotify::chooseFolder()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    if(dialog.exec()) {
        QString dir = dialog.selectedFiles().at(0);
        ui->downloadFolderLabel->setText(dir);
        settings->setValue("notify/directory", dir);
    }
}

void FCNotify::setupConfig()
{
    if(settings->value("notify/mag").toBool()) {
        ui->checkBoxMag->setChecked(true);
    }
    if(settings->value("notify/podcast").toBool()) {
        ui->checkBoxPodcast->setChecked(true);
    }
    if(settings->value("notify/news").toBool()) {
        ui->checkBoxNews->setChecked(true);
    }

    int style = settings->value("notify/style").toInt();

    ui->notificationStyle->insertItem(FCSettings::Notify, "Notify", 0);
    ui->notificationStyle->insertItem(FCSettings::Download, "Notify and download", 0);
    ui->notificationStyle->setCurrentIndex(style);

    if ((settings->value("notify/mag").toBool() ||
         settings->value("notify/podcast").toBool()) &&
        settings->value("notify/style").toUInt() == FCSettings::Download
        ) {
        ui->downloadLabel->setEnabled(true);
        ui->downloadButton->setEnabled(true);
        ui->downloadFolderLabel->setEnabled(true);
        if(settings->value("notify/directory").toString().length() > 0) {
            ui->downloadFolderLabel
                    ->setText(settings->value("notify/directory").toString());
        }
    }

    // Cache this, as inserting the first item triggers updateChanged(),
    // which resets this
    int freq = settings->value("notify/frequency").toInt();

    ui->updateFrequency->insertItem(FCSettings::Every30, tr("Every 30 minutes"), 0);
    ui->updateFrequency->insertItem(FCSettings::EveryHour, tr("Every hour"), 0);
    ui->updateFrequency->insertItem(FCSettings::Every2hours, tr("Every 2 hours"), 0);
    ui->updateFrequency->insertItem(FCSettings::EveryDay, tr("Every day"), 0);
    ui->updateFrequency->insertItem(FCSettings::EveryWeek, tr("Every week"), 0);
    ui->updateFrequency->setCurrentIndex(freq);

    // TODO: Automatically run at startup?
}

void FCNotify::setupIcons()
{
    QIcon fclogo(":/assets/fclogo.png");

    trayContextMenu = new QMenu(this);
    trayContextMenu->addAction(tr("Quit"), this, SLOT(endApp()), 0);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(fclogo);
    trayIcon->setContextMenu(trayContextMenu);
    trayIcon->setToolTip("Full Circle Notifier");
    setWindowIcon(fclogo);
}

void FCNotify::setupSettings()
{
    QCoreApplication::setOrganizationName("Octarine Parrot");
    QCoreApplication::setOrganizationDomain("octarineparrot.com");
    QCoreApplication::setApplicationName("Full Circle Notifier");
    settings = new FCSettings();
    if (settings->value("firstRun", true).toBool() == true) {
        show();
        settings->setValue("firstRun", false);
        settings->setValue("notify/mag", true);
        settings->setValue("notify/news", true);
        settings->setValue("notify/podcast", true);
        settings->setValue("notify/style", FCSettings::Notify);
        settings->setValue("notify/directory", "");
        settings->setValue("notify/frequency", FCSettings::EveryHour);
        settings->setValue("magazine/version", -1);
        settings->setValue("news/version", -1);
        settings->setValue("podcast/version", -1);
    }
}

void FCNotify::setupTimer()
{
    updateChecker = new UpdateChecker(settings);
    updateChecker->moveToThread(updateChecker);
    connect(updateChecker, SIGNAL(notify(QString,QString,QString)),
            this, SLOT(notify(QString,QString,QString)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(updateInterval(int)),
            updateChecker, SLOT(updateInterval(int)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(checkUpdates()), updateChecker,
            SLOT(checkForUpdates()), Qt::QueuedConnection);
    connect(updateChecker, SIGNAL(noUpdates()), this, SLOT(noUpdates()),
            Qt::QueuedConnection);

    QAction *quit = trayContextMenu->actions().at(0);

    QAction *checkUpdates = new QAction("Check for updates", this);
    connect(checkUpdates, SIGNAL(triggered()), this, SLOT(checkForUpdates()));
    trayContextMenu->insertAction(quit, checkUpdates);
    trayContextMenu->insertSeparator(quit);

    downloader = new Downloader(settings);
    downloader->moveToThread(downloader);
    connect(downloader, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(downloadProgress(qint64,qint64)), Qt::QueuedConnection);
    connect(updateChecker, SIGNAL(addDownloadUrl(QUrl)),
            downloader, SLOT(addUrl(QUrl)), Qt::QueuedConnection);
    connect(this, SIGNAL(abortDownload()), downloader, SLOT(cancelDownload()),
            Qt::QueuedConnection);
    downloader->start();

    updateChecker->start();
}

void FCNotify::notify(QString title, QString desc, QString url)
{
    clickedUrl = url;
    trayIcon->showMessage(title, desc, QSystemTrayIcon::Information, 100000);
}

void FCNotify::messageClicked()
{
    // TODO: This won't work if there's a new pod/mag/news simultanteously
    QDesktopServices::openUrl(QUrl(clickedUrl));
}

void FCNotify::downloadProgress(qint64 received, qint64 total)
{
    ui->progressBar->setVisible(true);
    ui->closeDownloadButton->setVisible(true);
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(received);
}

void FCNotify::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
