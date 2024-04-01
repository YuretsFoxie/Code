QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comports.cpp \
    graph.cpp \
    main.cpp \
    mainwindow.cpp \
    signalgenerator.cpp \
    sound.cpp \
    spectrum.cpp \
    spectrumanalyzer.cpp

HEADERS += \
    comports.h \
    comsettingsdata.h \
    graph.h \
    mainwindow.h \
    signalgenerator.h \
    sound.h \
    spectrum.h \
    spectrumanalyzer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

# Added the qcustomplot.a to the BUILD directory, not the SOURCE
# qcustomplot.dll added to the release directory

# Tell the qcustomplot header that it will be used as library:
DEFINES += QCUSTOMPLOT_USE_LIBRARY

# Link with debug version of qcustomplot if compiling in debug mode, else with release library:
CONFIG(debug, release|debug) {
  win32:QCPLIB = qcustomplotd2
  else: QCPLIB = qcustomplotd
} else {
  win32:QCPLIB = qcustomplot2
  else: QCPLIB = qcustomplot
}
LIBS += -L./ -l$$QCPLIB
