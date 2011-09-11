VERSION = 0.3.0

QT += network declarative
config += qtquickcomponents

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

    DEPLOYMENT.installer_header = 0x2002CCCF

    vendorinfo = "%{\"Denis Kormalev\"}" ":\"Denis Kormalev\""
    my_deployment.pkg_prerules = vendorinfo

    DEPLOYMENT += my_deployment
}


# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the
# MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp \
    vlcplayercontrol.cpp \
    core.cpp \
    abstractplayercontrol.cpp
HEADERS += \
    vlcplayercontrol.h \
    core.h \
    abstractplayercontrol.h

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

RESOURCES += \
    images.qrc

symbian|simulator {
    RESOURCES += \
        qml/qml_symbian.qrc
} else {
    CONFIG += qdeclarative-boostable
    RESOURCES += \
        qml/qml_meego.qrc
}

OTHER_FILES += \
    qml/symbian/main.qml \
    qml/meego/main.qml \
    qml/VolumeSlider.qml \
    qml/TimeSlider.qml \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/meego/ControlPage.qml \
    qml/PlayerButton.qml \
    qml/meego/SettingsPage.qml \
    qml/symbian/ControlPage.qml \
    qml/symbian/SettingsPage.qml














