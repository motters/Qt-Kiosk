#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qtwebengineglobal.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Merge user + program variables
    int newArgc = argc + 1 + 1;
    char** newArgv = new char*[newArgc];
    for(int i = 0; i < argc; i++)
    {
        newArgv[i] = argv[i];
    }

    // Disable web security
    char ARG_DISABLE_WEB_SECURITY[] = "--disable-web-security";
    newArgv[argc] = ARG_DISABLE_WEB_SECURITY;

    // Terminate
    newArgv[argc+1] = nullptr;

    // Start GUI application
    QGuiApplication app(newArgc, newArgv);

    // Init web engine for os
    QtWebEngine::initialize();

    // Start QML engine
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // Run program
    return app.exec();
}

