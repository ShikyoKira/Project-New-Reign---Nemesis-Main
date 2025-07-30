#pragma once

#include <QObject>
#include <QVariantList>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include "AppConfig.h"

class ModModelHandler : public QObject
{
    Q_OBJECT

private:
    AppConfig& Config;

public:
    explicit ModModelHandler(AppConfig& config, QObject* parent = nullptr);

    Q_INVOKABLE void populateModel(const QString& mods_dir, bool is_devmode);

signals:
    void dataReady(const QVariantList& data);
};

