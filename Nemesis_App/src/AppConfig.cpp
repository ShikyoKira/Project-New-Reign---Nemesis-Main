#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJSValue>
#include <sstream>
#include <unordered_map>

#include "AppConfig.h"

QString AppConfig::formatPlatform(const QString& platform) const
{
    constexpr std::array<const char*, 5> platforms = {"win32", "amd64", "ps3", "ps4", "xb360"};

    for (auto& pltrm : platforms)
    {
        if (pltrm == platform) return platform;
    }

    QMessageBox::critical(nullptr,
                          tr("Configuration Error"),
                          tr("Unsupported platform. Only win32, amd64, ps3, ps4 and xb360 are supported"));
    exit(-1);
    return "";
}

AppConfig::AppConfig(const std::filesystem::path& filepath, QObject* parent)
    : QObject{parent}
{
    IniSettings   = std::make_unique<QSettings>(QDir(filepath).path(), QSettings::IniFormat);
    Platform      = formatPlatform(IniSettings->value("Platform", "win32").toString());
    Width         = IniSettings->value("Width", "900").toInt();
    Height        = IniSettings->value("Height", "750").toInt();
    ModNameWidth  = IniSettings->value("ModNameWidth", "550").toInt();
    AuthorWidth   = IniSettings->value("AuthorWidth", "150").toInt();
    PriorityWidth = IniSettings->value("PriorityWidth", "50").toInt();
    DevMode       = IniSettings->value("DevMode", "false").toBool();

    auto dir      = IniSettings->value("DataDirectory");
    DataDirectory = dir.type() == QVariant::ByteArray ? QString::fromUtf8(dir.toByteArray()) : dir.toString();
    dir           = IniSettings->value("StageDirectory");
    StageDirectory
        = dir.type() == QVariant::ByteArray ? QString::fromUtf8(dir.toByteArray()) : dir.toString();

    QByteArray base64    = IniSettings->value("ListData").toByteArray();
    QByteArray json_data = QByteArray::fromBase64(base64);
    QJsonDocument doc    = QJsonDocument::fromJson(json_data);

    QJsonArray mod_order_list = doc.array();
    ModListData.clear();

    for (const QJsonValue& each : mod_order_list)
    {
        ModListData.append(each.toObject().toVariantMap());
    }

    if (!Platform.isEmpty()) return;

    QMessageBox::critical(nullptr, tr("Configuration Error"), tr("Platform type cannot be found. Only win32, amd64, ps3, ps4 and xb360 are supported"));
    exit(-1);
}

AppConfig::~AppConfig()
{
    if (!IniSettings) return;

    IniSettings->setValue("DataDirectory", DataDirectory);
    IniSettings->setValue("StageDirectory", StageDirectory);
    IniSettings->setValue("Platform", Platform);
    IniSettings->setValue("Width", QString::number(Width));
    IniSettings->setValue("Height", QString::number(Height));
    IniSettings->setValue("ModNameWidth", QString::number(ModNameWidth));
    IniSettings->setValue("AuthorWidth", QString::number(AuthorWidth));
    IniSettings->setValue("PriorityWidth", QString::number(PriorityWidth));
    IniSettings->setValue("DevMode", DevMode ? "true" : "false");

    QJsonArray mod_order_list;

    for (auto& each : ModListData)
    {
        mod_order_list.append(each.toJsonObject());
    }

    QJsonDocument doc(mod_order_list);
    QByteArray json_data = doc.toJson();
    QByteArray base64    = json_data.toBase64();
    IniSettings->setValue("ListData", QString::fromUtf8(base64));
}

QString AppConfig::getDataDirectory() const
{
    return DataDirectory;
}

QString AppConfig::getStageDirectory() const
{
    return StageDirectory;
}

QString AppConfig::getPlatform() const
{
    return Platform;
}

int AppConfig::getWidth() const
{
    return Width;
}

int AppConfig::getHeight() const
{
    return Height;
}

int AppConfig::getModNameWidth() const
{
    return ModNameWidth;
}

int AppConfig::getAuthorWidth() const
{
    return AuthorWidth;
}

int AppConfig::getPriorityWidth() const
{
    return PriorityWidth;
}

bool AppConfig::isDevMode() const
{
    return DevMode;
}

Q_INVOKABLE const QVariantList& AppConfig::getModListData() const
{
    return ModListData;
}

Q_INVOKABLE void AppConfig::setModListData(const QVariantList& mod_order_list)
{
    ModListData = mod_order_list;
}
