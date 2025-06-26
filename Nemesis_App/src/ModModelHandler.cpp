#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

#include <iostream>

#include "ModModelHandler.h"
#include "ModInfo.h"

ModModelHandler::ModModelHandler(QObject *parent) : QObject(parent)
{
}

void ModModelHandler::populateModel(const QString& mods_dir, bool is_devmode)
{
    QVariantList data_list;

    for (auto& entry : std::filesystem::directory_iterator(mods_dir.toStdWString()))
    {
        if (!entry.is_directory()) continue;

        auto ini_path = entry.path() / "info.ini";

        if (!std::filesystem::exists(ini_path)) continue;

        ModInfo mod_info;
        mod_info.ReadFile(ini_path);
        mod_info.SetModCode(QString::fromStdWString(entry.path().stem().wstring()).toLower().toStdString());

        QVariantMap mod_info_map;
        mod_info_map["checked"]      = !is_devmode;
        mod_info_map["display_name"] = QString::fromStdString(mod_info.GetDisplayName().data());
        mod_info_map["name"]         = QString::fromStdString(mod_info.GetName().data());
        mod_info_map["author"]       = QString::fromStdString(mod_info.GetAuthor().data());
        mod_info_map["mod_code"]     = QString::fromStdString(mod_info.GetModCode().data());
        mod_info_map["site"]         = QString::fromStdString(mod_info.GetSite().data());
        data_list.append(mod_info_map);
    }

    emit dataReady(data_list);
}
