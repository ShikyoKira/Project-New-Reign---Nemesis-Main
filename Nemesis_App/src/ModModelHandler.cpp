#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QDirIterator>

#include <iostream>

#include "ModModelHandler.h"
#include "ModInfo.h"

ModModelHandler::ModModelHandler(AppConfig& config, QObject* parent)
    : QObject(parent)
    , Config(config)
{
}

void ModModelHandler::populateModel(const QString& mods_dir, bool is_devmode)
{
    QDirIterator it(mods_dir, QDir::Dirs);
    const QVariantList& list_data = Config.getModListData();
    std::unordered_map<std::string, std::pair<QVariantMap, int>> order_map;

    for (size_t i = 0; i < list_data.size(); i++)
    {
        const QVariantMap& map = list_data[i].value<QVariantMap>();
        std::string str        = map.value("mod_code").toString().toStdString();
        order_map[str]         = std::make_pair(map, i);
    }

    std::vector<ModInfo> mod_info_list;

    while (it.hasNext())
    {
        QDir entry     = it.next();
        QDir ini_path  = entry.filePath("info.ini");
        auto ini_fpath = ini_path.filesystemPath();

        if (!std::filesystem::exists(ini_fpath)) continue;

        ModInfo& mod_info = mod_info_list.emplace_back();
        mod_info.ReadFile(ini_fpath);
        mod_info.SetModCode(QFileInfo(entry.path()).baseName().toLower().toStdString());
    }

    std::sort(mod_info_list.begin(),
              mod_info_list.end(),
              [&](const ModInfo& a, const ModInfo& b)
              {
                  bool a_in_order = order_map.find(a.GetModCode()) != order_map.end();
                  bool b_in_order = order_map.find(a.GetModCode()) != order_map.end();

                  if (!a_in_order && !b_in_order) return false;
                  if (!a_in_order) return true;
                  if (!b_in_order) return false;

                  return order_map[a.GetModCode()].second < order_map[b.GetModCode()].second;
              });

    QVariantList data_list;

    for (auto& mod_info : mod_info_list)
    {
        QVariantMap mod_info_map;

        if (is_devmode)
        {
            auto itr = order_map.find(mod_info.GetModCode());
            mod_info_map["checked"] = itr != order_map.end() && itr->second.first.value("checked").toBool();
        }
        else
        {
            mod_info_map["checked"] = true;
        }

        mod_info_map["display_name"] = QString::fromStdString(mod_info.GetDisplayName().data());
        mod_info_map["name"]         = QString::fromStdString(mod_info.GetName().data());
        mod_info_map["author"]       = QString::fromStdString(mod_info.GetAuthor().data());
        mod_info_map["mod_code"]     = QString::fromStdString(mod_info.GetModCode().data());
        mod_info_map["site"]         = QString::fromStdString(mod_info.GetSite().data());
        data_list.append(mod_info_map);
    }

    emit dataReady(data_list);
}
