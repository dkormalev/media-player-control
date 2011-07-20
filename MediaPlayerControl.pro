VERSION = 0.2.2

QT += network declarative

# Add files and directories to ship with the application
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian {
    LIBS += -lws32 \
            -lcone
    TARGET.UID3 = 0xE75FA288

    # Allow network access on Symbian
    TARGET.CAPABILITY += NetworkServices

    # Smart Installer package's UID
    # This UID is from the protected range
    # and therefore the package will fail to install if self-signed
    # By default qmake uses the unprotected range value if unprotected UID is defined for the application
    # and 0x2002CCCF value if protected UID is given to the application
    #symbian:DEPLOYMENT.installer_header = 0x2002CCCF

    vendorinfo = "%{\"Denis Kormalev\"}" ":\"Denis Kormalev\""
    my_deployment.pkg_prerules = vendorinfo
    DEPLOYMENT += my_deployment
}


# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the
# MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp mainwindow.cpp \
    vlcplayercontrol.cpp \
    core.cpp \
    abstractplayercontrol.cpp \
    settingsdialog.cpp
HEADERS += mainwindow.h \
    vlcplayercontrol.h \
    core.h \
    abstractplayercontrol.h \
    settingsdialog.h
FORMS += mainwindow.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

RESOURCES += \
    qml/qml.qrc \
    images.qrc

OTHER_FILES += \
    qml/main.qml \
    qml/Button.qml \
    qml/VolumeSlider.qml \
    qml/TimeSlider.qml


