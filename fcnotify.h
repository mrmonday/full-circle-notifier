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
    void updateInterval(int);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private slots:
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
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void updateChanged(int index);

private:
    bool doClose(bool notify=true);
    void setupConfig();
    void setupIcons();
    void setupSettings();
    void setupTimer();
    void toggleDownload();

    QString clickedUrl;
    Downloader *downloader;
    FCSettings *settings;
    QMenu *trayContextMenu;
    QSystemTrayIcon *trayIcon;
    Ui::FCNotify *ui;
    UpdateChecker *updateChecker;
};

#endif // FCNOTIFY_H
