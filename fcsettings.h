#ifndef FCSETTINGS_H
#define FCSETTINGS_H

#include <QReadWriteLock>
#include <QSettings>

class FCSettings : public QSettings
{
Q_OBJECT
public:
    explicit FCSettings(QObject *parent = 0);
    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key,
                   const QVariant &defaultValue = QVariant()) const;

    enum NotifyStyle {
        Notify = 0,
        Download = 1
    };

    enum UpdateFrequency {
        Every30 = 0,
        EveryHour = 1,
        Every2hours = 2,
        EveryDay = 3,
        EveryWeek = 4
    };

private:
    QReadWriteLock *settingsRwl;

};

#endif // FCSETTINGS_H
