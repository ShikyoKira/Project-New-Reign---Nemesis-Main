#include "Global.h"

#include <fstream>

#include "ui/ErrorMsgBox.h"
#include "ui/SettingsSave.h"

#include "utilities/alphanum.hpp"
#include "utilities/writetextfile.h"

void createLanguageCache(std::string language)
{
	if (CreateFolder("cache"))
	{
		FileWriter cachefile("cache\\language setting");

		if (cachefile.is_open())
		{
			cachefile << language;
		}
		else
		{
			CEMsgBox* msg = new CEMsgBox;
			msg->setWindowTitle("ERROR");
			msg->setText("Error: Failed to create cache file. Please ensure Nemesis has permission to create file in current folder");
			msg->show();
		}
	}
}

void createModCache(std::vector<std::string> chosenBehavior)
{
    if (CreateFolder("cache")) {
        FileWriter cachefile("cache\\mod settings");

        if (cachefile.is_open()) {
            for (auto &line : chosenBehavior) {
                cachefile << line + "\n";
            }
        } else {
            CEMsgBox *msg = new CEMsgBox;
            msg->setWindowTitle("ERROR");
            msg->setText("Error: Failed to create cache file. Please ensure Nemesis has permission "
                         "to create file in current folder");
            msg->show();
        }
    }
}

void createModOrderCache(std::vector<std::string> behaviorList)
{
    if (CreateFolder("cache")) {
        FileWriter cachefile("cache\\order list");

        if (cachefile.is_open()) {
            for (auto &line : behaviorList) {
                cachefile << line + "\n";
            }
        } else {
            CEMsgBox *msg = new CEMsgBox;
            msg->setWindowTitle("ERROR");
            msg->setText("Error: Failed to create cache file. Please ensure Nemesis has permission "
                         "to create file in current folder");
            msg->show();
        }
    }
}

bool getCache(std::string& language, std::unordered_map<std::string, bool>& chosenBehavior)
{
    if (!isFileExist("cache")) {
        CreateFolder("cache");
        return false;
    }

    std::string filename = "cache\\language setting";
    vecstr storeline;

    auto errFunc = [](std::string &language) {
        QMessageBox *msg = new QMessageBox;
        msg->setWindowTitle("WARNING");
        msg->setText("Warning: Failed to read language cache file. Language is set to the default "
                     "language (english)");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->setIcon(QMessageBox::Warning);
        msg->show();
        language = "english";
        createLanguageCache(language);
    };

    if (!isFileExist(filename) || !GetFunctionLines(filename, storeline, false)) {
        errFunc(language);
    } else {
        for (auto &line : storeline) {
            if (line.length() > 0) {
                language = line;
            }
        }

        if (language.length() == 0) {
            errFunc(language);
        }
    }

    filename = "cache\\mod settings";
    auto errFunc2 = []() {
        QMessageBox *msg = new QMessageBox;
        msg->setWindowTitle("WARNING");
        msg->setText(
            "Warning: Failed to read mod cache file. All mods will be reverted to unchecked state");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->setIcon(QMessageBox::Warning);
        msg->show();
        createModCache({""});
    };

    try {
        if (!isFileExist(filename) || !GetFunctionLines(filename, storeline, false)) {
            errFunc2();
            return false;
        }
    } catch (const std::exception &) {
        errFunc2();
        return false;
    }

    for (auto &line : storeline) {
        if (line.length() > 0) {
            chosenBehavior[line] = true;
        }
    }

    if (chosenBehavior.size() > 0) {
        return true;
    }

    return false;
}

bool getCache(std::wstring& language, std::unordered_map<std::string, bool>& chosenBehavior)
{
    if (!isFileExist("cache")) {
        CreateFolder("cache");
        return false;
    }

    std::wstring filename = L"cache\\language setting";
    std::vector<std::wstring> storeline;

    auto errFunc = [](std::wstring &language) {
        QMessageBox *msg = new QMessageBox;
        msg->setWindowTitle("WARNING");
        msg->setText("Warning: Failed to read language cache file. Language is set to the default "
                     "language (english)");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->setIcon(QMessageBox::Warning);
        msg->show();
        language = L"english";
        createLanguageCache(WStringToString(language));
    };

    if (!isFileExist(filename) || !GetFunctionLines(filename, storeline, false)) {
        errFunc(language);
    } else {
        for (auto &line : storeline) {
            if (line.length() > 0) {
                language = line;
            }
        }

        if (language.length() == 0) {
            errFunc(language);
        }
    }

    filename = L"cache\\mod settings";
    auto errFunc2 = []() {
        QMessageBox *msg = new QMessageBox;
        msg->setWindowTitle("WARNING");
        msg->setText("Warning: Failed to read mod cache file. All mods will be reverted to unchecked state");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->setIcon(QMessageBox::Warning);
        msg->show();
        createModCache({""});
    };

    try
    {
        if (!isFileExist(filename) || !GetFunctionLines(filename, storeline, false))
        {
            errFunc2();
            return false;
        }
    }
    catch (const std::exception &)
    {
        errFunc2();
        return false;
    }

    for (auto &line : storeline)
    {
        if (line.length() > 0)
        {
            chosenBehavior[WStringToString(line)] = true;
        }
    }

    if (chosenBehavior.size() > 0)
    {
        return true;
    }

    return false;
}

bool getOrderCache(std::vector<std::string>& orderList)
{
	if (!isFileExist("cache"))
	{
		CreateFolder("cache");
		return false;
	}

	std::string filename = "cache\\order list";
	std::vector<std::string> storeline;

	if (!isFileExist(filename))
	{
		return false;
	}

	if (!GetFunctionLines(filename, storeline, false))
	{
		return false;
	}

	orderList.clear();
	orderList.reserve(storeline.size());

	for (auto& line : storeline)
	{
		if (line.length() > 0)
		{
			orderList.push_back(line);
		}
	}

	return orderList.size() > 0;
}
