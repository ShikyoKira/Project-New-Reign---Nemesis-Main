#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>

#include "AppConfig.h"
#include "AppLauncher.h"
#include "ModModelHandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/resources/icon.png"));

    QQmlApplicationEngine engine;

    AppConfig app_config(std::filesystem::path(argv[0]).parent_path() / L"nemesis.ini");
    AppLauncher app_launcher;
    ModModelHandler mod_handler;

    QQmlContext* root_context = engine.rootContext();

    root_context->setContextProperty("appConfig", &app_config);
    root_context->setContextProperty("appLauncher", &app_launcher);
    root_context->setContextProperty("modHandler", &mod_handler);
    root_context->setContextProperty("appDirectoryPath", QGuiApplication::applicationDirPath());
    root_context->setContextProperty("appExecutablePath", QGuiApplication::applicationFilePath());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Nemesis_App", "Main");

    return app.exec();
}
