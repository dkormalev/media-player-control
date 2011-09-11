VERSION = 0.2.3

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
    abstractplayercontrol.cpp
HEADERS += mainwindow.h \
    vlcplayercontrol.h \
    core.h \
    abstractplayercontrol.h
FORMS +=

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

RESOURCES += \
    images.qrc

symbian {
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
    qml/meego/SettingsPage.qml










