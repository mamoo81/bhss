QT       += core gui sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adetdialogform.cpp \
    ayarlardialog.cpp \
    cari.cpp \
    carikartlardialog.cpp \
    fiyatgorform.cpp \
    hizliurunekleformdialog.cpp \
    kgform.cpp \
    main.cpp \
    loginform.cpp \
    paracekdialogform.cpp \
    satisform.cpp \
    satisgosterdialog.cpp \
    satisyapform.cpp \
    sepet.cpp \
    stokfrom.cpp \
    stokgrupform.cpp \
    stokkarti.cpp \
    urun.cpp \
    user.cpp \
    veritabani.cpp

HEADERS += \
    adetdialogform.h \
    ayarlardialog.h \
    cari.h \
    carikartlardialog.h \
    fiyatgorform.h \
    hizliurunekleformdialog.h \
    kgform.h \
    loginform.h \
    paracekdialogform.h \
    satisform.h \
    satisgosterdialog.h \
    satisyapform.h \
    sepet.h \
    stokfrom.h \
    stokgrupform.h \
    stokkarti.h \
    urun.h \
    user.h \
    veritabani.h

FORMS += \
    adetdialogform.ui \
    ayarlardialog.ui \
    carikartlardialog.ui \
    fiyatgorform.ui \
    hizliurunekleformdialog.ui \
    kgform.ui \
    loginform.ui \
    paracekdialogform.ui \
    satisform.ui \
    satisgosterdialog.ui \
    satisyapform.ui \
    stokfrom.ui \
    stokgrupform.ui

TRANSLATIONS += \
    mhss_tr_TR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES += \
    Readme.md
