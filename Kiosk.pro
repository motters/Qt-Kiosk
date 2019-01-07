requires(qtConfig(accessibility))

TEMPLATE = app

QT += qml quick webengine

SOURCES += main.cpp

RESOURCES += qml.qrc

qtHaveModule(widgets) {
    QT += widgets # QApplication is required to get native styling with QtQuickControls
}

target.path = $$[QT_INSTALL_EXAMPLES]/webengine/minimal
INSTALLS += target
