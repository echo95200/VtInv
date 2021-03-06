#-------------------------------------------------
#
# Project created by QtCreator 2018-06-21T17:30:26
#
#-------------------------------------------------

QT       += widgets sql script xml

QT       -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = VtInv
TEMPLATE = lib

DEFINES += VTINV_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        vtinv.cpp

HEADERS += \
        vtinv.h \
        vtinv_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#include($$PWD/LimeReport-1.4.7/limereport.pri)
#include($$PWD/LimeReport-master/limereport.pri)

RESOURCES += \
    img.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../LibLimeReportMaster/release/ -llimereportd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../LibLimeReportMaster/debug/ -llimereportd
else:unix: LIBS += -L$$PWD/../LibLimeReportMaster/ -llimereportd

INCLUDEPATH += $$PWD/../LibLimeReportMaster/include
DEPENDPATH += $$PWD/../LibLimeReportMaster/include
