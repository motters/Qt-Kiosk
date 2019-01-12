requires(qtConfig(accessibility))

TEMPLATE = app

CONFIG += c++14

QT += qml quick webengine

SOURCES += main.cpp

RESOURCES += qml.qrc

qtHaveModule(widgets) {
    QT += widgets # QApplication is required to get native styling with QtQuickControls
}

win32 {
    #RC_FILE = Kiosk.rc
CONFIG += embed_manifest_exe
QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level='requireAdministrator'"
}

target.path = $$[QT_INSTALL_EXAMPLES]/webengine/minimal
INSTALLS += target

DISTFILES += \
    Kiosk.exe.manifest \
    Kiosk.rc

