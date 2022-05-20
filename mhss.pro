QT       += core gui sql multimedia printsupport charts serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adetdialogform.cpp \
    ayarlardialog.cpp \
    birimekleform.cpp \
    cari.cpp \
    carihareketiekleform.cpp \
    carikartlardialog.cpp \
    fiyatgorform.cpp \
    gecmissatislardialog.cpp \
    hizliurunekleformdialog.cpp \
    kasadialogform.cpp \
    kasahareketekledialog.cpp \
    kgform.cpp \
    kullanicidialogform.cpp \
    main.cpp \
    loginform.cpp \
    paracekdialogform.cpp \
    rafetiketidialog.cpp \
    resimekledialog.cpp \
    satisform.cpp \
    satisgosterdialog.cpp \
    satisgrafigiform.cpp \
    satisyapform.cpp \
    sepet.cpp \
    stokfrom.cpp \
    stokgircikdialog.cpp \
    stokgrupform.cpp \
    stokhareketleridialog.cpp \
    stokkarti.cpp \
    stokkartlarimodel.cpp \
    terazi.cpp \
    toplustokyukledialog.cpp \
    urun.cpp \
    user.cpp \
    veritabani.cpp \
    yazici.cpp \
    yenicariform.cpp \
    yenicarikartdialog.cpp

HEADERS += \
    adetdialogform.h \
    ayarlardialog.h \
    birimekleform.h \
    cari.h \
    carihareketiekleform.h \
    carikartlardialog.h \
    fiyatgorform.h \
    gecmissatislardialog.h \
    hizliurunekleformdialog.h \
    kasadialogform.h \
    kasahareketekledialog.h \
    kgform.h \
    kullanicidialogform.h \
    loginform.h \
    paracekdialogform.h \
    rafetiketidialog.h \
    resimekledialog.h \
    satisform.h \
    satisgosterdialog.h \
    satisgrafigiform.h \
    satisyapform.h \
    sepet.h \
    stokfrom.h \
    stokgircikdialog.h \
    stokgrupform.h \
    stokhareketleridialog.h \
    stokkarti.h \
    stokkartlarimodel.h \
    terazi.h \
    toplustokyukledialog.h \
    urun.h \
    user.h \
    veritabani.h \
    yazici.h \
    yenicariform.h \
    yenicarikartdialog.h

FORMS += \
    adetdialogform.ui \
    ayarlardialog.ui \
    birimekleform.ui \
    carihareketiekleform.ui \
    carikartlardialog.ui \
    fiyatgorform.ui \
    gecmissatislardialog.ui \
    hizliurunekleformdialog.ui \
    kasadialogform.ui \
    kasahareketekledialog.ui \
    kgform.ui \
    kullanicidialogform.ui \
    loginform.ui \
    paracekdialogform.ui \
    rafetiketidialog.ui \
    resimekledialog.ui \
    satisform.ui \
    satisgosterdialog.ui \
    satisgrafigiform.ui \
    satisyapform.ui \
    stokfrom.ui \
    stokgircikdialog.ui \
    stokgrupform.ui \
    stokhareketleridialog.ui \
    toplustokyukledialog.ui \
    yenicariform.ui \
    yenicarikartdialog.ui

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES += \
    LICENCE \
    Readme.md
