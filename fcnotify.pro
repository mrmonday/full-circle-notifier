# -------------------------------------------------
# Project created by QtCreator 2010-07-31T14:32:24
# -------------------------------------------------
QT += network \
    xml
TARGET = fcnotify
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
