#include "Global.h"

#include <fstream>

#include "ui/ErrorMsgBox.h"
#include "ui/SettingsSave.h"

#include "utilities/algorithm.h"
#include "utilities/alphanum.hpp"
#include "utilities/writetextfile.h"

namespace sf = std::filesystem;

void CacheError()
{
    QMessageBox* msg = new QMessageBox();
    msg->setWindowTitle("WARNING");
    msg->setText("Warning: Failed to read mod cache file. All mods will be reverted to unchecked state");
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->setIcon(QMessageBox::Warning);
    msg->show();
    CreateModCache({""});
}

void CacheLanguageError(const std::wstring& language)
{
    QMessageBox* msg = new QMessageBox();
    msg->setWindowTitle("WARNING");
    msg->setText("Warning: Failed to read language cache file. Language is set to the default "
                 "language (english)");
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->setIcon(QMessageBox::Warning);
    msg->show();
}

void CreateLanguageCache(const std::wstring& language)
{
    if (!std::filesystem::create_directories("cache")) return;

    FileWriter cachefile("cache\\language_settings");

    if (cachefile.is_open())
    {
        cachefile << language;
        return;
    }

    CEMsgBox* msg = new CEMsgBox();
    msg->setWindowTitle("ERROR");
    msg->setText("Error: Failed to create cache file. Please ensure Nemesis has permission to create "
                 "file in current folder");
    msg->show();
}

void CreateModCache(const VecStr& chosenBehavior)
{
    if (!sf::exists("cache") && !sf::create_directories("cache")) return;

    FileWriter cachefile("cache\\mod_settings");

    if (cachefile.is_open())
    {
        for (auto& line : chosenBehavior)
        {
            cachefile << line + "\n";
        }

        return;
    }

    CEMsgBox* msg = new CEMsgBox();
    msg->setWindowTitle("ERROR");
    msg->setText("Error: Failed to create cache file. Please ensure Nemesis has permission "
                 "to create file in current folder");
    msg->show();
}

void CreateModOrderCache(const VecStr& behaviorList)
{
    if (!sf::exists("cache") && !sf::create_directories("cache")) return;

    FileWriter cachefile("cache\\order_list");

    if (cachefile.is_open())
    {
        for (auto& line : behaviorList)
        {
            cachefile << line + "\n";
        }

        return;
    }

    CEMsgBox* msg = new CEMsgBox();
    msg->setWindowTitle("ERROR");
    msg->setText("Error: Failed to create cache file. Please ensure Nemesis has permission "
                 "to create file in current folder");
    msg->show();
}

std::wstring GetCachedLanguage()
{
    std::string filename = "cache\\language_settings";
    VecWstr storeline;
    std::wstring language;

    if (!isFileExist(filename) || !GetFileLines(filename, storeline, false))
    {
        language = L"english";
        CreateFolder("cache");
        CacheLanguageError(language);
        CreateLanguageCache(language);
        return language;
    }

    for (auto& line : storeline)
    {
        if (line.empty()) continue;

        language = line;
    }

    if (language.empty())
    {
        language = L"english";
        CacheLanguageError(language);
        CreateLanguageCache(language);
    }

    return language;
}

bool GetModSelectionCache(UMap<std::string, bool>& chosenBehavior)
{
    std::string filename = "cache\\mod_settings";
    VecStr lines;

    try
    {
        if (!isFileExist(filename) || !GetFileLines(filename, lines, false))
        {
            CacheError();
            return false;
        }
    }
    catch (const std::exception&)
    {
        CacheError();
        return false;
    }

    for (auto& line : lines)
    {
        if (line.empty()) continue;

        chosenBehavior[line] = true;
    }

    return !chosenBehavior.empty();
}

bool GetOrderCache(VecWstr& order_list)
{
    if (!isFileExist("cache"))
    {
        CreateFolder("cache");
        return false;
    }

    std::string filename = "cache\\order_list";
    VecWstr storeline;
    order_list.clear();

    if (!isFileExist(filename)) return false;

    if (!GetFileLines(
            filename, order_list, [](std::wstring& line) { return !line.empty(); }, false))
    {
        return false;
    }

    return !order_list.empty();
}
