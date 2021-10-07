QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    loginform.cpp \
    satisform.cpp \
    sepet.cpp \
    stokfrom.cpp \
    stokgrupform.cpp \
    stokkarti.cpp \
    user.cpp

HEADERS += \
    loginform.h \
    satisform.h \
    sepet.h \
    stokfrom.h \
    stokgrupform.h \
    stokkarti.h \
    user.h

FORMS += \
    loginform.ui \
    satisform.ui \
    stokfrom.ui \
    stokgrupform.ui

TRANSLATIONS += \
    mhss_tr_TR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
