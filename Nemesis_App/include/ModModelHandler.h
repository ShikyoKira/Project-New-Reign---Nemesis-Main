#pragma once

#include <QObject>
#include <QVariantList>
#include <QDir>
#include <QFile>
#include <QTextStream>

class ModModelHandler : public QObject
{
    Q_OBJECT

public:
    explicit ModModelHandler(QObject* parent = nullptr);

    Q_INVOKABLE void populateModel(const QString& mods_dir, bool is_devmode);

signals:
    void dataReady(const QVariantList& data);
};

