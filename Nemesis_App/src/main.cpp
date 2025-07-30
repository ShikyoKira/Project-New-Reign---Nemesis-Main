#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMessageBox>
#include <QIcon>

#include "AppConfig.h"
#include "AppLauncher.h"
#include "ModModelHandler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/resources/icon.png"));

    try
    {
        QQmlApplicationEngine engine;

        AppConfig app_config(std::filesystem::path(argv[0]).parent_path() / L"nemesis.ini");
        AppLauncher app_launcher;
        ModModelHandler mod_handler(app_config);

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
        QObject::connect(&app, &QApplication::aboutToQuit, &app_launcher, &AppLauncher::quitRunningProcess);
        engine.loadFromModule("Nemesis_App", "Main");
        return app.exec();
    }
    catch (const std::exception& ex)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QObject::tr("Exception Caught!"));
        msgBox.setInformativeText(ex.what());
        msgBox.setStandardButtons(QMessageBox::Abort);
        msgBox.setWindowTitle(QObject::tr("Application Error"));
        msgBox.exec();
        return -1;
    }
}
