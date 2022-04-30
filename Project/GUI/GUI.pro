QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#QMAKE_CXX = GCC #/home/greenhoody/spack/opt/spack/linux-ubuntu20.04-zen2/gcc-9.4.0/gcc-11.2.0-ak6y6oemg5aewqgo5pp4u3jmg74fyakr/bin/gcc
#QMAKE_CC = GCC #/home/greenhoody/spack/opt/spack/linux-ubuntu20.04-zen2/gcc-9.4.0/gcc-11.2.0-ak6y6oemg5aewqgo5pp4u3jmg74fyakr/bin/gcc
QMAKE_CXXFLAGS += --shared -fopenacc -foffload=nvptx-none -fcf-protection=none

SOURCES += \
    cudafactory.cpp \
    main.cpp \
    mainwindow.cpp \
    mygraphicsview.cpp \
    factory.cpp \
    simulation.cpp \
    openaccfactory.cpp \
    openaccsimulation.cpp \
    noteditedfactory.cpp \
    noteditedsimulation.cpp


HEADERS += \
    cudafactory.h \
    mainwindow.h \
    mygraphicsview.h \
    factory.h \
    openaccsimulation.h \
    simulation.h \
    openaccfactory.h \
    noteditedfactory.h \
    noteditedsimulation.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    GUI_en_150.ts
CONFIG += lrelease
CONFIG += embed_translations


unix {
    LIBS += -L"../GUI" -laccgpu -L"../CUDA" -lcuda

}

INCLUDEPATH += ../GUI ../CUDA

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
