# -------------------------------------------------
# Project created by QtCreator 2010-07-31T14:32:24
# fcnotifier: Notify the user of a new full circle issue/podcast/news item
# Copyright (C) 2010  Robert Clipsham <robert@octarineparrot.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# -------------------------------------------------
QT += network \
    xml
TARGET = fcnotifier
TEMPLATE = app
SOURCES += main.cpp \
    fcnotify.cpp \
    qtsingleapplication/qtsinglecoreapplication.cpp \
    qtsingleapplication/qtsingleapplication.cpp \
    qtsingleapplication/qtlockedfile_win.cpp \
    qtsingleapplication/qtlockedfile_unix.cpp \
    qtsingleapplication/qtlockedfile.cpp \
    qtsingleapplication/qtlocalpeer.cpp \
    updatechecker.cpp \
    fcsettings.cpp \
    downloader.cpp
HEADERS += fcnotify.h \
    qtsingleapplication/qtsinglecoreapplication.h \
    qtsingleapplication/qtsingleapplication.h \
    qtsingleapplication/qtlockedfile.h \
    qtsingleapplication/qtlocalpeer.h \
    updatechecker.h \
    fcsettings.h \
    downloader.h
FORMS += fcnotify.ui
RESOURCES += resources.qrc
OTHER_FILES += qtsingleapplication/qtsinglecoreapplication.pri \
    qtsingleapplication/QtSingleApplication \
    qtsingleapplication/qtsingleapplication.pri \
    qtsingleapplication/QtLockedFile
INCLUDEPATH += qtsingleapplication
LIBS += -lqjson
