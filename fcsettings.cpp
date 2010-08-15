#include "fcsettings.h"

FCSettings::FCSettings(QObject *parent) :
    QSettings(parent)
{
    settingsRwl = new QReadWriteLock();
}

void FCSettings::setValue(const QString &key, const QVariant &value)
{
    settingsRwl->lockForWrite();
    QSettings::setValue(key, value);
    settingsRwl->unlock();
}

QVariant FCSettings::value(const QString &key,
                         const QVariant &defaultValue) const
{
    settingsRwl->lockForRead();
    QVariant ret = QSettings::value(key, defaultValue);
    settingsRwl->unlock();
    return ret;
}
